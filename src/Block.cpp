//
//  Copyright (C) 2015-2021 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
//
//  This file is part of libcasm-ir.
//
//  libcasm-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcasm-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcasm-ir. If not, see <http://www.gnu.org/licenses/>.
//
//  Additional permission under GNU GPL version 3 section 7
//
//  libcasm-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcasm-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcasm-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcasm-ir give you permission to link libcasm-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcasm-ir. If you modify libcasm-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "Block.h"

#include "Instruction.h"
#include "Rule.h"
#include "Statement.h"

using namespace libcasm_ir;

Block::Block( Value::ID id )
: Value( libstdhl::Memory::get< LabelType >(), id )
, m_parent()
, m_scope()
{
}

void Block::clear( void )
{
    m_parent.reset();
    m_scope.reset();
}

void Block::setParent( const Block::Ptr& parent )
{
    assert( parent );
    m_parent = parent;
}

Block::Ptr Block::parent( void ) const
{
    return m_parent.lock();
}

void Block::setScope( const ExecutionSemanticsBlock::Ptr& scope )
{
    assert( scope );
    m_scope = scope;
}

ExecutionSemanticsBlock::Ptr Block::scope( void ) const
{
    return m_scope.lock();
}

std::string Block::name( void ) const
{
    if( isa< ExecutionSemanticsBlock >( this ) )
    {
        return static_cast< const ExecutionSemanticsBlock* >( this )->name();
    }
    else if( isa< Statement >( this ) )
    {
        return static_cast< const Statement* >( this )->name();
    }

    assert( !" invalid block to dispatch 'name' found! " );
    return "";
}

std::size_t Block::hash( void ) const
{
    return libstdhl::Hash::combine( classid(), std::hash< std::string >()( name() ) );
}

u1 Block::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const Block& >( rhs );
    return ( this->name() == other.name() );
}

u1 Block::classof( Value const* obj )
{
    return obj->id() == classid() or ExecutionSemanticsBlock::classof( obj ) or
           Statement::classof( obj );
}

ExecutionSemanticsBlock::ExecutionSemanticsBlock( const u1 parallel, Value::ID id )
: Block( id )
, m_parallel( parallel )
, m_pseudostate( 0 )
{
}

ExecutionSemanticsBlock::Ptr ExecutionSemanticsBlock::init( void )
{
    auto self = ptr_this< ExecutionSemanticsBlock >();

    m_entry = libstdhl::Memory::make< TrivialStatement >();
    m_entry->setScope( self );
    m_entry->setParent( self );

    auto f = libstdhl::Memory::make< ForkInstruction >();
    m_entry->add( f );

    m_exit = libstdhl::Memory::make< TrivialStatement >();
    m_exit->setScope( self );
    m_exit->setParent( self );

    auto m = libstdhl::Memory::make< MergeInstruction >();
    m_exit->add( m );

    return self;
}

u1 ExecutionSemanticsBlock::parallel( void ) const
{
    return m_parallel;
}

u64 ExecutionSemanticsBlock::pseudostate( void ) const
{
    return m_pseudostate;
}

std::shared_ptr< Statement > ExecutionSemanticsBlock::entry( void ) const
{
    return m_entry;
}

std::shared_ptr< Statement > ExecutionSemanticsBlock::exit( void ) const
{
    return m_exit;
}

Blocks& ExecutionSemanticsBlock::blocks( void )
{
    return m_blocks;
}

void ExecutionSemanticsBlock::add( const Block::Ptr& block )
{
    if( not block )
    {
        throw std::domain_error( "adding a null pointer block is not allowed" );
    }
    if( block->scope() )
    {
        throw std::domain_error(
            "block '" + block->dump() + "' is already bound to scope '" + block->scope()->dump() +
            "'" );
    }

    const auto self = ptr_this< ExecutionSemanticsBlock >();

    block->setScope( self );
    block->setParent( self );

    m_blocks.add( block );
}

void ExecutionSemanticsBlock::replace( Block& from, const Block::Ptr to )
{
    std::replace_if(
        m_blocks.begin(),
        m_blocks.end(),
        [&]( const Block::Ptr& v ) { return *v.get() == from; },
        to );

    from.clear();

    const auto self = ptr_this< ExecutionSemanticsBlock >();

    to->setScope( self );
    to->setParent( self );
}

std::string ExecutionSemanticsBlock::name( void ) const
{
    if( parallel() )
    {
        return "par";
    }
    else
    {
        return "seq";
    }
}

u1 ExecutionSemanticsBlock::classof( Value const* obj )
{
    return obj->id() == classid() or ParallelBlock::classof( obj ) or
           SequentialBlock::classof( obj );
}

//
//
// ParallelBlock
//

ParallelBlock::Ptr ParallelBlock::create( u1 empty )
{
    auto block = std::shared_ptr< ParallelBlock >( new ParallelBlock );

    if( not empty )
    {
        block->init();
    }

    return block;
}

ParallelBlock::ParallelBlock( void )
: ExecutionSemanticsBlock( true, classid() )
{
}

void ParallelBlock::setRule( const Rule::Ptr& rule )
{
    assert( rule );
    m_rule = rule;
}

Rule::Ptr ParallelBlock::rule( void ) const
{
    return m_rule.lock();
}

void ParallelBlock::replaceWith( const Block::Ptr block )
{
    if( auto r = rule() )
    {
        if( isa< ParallelBlock >( block ) )
        {
            r->setContext( std::static_pointer_cast< ParallelBlock >( block ) );
        }
        else
        {
            auto cxt = ParallelBlock::create();
            cxt->add( block );

            r->setContext( cxt );
        }
    }
    else
    {
        assert( parent() );
        assert( isa< ExecutionSemanticsBlock >( parent() ) );

        const auto blk = std::static_pointer_cast< ExecutionSemanticsBlock >( parent() );

        blk->replace( *this, block );
    }
}

void ParallelBlock::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 ParallelBlock::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
//
// SequentialBlock
//

SequentialBlock::Ptr SequentialBlock::create( u1 empty )
{
    auto block = std::shared_ptr< SequentialBlock >( new SequentialBlock );

    if( not empty )
    {
        block->init();
    }

    return block;
}

SequentialBlock::SequentialBlock( void )
: ExecutionSemanticsBlock( false, classid() )
{
}

void SequentialBlock::replaceWith( const Block::Ptr block )
{
    assert( parent() );
    assert( isa< ExecutionSemanticsBlock >( parent() ) );

    const auto blk = std::static_pointer_cast< ExecutionSemanticsBlock >( parent() );

    blk->replace( *this, block );
}

void SequentialBlock::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 SequentialBlock::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

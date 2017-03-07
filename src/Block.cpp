//
//  Copyright (c) 2015-2017 CASM Organization
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcasm-ir
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

#include "Block.h"

#include "Instruction.h"
#include "Rule.h"
#include "Statement.h"

using namespace libcasm_ir;

Block::Block( const std::string& name, Value::ID id )
: Value( name, libstdhl::get< LabelType >(), id )
, m_parent()
, m_scope()
{
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

u1 Block::classof( Value const* obj )
{
    return obj->id() == classid() or ExecutionSemanticsBlock::classof( obj )
           or Statement::classof( obj );
}

ExecutionSemanticsBlock::ExecutionSemanticsBlock(
    const std::string& name, u1 parallel, Value::ID id )
: Block( name, id )
, m_parallel( parallel )
, m_pseudostate( 0 )
{
}

ExecutionSemanticsBlock::~ExecutionSemanticsBlock( void )
{
}

ExecutionSemanticsBlock::Ptr ExecutionSemanticsBlock::init( void )
{
    auto self = ptr_this< ExecutionSemanticsBlock >();

    m_entry = libstdhl::make< TrivialStatement >();
    m_entry->setScope( self );
    m_entry->setParent( self );

    auto f = libstdhl::make< ForkInstruction >();
    m_entry->add( f );

    m_exit = libstdhl::make< TrivialStatement >();
    m_exit->setScope( self );
    m_exit->setParent( self );

    auto m = libstdhl::make< MergeInstruction >();
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

Blocks ExecutionSemanticsBlock::blocks( void ) const
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
        throw std::domain_error( "block '" + block->dump()
                                 + "' is already bound to scope '"
                                 + block->scope()->dump()
                                 + "'" );
    }

    const auto self = ptr_this< ExecutionSemanticsBlock >();

    block->setScope( self );
    block->setParent( self );

    m_blocks.add( block );
}

u1 ExecutionSemanticsBlock::classof( Value const* obj )
{
    return obj->id() == classid() or ParallelBlock::classof( obj )
           or SequentialBlock::classof( obj );
}

//
//
// ParallelBlock
//

ParallelBlock::ParallelBlock( void )
: ExecutionSemanticsBlock( "par", true, classid() )
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

u1 ParallelBlock::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
//
// SequentialBlock
//

SequentialBlock::SequentialBlock( void )
: ExecutionSemanticsBlock( "seq", false, classid() )
{
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

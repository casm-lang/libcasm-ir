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
    m_parent = parent;
}

Block::Ptr Block::parent( void ) const
{
    return m_parent.lock();
}

void Block::setScope( const ExecutionSemanticsBlock::Ptr& scope )
{
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

ExecutionSemanticsBlock::ExecutionSemanticsBlock( const std::string& name,
    u1 parallel, const ExecutionSemanticsBlock::Ptr& scope, Value::ID id )
: Block( name, id )
, m_parallel( parallel )
, m_pseudostate( 0 )
{
    setScope( scope );

    // auto s = libstdhl::make< TrivialStatement >( this );
    // add( s );
    // s->add( libstdhl::make< ForkInstruction >() );

    // s = libstdhl::make< TrivialStatement >( this );
    // add( s );
    // s->add( libstdhl::make< MergeInstruction >() );
}

ExecutionSemanticsBlock::~ExecutionSemanticsBlock( void )
{
}

u1 ExecutionSemanticsBlock::parallel( void ) const
{
    return m_parallel;
}

u64 ExecutionSemanticsBlock::pseudostate( void ) const
{
    return m_pseudostate;
}

Blocks ExecutionSemanticsBlock::blocks( void ) const
{
    return m_blocks;
}

void ExecutionSemanticsBlock::add( const Block::Ptr& block )
{
    m_blocks.add( block );

    // if( isa< ExecutionSemanticsBlock >( block ) )
    // {
    //     ExecutionSemanticsBlock* inner
    //         = static_cast< ExecutionSemanticsBlock* >( block );
    //     inner->setScope( this );
    //     inner->setParent( this );
    // }
    // else if( isa< Statement >( block ) )
    // {
    //     block->setParent( this );
    // }
    // else
    // {
    //     assert( !" invalid block to add " );
    // }
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
: ExecutionSemanticsBlock( "par", true, 0, classid() )
{
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
: ExecutionSemanticsBlock( "seq", false, 0, classid() )
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

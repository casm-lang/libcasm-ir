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

Block::Block( const char* name, Value::ID id )
: Value( name, Type::Label(), id )
, m_parent( 0 )
{
}

void Block::setParent( Value* parent )
{
    assert( parent );
    m_parent = parent;
}

Value* Block::parent( void ) const
{
    return m_parent;
}

u1 Block::classof( Value const* obj )
{
    return obj->id() == classid() or ExecutionSemanticsBlock::classof( obj )
           or Statement::classof( obj );
}

ExecutionSemanticsBlock::ExecutionSemanticsBlock( const char* name,
    const u1 is_parallel, ExecutionSemanticsBlock* scope, Value::ID id )
: Block( name, id )
, m_is_parallel( is_parallel )
, m_pseudo_state( 0 )
, m_scope( scope )
, m_entry( 0 )
, m_exit( 0 )
{
    setScope( scope );

    Statement* tmp = new TrivialStatement( this );
    Instruction* instr = new ForkInstruction();
    tmp->add( instr );
    m_entry = tmp;

    tmp = new TrivialStatement( this );
    tmp->add( new MergeInstruction() );
    m_exit = tmp;
}

ExecutionSemanticsBlock::~ExecutionSemanticsBlock( void )
{
    delete m_entry;
    delete m_exit;
}

Block* ExecutionSemanticsBlock::entryBlock( void ) const
{
    return m_entry;
}

Block* ExecutionSemanticsBlock::exitBlock( void ) const
{
    return m_exit;
}

const u1 ExecutionSemanticsBlock::isParallel( void ) const
{
    return m_is_parallel;
}

const u64 ExecutionSemanticsBlock::pseudostate( void ) const
{
    return m_pseudo_state;
}

ExecutionSemanticsBlock* ExecutionSemanticsBlock::scope( void ) const
{
    return m_scope;
}

void ExecutionSemanticsBlock::setScope( ExecutionSemanticsBlock* scope_block )
{
    m_scope = scope_block;

    if( m_scope )
    {
        m_pseudo_state = m_scope->pseudostate();

        if( m_scope->isParallel() != this->isParallel() )
        {
            m_pseudo_state++;
        }
    }
}

const std::vector< Block* >& ExecutionSemanticsBlock::blocks( void ) const
{
    return m_blocks;
}

void ExecutionSemanticsBlock::add( Block* block )
{
    assert( block );

    if( isa< ExecutionSemanticsBlock >( block ) )
    {
        ExecutionSemanticsBlock* inner
            = static_cast< ExecutionSemanticsBlock* >( block );
        inner->setScope( this );
        inner->setParent( this );
    }
    else if( isa< Statement >( block ) )
    {
        block->setParent( this );
    }
    else
    {
        assert( !" invalid block to add " );
    }

    m_blocks.push_back( block );
}

u1 ExecutionSemanticsBlock::classof( Value const* obj )
{
    return obj->id() == classid() or ParallelBlock::classof( obj )
           or SequentialBlock::classof( obj );
}

ParallelBlock::ParallelBlock( ExecutionSemanticsBlock* scope )
: ExecutionSemanticsBlock( "par", true, scope, classid() )
{
}

u1 ParallelBlock::classof( Value const* obj )
{
    return obj->id() == classid();
}

SequentialBlock::SequentialBlock( ExecutionSemanticsBlock* scope )
: ExecutionSemanticsBlock( "seq", false, scope, classid() )
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

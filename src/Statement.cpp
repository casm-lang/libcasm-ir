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

#include "Statement.h"

#include "Constant.h"
#include "Instruction.h"

using namespace libcasm_ir;

Statement::Statement(
    const char* name, ExecutionSemanticsBlock* scope, Value::ID id )
: Block( name, id )
, m_scope( scope )
{
    if( scope )
    {
        setParent( scope );
    }
}

ExecutionSemanticsBlock* Statement::scope( void ) const
{
    return m_scope;
}

const std::vector< Value* >& Statement::instructions( void ) const
{
    return m_instructions;
}

void Statement::add( Value* instruction )
{
    // printf( "%s: %p\n", __FUNCTION__, instruction );

    if( isa< Constant >( instruction ) )
    {
        // printf( "%s: %p --> Constant, omitted\n", __FUNCTION__, instruction
        // );
        return;
    }

    assert( instruction and isa< Instruction >( instruction ) );

    Instruction* instr = static_cast< Instruction* >( instruction );
    instr->setStatement( this );
    m_instructions.push_back( instr );

    // printf( "[Stmt] add: %p\n", instruction );
}

void Statement::addBlock( ExecutionSemanticsBlock* block )
{
    assert( block );

    if( isa< TrivialStatement >( this ) )
    {
        assert(
            !" trivial statements are not allowed to have inside blocks! " );
    }
    // else if( isa< ForallStatement >( this ) )
    // {
    //     assert( blocks.size() < 1 );
    // }

    m_blocks.push_back( block );

    if( block->parent() == 0 )
    {
        block->setParent( this );
    }

    assert( block->parent() == this && " inconsistent block nesting! " );
}

const std::vector< ExecutionSemanticsBlock* >& Statement::blocks( void ) const
{
    if( isa< TrivialStatement >( this ) )
    {
        assert( !" trivial statements do not contain inside blocks! " );
    }

    return m_blocks;
}

u1 Statement::classof( Value const* obj )
{
    return obj->id() == classid() or TrivialStatement::classof( obj )
           or BranchStatement::classof( obj );
}

TrivialStatement::TrivialStatement( ExecutionSemanticsBlock* scope )
: Statement( "statement", scope, classid() )
{
}

u1 TrivialStatement::classof( Value const* obj )
{
    return obj->id() == classid();
}

BranchStatement::BranchStatement( ExecutionSemanticsBlock* scope )
: Statement( "branch", scope, classid() )
{
}

u1 BranchStatement::classof( Value const* obj )
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

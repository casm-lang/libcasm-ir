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

using namespace libcasm_ir;

Statement::Statement( const std::string& name, Value::ID id )
: Block( name, id )
{
}

void Statement::add( const Instruction::Ptr& instruction )
{
    if( not instruction )
    {
        throw std::domain_error(
            "adding a null pointer instruction is not allowed" );
    }

    if( instruction->statement() )
    {
        // instruction already bound to a statement, just return
        return;
    }

    const auto self = ptr_this< Statement >();

    for( auto operand : instruction->operands() )
    {
        if( isa< Instruction >( operand ) )
        {
            add( std::static_pointer_cast< Instruction >( operand ) );
        }
        else if( isa< ExecutionSemanticsBlock >( operand ) )
        {
            auto esb = std::static_pointer_cast< ExecutionSemanticsBlock >(
                operand );

            esb->setParent( self );
            esb->setScope( self->scope() );
        }
    }

    instruction->setStatement( self );

    if( m_instructions.size() > 0 )
    {
        m_instructions.back()->setNext( instruction );
    }

    m_instructions.add( instruction );
}

Instructions Statement::instructions( void ) const
{
    return m_instructions;
}

ExecutionSemanticsBlock::Ptr Statement::add(
    const ExecutionSemanticsBlock::Ptr& block )
{
    assert( block );

    if( isa< TrivialStatement >( this ) )
    {
        assert(
            !" trivial statements are not allowed to have inside blocks! " );
    }

    const auto self = ptr_this< Statement >();

    block->setParent( self );
    block->setScope( self->scope() );

    m_blocks.add( block );

    return block;
}

ExecutionSemanticsBlocks Statement::blocks( void ) const
{
    if( isa< TrivialStatement >( this ) )
    {
        assert( !" trivial statements do not contain inside blocks! " );
    }

    return m_blocks;
}

void Statement::replaceWith( const Block::Ptr block )
{
    assert( parent() );
    assert( isa< ExecutionSemanticsBlock >( parent() ) );

    const auto blk
        = std::static_pointer_cast< ExecutionSemanticsBlock >( parent() );

    blk->replace( *this, block );
}

u1 Statement::classof( Value const* obj )
{
    return obj->id() == classid() or TrivialStatement::classof( obj )
           or BranchStatement::classof( obj );
}

TrivialStatement::TrivialStatement( void )
: Statement( "statement", classid() )
{
}

void TrivialStatement::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 TrivialStatement::classof( Value const* obj )
{
    return obj->id() == classid();
}

BranchStatement::BranchStatement( void )
: Statement( "branch", classid() )
{
}

void BranchStatement::accept( Visitor& visitor )
{
    visitor.visit( *this );
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

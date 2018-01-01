//
//  Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
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

#include "Statement.h"

#include "Constant.h"

using namespace libcasm_ir;

Statement::Statement( const Value::ID id )
: Block( id )
{
}

void Statement::add( const Instruction::Ptr& instruction )
{
    if( not instruction )
    {
        throw std::domain_error( "adding a null pointer instruction is not allowed" );
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
            auto esb = std::static_pointer_cast< ExecutionSemanticsBlock >( operand );

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

Instructions& Statement::instructions( void )
{
    return m_instructions;
}

ExecutionSemanticsBlock::Ptr Statement::add( const ExecutionSemanticsBlock::Ptr& block )
{
    assert( block );

    if( isa< TrivialStatement >( this ) )
    {
        assert( !" trivial statements are not allowed to have inside blocks! " );
    }

    const auto self = ptr_this< Statement >();

    block->setParent( self );
    block->setScope( self->scope() );

    m_blocks.add( block );

    return block;
}

ExecutionSemanticsBlocks& Statement::blocks( void )
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

    const auto blk = std::static_pointer_cast< ExecutionSemanticsBlock >( parent() );

    blk->replace( *this, block );
}

std::string Statement::name( void ) const
{
    if( isa< TrivialStatement >( this ) )
    {
        return "statement";
    }
    else if( isa< BranchStatement >( this ) )
    {
        return "branch";
    }

    assert( !" invalid statement to dispatch 'name' found! " );
    return "";
}

u1 Statement::classof( Value const* obj )
{
    return obj->id() == classid() or TrivialStatement::classof( obj ) or
           BranchStatement::classof( obj );
}

TrivialStatement::TrivialStatement( void )
: Statement( classid() )
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
: Statement( classid() )
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

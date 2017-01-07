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

/**
   @file     Value.cpp
   @author   Philipp Paulweber
   @date     2015-02-20
*/

#include "Value.h"
#include "Agent.h"
#include "Constant.h"
#include "Derived.h"
#include "Function.h"
#include "Instruction.h"
#include "Rule.h"
#include "Specification.h"
#include "Statement.h"

using namespace libcasm_ir;

Value::Value( const char* name, Type* type, Value::ID id )
: name( name )
, type( type )
, id( id )
, type_lock( false )
{
    getSymbols()[ name ].insert( this );

    // printf( "[Value] created '%s' @ %p", name, this );
    // if( type )
    // {
    //     printf( " of type '%s' (=0x%lx)", type->getName(), type->getID() );
    // }
    // printf( "\n" );
}

Value::~Value()
{
    getSymbols()[ name ].erase( this );
    // printf( "[Value] deleted '%s' @ %p of type %p\n", name, this, type );
}

const char* Value::getName( void ) const
{
    return name;
}

Type* Value::getType( void ) const
{
    return type;
}

u1 Value::hasType( void ) const
{
    return type ? true : false;
}

void Value::setType( Type* type )
{
    assert( !type_lock );
    type_lock = true;

    this->type = type;
}

Value::ID Value::getValueID() const
{
    return id;
}

void Value::debug( void ) const
{
    printf( "%p '%s' : ", this, getName() );
    if( getType() )
    {
        printf( "%s", getType()->getName() );
    }
    printf( "\n" );
}

void Value::dump( void ) const
{
    switch( this->getValueID() )
    {
        case Value::AGENT:
            ( (Rule*)this )->dump();
            break;
        case Value::RULE:
            ( (Rule*)this )->dump();
            break;
        case Value::DERIVED:
            ( (Derived*)this )->dump();
            break;
        case Value::FUNCTION:
            ( (Function*)this )->dump();
            break;
        case Value::BUILTIN:
            ( (Builtin*)this )->dump();
            break;

        case Value::BLOCK:
            ( (Block*)this )->dump();
            break;
        case Value::EXECUTION_SEMANTICS_BLOCK:
            ( (ExecutionSemanticsBlock*)this )->dump();
            break;
        case Value::PARALLEL_BLOCK:
            ( (ParallelBlock*)this )->dump();
            break;
        case Value::SEQUENTIAL_BLOCK:
            ( (SequentialBlock*)this )->dump();
            break;

        case Value::STATEMENT:
            ( (Statement*)this )->dump();
            break;
        case Value::TRIVIAL_STATEMENT:
            ( (TrivialStatement*)this )->dump();
            break;
        case Value::BRANCH_STATEMENT:
            ( (BranchStatement*)this )->dump();
            break;

        case Value::CONSTANT:
            ( (Constant*)this )->dump();
            break;
        case Value::IDENTIFIER:
            ( (Identifier*)this )->dump();
            break;
        case Value::INTEGER_CONSTANT:
            ( (IntegerConstant*)this )->dump();
            break;

        default:
            if( Value::isa< Instruction >( this ) )
            {
                ( (Instruction*)this )->dump();
            }
            else
            {
                debug();
            }
    }
}

void Value::iterate(
    Traversal order, Visitor* visitor, std::function< void( Value* ) > action )
{
    if( order == Traversal::PREORDER )
    {
        action( /*order, */ this );
    }

    if( visitor )
    {
        visitor->dispatch( Visitor::Stage::PROLOG, this );
    }

    if( Value::isa< Specification >( this ) )
    {
        Specification* obj = ( (Specification*)this );
        const std::unordered_map< std::string, Value* > empty = {};

        for( auto p : ( obj->has< Agent >() ? obj->get< Agent >() : empty ) )
        {
            p.second->iterate( order, visitor, action );
        }

        for( auto p :
            ( obj->has< Function >() ? obj->get< Function >() : empty ) )
        {
            p.second->iterate( order, visitor, action );
        }

        for( auto p :
            ( obj->has< Derived >() ? obj->get< Derived >() : empty ) )
        {
            p.second->iterate( order, visitor, action );
        }

        for( auto p :
            ( obj->has< Constant >() ? obj->get< Constant >() : empty ) )
        {
            p.second->iterate( order, visitor, action );
        }

        for( auto p :
            ( obj->has< Builtin >() ? obj->get< Builtin >() : empty ) )
        {
            p.second->iterate( order, visitor, action );
        }

        for( auto p : ( obj->has< Rule >() ? obj->get< Rule >() : empty ) )
        {
            p.second->iterate( order, visitor, action );
        }
    }
    else if( Value::isa< Rule >( this ) )
    {
        Rule* obj = ( (Rule*)this );

        if( visitor )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, this );
        }

        Value* context = obj->getContext();
        assert( context );

        context->iterate( order, visitor, action );
    }
    else if( Value::isa< Derived >( this ) )
    {
        Derived* obj = ( (Derived*)this );

        if( visitor )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, this );
        }
        
        Value* context = obj->getContext();
        assert( context );
        
        context->iterate( order, visitor, action );
    }
    else if( Value::isa< ExecutionSemanticsBlock >( this ) )
    {
        ExecutionSemanticsBlock* val
            = static_cast< ExecutionSemanticsBlock* >( this );

        Block* entry = val->getEntryBlock();
        Block* exit = val->getExitBlock();

        if( entry )
        {
            entry->iterate( order, visitor, action );
        }

        for( Value* block : val->getBlocks() )
        {
            assert( block );
            block->iterate( order, visitor, action );
        }

        if( exit )
        {
            exit->iterate( order, visitor, action );
        }
    }
    else if( Value::isa< Statement >( this ) )
    {
        Statement* stmt = (Statement*)this;
        assert( stmt->getInstructions().size() > 0
                and " a statement must contain at least one instruction " );

        for( Value* instr : stmt->getInstructions() )
        {
            assert( instr );
            instr->iterate( order, visitor, action );
        }

        if( not Value::isa< TrivialStatement >( this ) )
        {
            if( visitor )
            {
                visitor->dispatch( Visitor::Stage::INTERLOG, this );
            }

            for( ExecutionSemanticsBlock* sco : stmt->getBlocks() )
            {
                assert( sco );
                sco->iterate( order, visitor, action );
            }
        }
    }

    if( visitor )
    {
        visitor->dispatch( Visitor::Stage::EPILOG, this );
    }

    if( order == Traversal::POSTORDER )
    {
        action( /*order, */ this );
    }
}

void Value::iterate( Traversal order, std::function< void( Value* ) > action )
{
    iterate( order, 0, action );
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

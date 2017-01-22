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
    id2objs()[ id ].insert( this );
}

Value::~Value()
{
    id2objs()[ id ].erase( this );
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
            if( isa< Instruction >( this ) )
            {
                ( (Instruction*)this )->dump();
            }
            else
            {
                debug();
            }
    }
}

void Value::iterate( Traversal order,
    Visitor* visitor,
    Context* context,
    std::function< void( Value&, Context& ) >
        action )
{
    static Context default_context = Context();

    Context* cxt = context ? context : &default_context;

    Value& value = static_cast< Value& >( *this );

    if( order == Traversal::PREORDER )
    {
        action( /*order, */ value, *cxt );
    }

    if( visitor )
    {
        visitor->dispatch( Visitor::Stage::PROLOG, value, *cxt );
    }

    if( isa< Specification >( value ) )
    {
        Specification& obj = static_cast< Specification& >( value );

        const std::unordered_map< std::string, Value* > empty = {};

        for( auto p :
            ( obj.has< Constant >() ? obj.get< Constant >() : empty ) )
        {
            p.second->iterate( order, visitor, cxt, action );
        }

        for( auto p : ( obj.has< Builtin >() ? obj.get< Builtin >() : empty ) )
        {
            p.second->iterate( order, visitor, cxt, action );
        }

        for( auto p :
            ( obj.has< Function >() ? obj.get< Function >() : empty ) )
        {
            p.second->iterate( order, visitor, cxt, action );
        }

        for( auto p : ( obj.has< Derived >() ? obj.get< Derived >() : empty ) )
        {
            p.second->iterate( order, visitor, cxt, action );
        }

        for( auto p : ( obj.has< Rule >() ? obj.get< Rule >() : empty ) )
        {
            p.second->iterate( order, visitor, cxt, action );
        }

        for( auto p : ( obj.has< Agent >() ? obj.get< Agent >() : empty ) )
        {
            p.second->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< Rule >( value ) )
    {
        Rule& obj = static_cast< Rule& >( value );

        if( visitor )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );
        }

        Value* context = obj.getContext();
        assert( context );

        context->iterate( order, visitor, cxt, action );
    }
    else if( isa< Derived >( value ) )
    {
        Derived& obj = static_cast< Derived& >( value );

        if( visitor )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );
        }

        Value* context = obj.getContext();
        assert( context );

        context->iterate( order, visitor, cxt, action );
    }
    else if( isa< ExecutionSemanticsBlock >( value ) )
    {
        ExecutionSemanticsBlock& obj
            = static_cast< ExecutionSemanticsBlock& >( value );

        Block* entry = obj.getEntryBlock();
        Block* exit = obj.getExitBlock();

        if( entry )
        {
            entry->iterate( order, visitor, cxt, action );
        }

        for( Value* block : obj.getBlocks() )
        {
            assert( block );
            block->iterate( order, visitor, cxt, action );
        }

        if( exit )
        {
            exit->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< Statement >( value ) )
    {
        Statement& obj = static_cast< Statement& >( value );

        assert( obj.getInstructions().size() > 0
                and " a statement must contain at least one instruction " );

        for( Value* instr : obj.getInstructions() )
        {
            assert( instr );
            instr->iterate( order, visitor, cxt, action );
        }

        if( not isa< TrivialStatement >( value ) )
        {
            if( visitor )
            {
                visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );
            }

            for( ExecutionSemanticsBlock* sco : obj.getBlocks() )
            {
                assert( sco );
                sco->iterate( order, visitor, cxt, action );
            }
        }
    }

    if( visitor )
    {
        visitor->dispatch( Visitor::Stage::EPILOG, value, *cxt );
    }

    if( order == Traversal::POSTORDER )
    {
        action( /*order, */ value, *cxt );
    }
}

void Value::iterate(
    Traversal order, std::function< void( Value&, Context& ) > action )
{
    iterate( order, nullptr, nullptr, action );
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

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

#include "Value.h"

#include "Agent.h"
#include "Builtin.h"
#include "Constant.h"
#include "Derived.h"
#include "Function.h"
#include "Instruction.h"
#include "Rule.h"
#include "Specification.h"
#include "Statement.h"

#include "../stdhl/cpp/Default.h"
#include "../stdhl/cpp/Log.h"

using namespace libcasm_ir;

Value::Value( const std::string& name, const Type::Ptr& type, Value::ID id )
: m_name( name )
, m_type( type )
, m_id( id )
{
    m_id2objs()[ m_id ].insert( this );
}

Value::~Value()
{
    m_id2objs()[ m_id ].erase( this );
}

const char* Value::name( void ) const
{
    return m_name.c_str();
}

std::string Value::str_name( void ) const
{
    return m_name;
}

const Type& Value::type( void ) const
{
    return *m_type.get();
}

Type::Ptr Value::ptr_type( void ) const
{
    return m_type;
}

Value::ID Value::id( void ) const
{
    return m_id;
}

const char* Value::description( void ) const
{
    return str_description().c_str();
}

std::string Value::str_description( void ) const
{
    return type().str_name() + " " + str_name();
}

void Value::dump( void ) const
{
    std::string tmp = "";

    tmp += "[";
    tmp += type().name();
    tmp += "]";

    tmp += label();
    tmp += " = ";

    if( isa< Constant >( this ) )
    {
        tmp += type().name();
        tmp += " ";
    }
    tmp += name();

    if( auto instr = cast< Instruction >( this ) )
    {
        u1 first = true;
        for( auto operand : instr->operands() )
        {
            if( first )
            {
                first = false;
                tmp += " ";
            }
            else
            {
                tmp += ", ";
            }
            tmp += operand->type().name();
            tmp += " ";
            tmp += operand->label();
        }
    }

    libstdhl::Log::info( "%p: %s", tmp.c_str() );
}

std::string Value::make_hash( void ) const
{
    return "v:" + std::to_string( id() ) + ":" + str_description();
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

        const std::unordered_map< std::string, std::weak_ptr< Value > > empty
            = {};

        for( auto p :
            ( obj.has< Constant >() ? obj.get< Constant >() : empty ) )
        {
            p.second.lock()->iterate( order, visitor, cxt, action );
        }

        for( auto p : ( obj.has< Builtin >() ? obj.get< Builtin >() : empty ) )
        {
            p.second.lock()->iterate( order, visitor, cxt, action );
        }

        for( auto p :
            ( obj.has< Function >() ? obj.get< Function >() : empty ) )
        {
            p.second.lock()->iterate( order, visitor, cxt, action );
        }

        for( auto p : ( obj.has< Derived >() ? obj.get< Derived >() : empty ) )
        {
            p.second.lock()->iterate( order, visitor, cxt, action );
        }

        for( auto p : ( obj.has< Rule >() ? obj.get< Rule >() : empty ) )
        {
            p.second.lock()->iterate( order, visitor, cxt, action );
        }

        for( auto p : ( obj.has< Agent >() ? obj.get< Agent >() : empty ) )
        {
            p.second.lock()->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< Rule >( value ) )
    {
        Rule& obj = static_cast< Rule& >( value );

        if( visitor )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );
        }

        auto context = obj.context();
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

        auto context = obj.context();
        assert( context );

        context->iterate( order, visitor, cxt, action );
    }
    else if( isa< ExecutionSemanticsBlock >( value ) )
    {
        ExecutionSemanticsBlock& obj
            = static_cast< ExecutionSemanticsBlock& >( value );

        for( auto block : obj.blocks() )
        {
            assert( block );
            block->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< Statement >( value ) )
    {
        Statement& obj = static_cast< Statement& >( value );

        assert( obj.instructions().size() > 0
                and " a statement must contain at least one instruction " );

        for( auto instr : obj.instructions() )
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

            for( auto sco : obj.blocks() )
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

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
#include "Block.h"
#include "Builtin.h"
#include "Constant.h"
#include "Derived.h"
#include "Function.h"
#include "Instruction.h"
#include "Rule.h"
#include "Specification.h"
#include "Statement.h"
#include "Value.h"
#include "Visitor.h"

using namespace libcasm_ir;

Value::Value( const std::string& name, const Type::Ptr& type, Value::ID id )
: m_name( name )
, m_type( type )
, m_id( id )
{
    assert( type );
    m_id2objs()[ m_id ].insert( this );
}

Value::~Value()
{
    m_id2objs()[ m_id ].erase( this );
}

std::string Value::name( void ) const
{
    return m_name;
}

std::string Value::description( void ) const
{
    return type().name() + " " + name();
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

std::string Value::dump( void ) const
{
    std::string tmp = "[" + type().name() + "] ";

    if( not type().isVoid() )
    {
        tmp += label() + " = ";
    }

    if( isa< Constant >( this ) or isa< Builtin >( this )
        or isa< Function >( this ) )
    {
        tmp += type().name() + " ";
    }

    if( not isa< Function >( this ) )
    {
        tmp += name();
    }

    if( auto instr = cast< Instruction >( this ) )
    {
        if( isa< ForkInstruction >( this ) or isa< MergeInstruction >( this ) )
        {
            tmp += " " + instr->statement()->scope()->name();
        }

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

            tmp += operand->type().name() + " " + operand->label();
        }
    }

    return tmp;
}

std::string Value::make_hash( void ) const
{
    return "v:" + std::to_string( id() ) + ":" + description();
}

std::string Value::label( void ) const
{
    static std::unordered_map< u8, u64 > cnt;
    static std::unordered_map< const Value*, std::string > lbl;

    auto result = lbl.find( this );
    if( result != lbl.end() )
    {
        return result->second;
    }

    if( isa< Instruction >( this ) )
    {
        auto result = cnt.find( INSTRUCTION );
        if( result == cnt.end() )
        {
            cnt[ INSTRUCTION ] = 0;
        }

        if( this->type().result().isVoid() )
        {
            return name();
        }

        return lbl
            .emplace( this, "%r" + std::to_string( cnt[ INSTRUCTION ]++ ) )
            .first->second;
    }
    else if( isa< Block >( this ) )
    {
        auto result = cnt.find( BLOCK );
        if( result == cnt.end() )
        {
            cnt[ BLOCK ] = 0;
        }

        return lbl.emplace( this, "%lbl" + std::to_string( cnt[ BLOCK ]++ ) )
            .first->second;
    }
    else if( isa< Constant >( this ) )
    {
        auto result = cnt.find( CONSTANT );
        if( result == cnt.end() )
        {
            cnt[ CONSTANT ] = 0;
        }

        return lbl.emplace( this, "@c" + std::to_string( cnt[ CONSTANT ]++ ) )
            .first->second;
    }
    else if( isa< Builtin >( this ) )
    {
        auto result = cnt.find( BUILTIN );
        if( result == cnt.end() )
        {
            cnt[ BUILTIN ] = 0;
        }

        return lbl.emplace( this, "@b" + std::to_string( cnt[ BUILTIN ]++ ) )
            .first->second;
    }
    else
    {
        return name();
    }
}

void Value::iterate(
    const Traversal order, std::function< void( Value& ) > action )
{
    TraversalVisitor visitor( order, action );

    accept( visitor );
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

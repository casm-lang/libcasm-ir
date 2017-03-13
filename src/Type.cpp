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

#include "Type.h"

#include "Agent.h"
#include "Enumeration.h"

using namespace libcasm_ir;

Type::Type(
    const std::string& name, const std::string& description, Type::ID id )
: m_name( name )
, m_description( description )
, m_id( id )
{
}

std::string Type::name( void ) const
{
    return m_name;
}

std::string Type::description() const
{
    return m_description;
}

Type::ID Type::id( void ) const
{
    return m_id;
}

const Type& Type::result( void ) const
{
    if( isRelation() )
    {
        return *m_result.get();
    }

    return *this;
}

Type::Ptr Type::ptr_result( void )
{
    if( isRelation() )
    {
        return m_result;
    }

    return ptr_this< Type >();
}

Types Type::arguments( void ) const
{
    return m_arguments;
}

std::string Type::make_hash( void ) const
{
    return "t:" + std::to_string( id() ) + ":" + description();
}

u1 Type::isVoid( void ) const
{
    return id() == Type::VOID;
}

u1 Type::isLabel( void ) const
{
    return id() == Type::LABEL;
}

u1 Type::isLocation( void ) const
{
    return id() == Type::LOCATION;
}

u1 Type::isAgent( void ) const
{
    return id() == Type::AGENT;
}

u1 Type::isRuleReference( void ) const
{
    return id() == Type::RULE_REFERENCE;
}

u1 Type::isBoolean( void ) const
{
    return id() == Type::BOOLEAN;
}

u1 Type::isInteger( void ) const
{
    return id() == Type::INTEGER;
}

u1 Type::isBit( void ) const
{
    return id() == Type::BIT;
}

u1 Type::isString( void ) const
{
    return id() == Type::STRING;
}

u1 Type::isFloating( void ) const
{
    return id() == Type::FLOATING;
}

u1 Type::isRational( void ) const
{
    return id() == Type::RATIONAL;
}

u1 Type::isEnumeration( void ) const
{
    return id() == Type::ENUMERATION;
}

u1 Type::isRelation( void ) const
{
    return id() == Type::RELATION;
}

//
//
// Primitive Type
//

PrimitiveType::PrimitiveType(
    const std::string& name, const std::string& description, Type::ID id )
: Type( name, description, id )
{
}

//
// Void Type
//

VoidType::VoidType()
: PrimitiveType( "v", "Void", Type::VOID )
{
}

//
// Label Type
//

LabelType::LabelType()
: PrimitiveType( "label", "Label", Type::LABEL )
{
}

//
// Location Type
//

LocationType::LocationType()
: PrimitiveType( "l", "Location", Type::LOCATION )
{
}

//
// Rule Reference Type
//

RuleReferenceType::RuleReferenceType()
: PrimitiveType( "r", "RuleRef", Type::RULE_REFERENCE )
{
}

//
// Boolean Type
//

BooleanType::BooleanType()
: PrimitiveType( "b", "Boolean", Type::BOOLEAN )
{
}

//
// Integer Type
//

IntegerType::IntegerType()
: PrimitiveType( "i", "Integer", Type::INTEGER )
{
}

//
// Bit Type
//

BitType::BitType( u16 bitsize )
: PrimitiveType( "u" + std::to_string( bitsize ),
      "Bit(" + std::to_string( bitsize ) + ")", Type::BIT )
, m_bitsize( bitsize )
{
}

u16 BitType::bitsize( void ) const
{
    return m_bitsize;
}

//
// String Type
//

StringType::StringType()
: PrimitiveType( "s", "String", Type::STRING )
{
}

//
// Flaoting Type
//

FloatingType::FloatingType()
: PrimitiveType( "f", "Floating", Type::FLOATING )
{
}

//
// Rational Type
//

RationalType::RationalType()
: PrimitiveType( "q", "Rational", Type::RATIONAL )
{
}

//
// Enumeration Type
//

EnumerationType::EnumerationType( const Enumeration::Ptr& kind, Type::ID id )
: PrimitiveType( kind->name(), kind->name(), id )
, m_kind( kind )
{
}

EnumerationType::EnumerationType( const Enumeration::Ptr& kind )
: EnumerationType( kind, Type::ENUMERATION )
{
}

Enumeration& EnumerationType::kind( void ) const
{
    return *m_kind.get();
}

Enumeration::Ptr EnumerationType::ptr_kind( void ) const
{
    return m_kind;
}

//
// Agent Type
//

AgentType::AgentType( const Agent::Ptr& agent )
: EnumerationType( agent, Type::AGENT )
{
}

//
//
// Relation Type
//

RelationType::RelationType( const Type::Ptr& result, const Types& arguments )
: Type( "", "", Type::RELATION )
{
    m_result = result;
    m_arguments = arguments;

    m_name = "(";
    m_description = "(";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        if( not first )
        {
            m_name += ", ";
            m_description += " x ";
        }

        m_name += argument->name();
        m_description += argument->description();

        first = false;
    }

    m_name += " -> " + m_result->name() + ")";
    m_description += " -> " + m_result->description() + ")";
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

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

#include "Enumeration.h"

using namespace libcasm_ir;

Type::Type( Type::ID id )
: m_id( id )
{
}

std::string Type::name( void ) const
{
    switch( m_id )
    {
        case _BOTTOM_:
        {
            return "_BOTTOM_";
        }
        case VOID:
        {
            return "v";
        }
        case LABEL:
        {
            return "label";
        }
        case LOCATION:
        {
            return "l";
        }
        case RULE_REFERENCE:
        {
            return "r";
        }
        case BOOLEAN:
        {
            return "b";
        }
        case INTEGER:
        {
            return "i";
        }
        case BIT:
        {
            return static_cast< const BitType* >( this )->name();
        }
        case STRING:
        {
            return "s";
        }
        case FLOATING:
        {
            return "f";
        }
        case RATIONAL:
        {
            return "q";
        }
        case ENUMERATION:
        {
            return static_cast< const EnumerationType* >( this )->name();
        }
        case RELATION:
        {
            return static_cast< const RelationType* >( this )->name();
        }
        case _TOP_:
        {
            return "";
        }
    }
}

std::string Type::description( void ) const
{
    switch( m_id )
    {
        case _BOTTOM_:
        {
            return "_BOTTOM_";
        }
        case VOID:
        {
            return "Void";
        }
        case LABEL:
        {
            return "Label";
        }
        case LOCATION:
        {
            return "Location";
        }
        case RULE_REFERENCE:
        {
            return "RuleRef";
        }
        case BOOLEAN:
        {
            return "Boolean";
        }
        case INTEGER:
        {
            return "Integer";
        }
        case BIT:
        {
            return static_cast< const BitType* >( this )->description();
        }
        case STRING:
        {
            return "String";
        }
        case FLOATING:
        {
            return "Floating";
        }
        case RATIONAL:
        {
            return "Rational";
        }
        case ENUMERATION:
        {
            return static_cast< const EnumerationType* >( this )->description();
        }
        case RELATION:
        {
            return static_cast< const RelationType* >( this )->description();
        }
        case _TOP_:
        {
            return "";
        }
    }
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

PrimitiveType::PrimitiveType( Type::ID id )
: Type( id )
{
}

//
// Void Type
//

VoidType::VoidType()
: PrimitiveType( Type::VOID )
{
}

//
// Label Type
//

LabelType::LabelType()
: PrimitiveType( Type::LABEL )
{
}

//
// Location Type
//

LocationType::LocationType()
: PrimitiveType( Type::LOCATION )
{
}

//
// Rule Reference Type
//

RuleReferenceType::RuleReferenceType()
: PrimitiveType( Type::RULE_REFERENCE )
{
}

//
// Boolean Type
//

BooleanType::BooleanType()
: PrimitiveType( Type::BOOLEAN )
{
}

//
// Integer Type
//

IntegerType::IntegerType()
: PrimitiveType( Type::INTEGER )
{
}

//
// Bit Type
//

BitType::BitType( u16 bitsize )
: PrimitiveType( Type::BIT )
, m_bitsize( bitsize )
{
}

u16 BitType::bitsize( void ) const
{
    return m_bitsize;
}

std::string BitType::name( void ) const
{
    return "u" + std::to_string( m_bitsize );
}

std::string BitType::description( void ) const
{
    return "Bit'" + std::to_string( m_bitsize );
}

//
// String Type
//

StringType::StringType()
: PrimitiveType( Type::STRING )
{
}

//
// Flaoting Type
//

FloatingType::FloatingType()
: PrimitiveType( Type::FLOATING )
{
}

//
// Rational Type
//

RationalType::RationalType()
: PrimitiveType( Type::RATIONAL )
{
}

//
// Enumeration Type
//

EnumerationType::EnumerationType( const Enumeration::Ptr& kind )
: PrimitiveType( Type::ENUMERATION )
, m_kind( kind )
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

std::string EnumerationType::name( void ) const
{
    return m_kind->name();
}

std::string EnumerationType::description( void ) const
{
    return m_kind->name();
}

//
//
// Relation Type
//

RelationType::RelationType( const Type::Ptr& result, const Types& arguments )
: Type( Type::RELATION )
{
    m_result = result;
    m_arguments = arguments;
}

std::string RelationType::name( void ) const
{
    std::string tmp = "(";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        if( not first )
        {
            tmp += ", ";
        }

        tmp += argument->name();

        first = false;
    }

    tmp += " -> " + m_result->name() + ")";

    return tmp;
}

std::string RelationType::description( void ) const
{
    std::string tmp = "(";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        if( not first )
        {
            tmp += " x ";
        }

        tmp += argument->description();

        first = false;
    }

    tmp += " -> " + m_result->description() + ")";

    return tmp;
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

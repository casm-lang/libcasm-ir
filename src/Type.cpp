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

#include "Constant.h"
#include "Enumeration.h"

using namespace libcasm_ir;

Type::Type( Type::ID id )
: m_id( id )
{
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

u1 Type::isRange( void ) const
{
    return id() == Type::RANGE;
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

std::string Type::token( const Type::ID id )
{
    switch( id )
    {
        case _BOTTOM_:
        {
            return "_BOTTOM_";
        }
        case RANGE:
        {
            return "Range";
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
            return "Bit";
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
            return "Enumeration";
        }
        case RELATION:
        {
            return "Relation";
        }
        case _TOP_:
        {
            return "_TOP_";
        }
    }

    assert( !" internal error " );
    return "";
}

//
//
// Range Type
//

RangeType::RangeType(
    const IntegerConstant::Ptr& from, const IntegerConstant::Ptr& to )
: Type( Type::RANGE )
, m_from( from )
, m_to( to )
, m_increasing( from->value() <= to->value() )
{
}

u1 RangeType::increasing( void ) const
{
    return m_increasing;
}

std::string RangeType::name( void ) const
{
    return "[" + m_from->name() + ".." + m_to->name() + "]";
}

std::string RangeType::description( void ) const
{
    return this->name();
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

std::string VoidType::name( void ) const
{
    return "v";
}

std::string VoidType::description( void ) const
{
    return token( id() );
}

//
// Label Type
//

LabelType::LabelType()
: PrimitiveType( Type::LABEL )
{
}

std::string LabelType::name( void ) const
{
    return "label";
}

std::string LabelType::description( void ) const
{
    return token( id() );
}

//
// Location Type
//

LocationType::LocationType()
: PrimitiveType( Type::LOCATION )
{
}

std::string LocationType::name( void ) const
{
    return "location";
}

std::string LocationType::description( void ) const
{
    return token( id() );
}

//
// Rule Reference Type
//

RuleReferenceType::RuleReferenceType()
: PrimitiveType( Type::RULE_REFERENCE )
{
}

std::string RuleReferenceType::name( void ) const
{
    return "r";
}

std::string RuleReferenceType::description( void ) const
{
    return token( id() ); // + TODO: PPA: add concrete type relation of ref
}

//
// Boolean Type
//

BooleanType::BooleanType()
: PrimitiveType( Type::BOOLEAN )
{
}

std::string BooleanType::name( void ) const
{
    return "r";
}

std::string BooleanType::description( void ) const
{
    return token( id() );
}

//
// Integer Type
//

IntegerType::IntegerType()
: PrimitiveType( Type::INTEGER )
, m_range( nullptr )
{
}

IntegerType::IntegerType( const RangeType::Ptr& range )
: PrimitiveType( Type::INTEGER )
, m_range( range )
{
    assert( range );

    if( not range->increasing() )
    {
        throw std::domain_error(
            "range '" + range->name() + "' violates monotonically nondecreasing property of 'Integer' type" );
    }
}

u1 IntegerType::constrained( void ) const
{
    return m_range ? true : false;
}

RangeType::Ptr IntegerType::range( void ) const
{
    return m_range;
}

std::string IntegerType::name( void ) const
{
    return "i" + ( m_range ? m_range->name() : "" );
}

std::string IntegerType::description( void ) const
{
    if( not m_range )
    {
        return token( id() );
    }
    else
    {
        return token( id() ) + "'" + m_range->name();
    }
}

//
// Bit Type
//

BitType::BitType( u16 bitsize )
: PrimitiveType( Type::BIT )
, m_bitsize( bitsize )
{
    if( m_bitsize < 1 or m_bitsize > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
}

BitType::BitType( const IntegerConstant::Ptr& bitsize )
: PrimitiveType( Type::BIT )
{
    assert( bitsize );
    if( bitsize->value() > BitType::SizeMax )
    {
        throw std::domain_error(
            "invalid bit size '" + bitsize->name() + "' for 'Bit' type" );
    }

    m_bitsize = bitsize->value_i64();

    if( m_bitsize < 1 )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
}

BitType::BitType( const std::string& value, const libstdhl::Type::Radix radix )
: PrimitiveType( Type::BIT )
{
    std::string tmp = value;
    tmp.erase( std::remove( tmp.begin(), tmp.end(), '\'' ), tmp.end() );

    m_bitsize = (u16)tmp.size() * std::log2( (double)radix );

    if( m_bitsize < 1 or m_bitsize > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
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
    return token( id() ) + "'" + std::to_string( m_bitsize );
}

//
// String Type
//

StringType::StringType()
: PrimitiveType( Type::STRING )
{
}

std::string StringType::name( void ) const
{
    return "s";
}

std::string StringType::description( void ) const
{
    return token( id() );
}

//
// Flaoting Type
//

FloatingType::FloatingType()
: PrimitiveType( Type::FLOATING )
{
}

std::string FloatingType::name( void ) const
{
    return "f";
}

std::string FloatingType::description( void ) const
{
    return token( id() );
}

//
// Rational Type
//

RationalType::RationalType()
: PrimitiveType( Type::RATIONAL )
{
}

std::string RationalType::name( void ) const
{
    return "r";
}

std::string RationalType::description( void ) const
{
    return token( id() );
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

#ifndef NDEBUG
    assert( result );
    for( auto argument : arguments )
    {
        assert( argument );
    }
#endif
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
            tmp += " * ";
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

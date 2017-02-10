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

#include "../stdhl/cpp/Allocator.h"

using namespace libcasm_ir;

Type::Type( const char* name, const char* description, Type::ID id )
: m_hash( 0 )
, m_name( name )
, m_description( description )
, m_id( id )
{
}

const Type::ID Type::id( void ) const
{
    return m_id;
}

u1 Type::isVoid( void ) const
{
    return id() == Type::VOID;
}
u1 Type::isLabel( void ) const
{
    return id() == Type::LABEL;
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

Type* Type::result( void ) const
{
    if( id() == Type::RELATION )
    {
        const RelationType* rt = static_cast< const RelationType* >( this );
        return (Type*)rt->result();
    }
    return (Type*)this;
}

Type* Type::Void( void )
{
    static VoidType cache = VoidType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Label( void )
{
    static LabelType cache = LabelType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Agent( void )
{
    static AgentType cache = AgentType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::RuleReference( void )
{
    static RuleReferenceType cache = RuleReferenceType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Boolean( void )
{
    static BooleanType cache = BooleanType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Integer( void )
{
    static IntegerType cache = IntegerType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Bit( u16 size )
{
    BitType tmp( size );

    auto cache = m_str2obj().find( tmp.name() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new BitType( tmp );
    m_str2obj()[ tmp.name() ] = ptr;
    return ptr;
}

Type* Type::String( void )
{
    static StringType cache = StringType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Floating( void )
{
    static FloatingType cache = FloatingType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Rational( void )
{
    static RationalType cache = RationalType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Enumeration( const char* name )
{
    assert( !" TODO " );
    return 0;
}

Type* Type::Relation( Type* result, std::vector< Type* > arguments )
{
    RelationType tmp( result, arguments );

    auto cache = m_str2obj().find( tmp.name() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new RelationType( tmp );
    m_str2obj()[ tmp.name() ] = ptr;
    return ptr;
}

const char* Type::make_hash( void )
{
    if( not m_hash )
    {
        std::string tmp;
        tmp += "t:";
        tmp += std::to_string( id() );
        tmp += ":";
        tmp += name();

        m_hash = libstdhl::Allocator::string( tmp );
    }

    return m_hash;
}

PrimitiveType::PrimitiveType(
    const char* name, const char* description, Type::ID id )
: Type( name, description, id )
{
    this->name();
}

const char* PrimitiveType::name( void )
{
    return m_name;
}

const char* PrimitiveType::description( void )
{
    return m_description;
}

const std::vector< Type* >& PrimitiveType::arguments( void )
{
    static std::vector< Type* > empty = {};
    return empty;
}

VoidType::VoidType()
: PrimitiveType( "v", "Void", Type::LABEL )
{
}

LabelType::LabelType()
: PrimitiveType( "label", "Label", Type::LABEL )
{
}

AgentType::AgentType()
: PrimitiveType( "a", "Agent", Type::AGENT )
{
}

RuleReferenceType::RuleReferenceType()
: PrimitiveType( "r", "RuleRef", Type::RULE_REFERENCE )
{
}

BooleanType::BooleanType()
: PrimitiveType( "b", "Boolean", Type::BOOLEAN )
{
}

IntegerType::IntegerType()
: PrimitiveType( "i", "Integer", Type::INTEGER )
{
}

BitType::BitType( u16 size )
: PrimitiveType( libstdhl::Allocator::string( "u" + std::to_string( size ) ),
      libstdhl::Allocator::string( "Bit(" + std::to_string( size ) + ")" ),
      Type::BIT )
, m_size( size )
{
}

const u16 BitType::bitsize( void ) const
{
    return m_size;
}

StringType::StringType()
: PrimitiveType( "s", "String", Type::STRING )
{
}

FloatingType::FloatingType()
: PrimitiveType( "f", "Floating", Type::FLOATING )
{
}

RationalType::RationalType()
: PrimitiveType( "q", "Rational", Type::RATIONAL )
{
}

EnumerationType::EnumerationType( const char* name )
: PrimitiveType( name, name, Type::ENUMERATION )
{
}

RelationType::RelationType( Type* result, std::vector< Type* > arguments )
: Type( 0, 0, Type::RELATION )
, m_result( result )
, m_arguments( arguments )
{
    this->name();
    assert( result );
}

const char* RelationType::name( void )
{
    if( not m_name )
    {
        u1 first = true;
        std::string tmp = "(";
        for( auto argument : m_arguments )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += argument->name();
            first = false;
        }

        tmp += " -> ";
        tmp += m_result->name();
        tmp += ")";

        m_name = libstdhl::Allocator::string( tmp );
    }

    return m_name;
}

const char* RelationType::description( void )
{
    if( not m_description )
    {
        u1 first = true;
        std::string tmp = "(";
        for( auto argument : m_arguments )
        {
            if( not first )
            {
                tmp += " x ";
            }
            tmp += argument->description();
            first = false;
        }

        tmp += " -> ";
        tmp += m_result->description();
        tmp += ")";

        m_description = libstdhl::Allocator::string( tmp );
    }

    return m_description;
}

const std::vector< Type* >& RelationType::arguments( void )
{
    return m_arguments;
}

const Type* RelationType::result( void ) const
{
    return m_result;
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

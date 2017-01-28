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
: name( name )
, description( description )
, id( id )
{
}

const Type::ID Type::getID( void ) const
{
    return id;
}

u1 Type::isLabel( void ) const
{
    return getID() == Type::LABEL;
}

u1 Type::isAgent( void ) const
{
    return getID() == Type::AGENT;
}
u1 Type::isRuleReference( void ) const
{
    return getID() == Type::RULE_REFERENCE;
}
u1 Type::isBoolean( void ) const
{
    return getID() == Type::BOOLEAN;
}
u1 Type::isInteger( void ) const
{
    return getID() == Type::INTEGER;
}
u1 Type::isBit( void ) const
{
    return getID() == Type::BIT;
}
u1 Type::isString( void ) const
{
    return getID() == Type::STRING;
}
u1 Type::isFloating( void ) const
{
    return getID() == Type::FLOATING;
}
u1 Type::isRational( void ) const
{
    return getID() == Type::RATIONAL;
}
u1 Type::isEnumeration( void ) const
{
    return getID() == Type::ENUMERATION;
}
u1 Type::isRelation( void ) const
{
    return getID() == Type::RELATION;
}

Type* Type::getResult( void ) const
{
    if( getID() == Type::RELATION )
    {
        const RelationType* rt = static_cast< const RelationType* >( this );
        return (Type*)rt->getResult();
    }
    return (Type*)this;
}

Type* Type::getLabel( void )
{
    static LabelType cache = LabelType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getAgent( void )
{
    static AgentType cache = AgentType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getRuleReference( void )
{
    static RuleReferenceType cache = RuleReferenceType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getBoolean( void )
{
    static BooleanType cache = BooleanType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getInteger( void )
{
    static IntegerType cache = IntegerType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getBit( u16 size )
{
    BitType tmp( size );

    auto cache = str2obj().find( tmp.getName() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new BitType( tmp );
    str2obj()[ tmp.getName() ] = ptr;
    return ptr;
}

Type* Type::getString( void )
{
    static StringType cache = StringType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getFloating( void )
{
    static FloatingType cache = FloatingType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getRational( void )
{
    static RationalType cache = RationalType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getEnumeration( const char* name )
{
    assert( !" TODO " );
    return 0;
}

Type* Type::getRelation( Type* result, std::vector< Type* > arguments )
{
    RelationType tmp( result, arguments );

    auto cache = str2obj().find( tmp.getName() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new RelationType( tmp );
    str2obj()[ tmp.getName() ] = ptr;
    return ptr;
}

PrimitiveType::PrimitiveType(
    const char* name, const char* description, Type::ID id )
: Type( name, description, id )
{
}

const char* PrimitiveType::getName( void )
{
    return name;
}

const char* PrimitiveType::getDescription( void )
{
    return description;
}

const std::vector< Type* >& PrimitiveType::getArguments( void )
{
    static std::vector< Type* > empty = {};
    return empty;
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
, size( size )
{
}

const u16 BitType::getSize( void ) const
{
    return size;
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
, result( result )
, arguments( arguments )
{
    assert( result );
}

const char* RelationType::getName( void )
{
    if( not name )
    {
        u1 first = true;
        std::string tmp = "(";
        for( auto argument : arguments )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += argument->getName();
            first = false;
        }

        tmp += " -> ";
        tmp += result->getName();
        tmp += ")";

        name = libstdhl::Allocator::string( tmp );
    }

    return name;
}

const char* RelationType::getDescription( void )
{
    if( not description )
    {
        u1 first = true;
        std::string tmp = "(";
        for( auto argument : arguments )
        {
            if( not first )
            {
                tmp += " x ";
            }
            tmp += argument->getDescription();
            first = false;
        }

        tmp += " -> ";
        tmp += result->getDescription();
        tmp += ")";

        description = libstdhl::Allocator::string( tmp );
    }

    return description;
}

const std::vector< Type* >& RelationType::getArguments( void )
{
    return arguments;
}

const Type* RelationType::getResult( void ) const
{
    return result;
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

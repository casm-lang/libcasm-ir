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

Type* Type::getBit( u8 size )
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

BitType::BitType( u8 size )
: PrimitiveType( libstdhl::Allocator::string( "u" + std::to_string( size ) ),
      libstdhl::Allocator::string( "Bit(" + std::to_string( size ) + ")" ),
      Type::BIT )
, size( size )
{
}

const u8 BitType::getSize( void ) const
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

const Type* RelationType::getResult( void ) const
{
    return result;
}

const std::vector< Type* >& RelationType::getArguments( void ) const
{
    return arguments;
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

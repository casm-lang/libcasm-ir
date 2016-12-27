//
//  Copyright (c) 2015-2016 CASM Organization
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

using namespace libcasm_ir;

// const char* Type::ID2str[ Type::ID::_TOP_ ] = {
//     "Agent" // AGENT = 0
//     ,
//     "RuleRef" // RULEPOINTER
//     ,
//     "Boolean" // BOOLEAN
//     ,
//     "Integer" // INTEGER
//     ,
//     "Bit" // BIT
//     ,
//     "String" // STRING
//     ,
//     "Floating" // FLOATING
//     ,
//     "Rational" // RATIONAL
//     ,
//     "<Enumeration>" // ENUMERATION // TODO: PPA: fix this to support all
//                     // variations of types!
// };

Type::Type( const char* name, Type::ID id ) //, i16 bitsize, Type::STATE state )
    //: Value( ".type", self, Value::TYPE );
    : name( name ),
      id( id )
// type_uid_hash( 0 ),
// type_state( Type::STATE::CHANGED ),
// bitsize( bitsize )
{
    // getName();
    // type_state = state;

    // assert( id2str().find( name ) == id2str().end() );
    // id2str()[ name ] = this;
}

const Type::ID Type::getID( void ) const
{
    return id;
}

// const u64 Type::getID( void ) const
// {
//     return id; // type_uid_hash;
// }

// const char* Type::getName( void )
// {
//     return name;

// if( type_state != Type::STATE::CHANGED )
// {
//     return description.c_str();
// }

// description.clear();
// if( parameters.size() > 0 )
// {
//     u1 flag = 0;
//     for( auto parameter : parameters )
//     {
//         if( flag )
//         {
//             description.append( " x " );
//         }
//         else
//         {
//             flag = 1;
//         }
//         description.append( parameter->getName() );
//     }
//     description.append( " -> " );
// }

// description.append( ID2str[ type_id ] );
// if( type_id == Type::BIT )
// {
//     description.append( "(" );
//     description.append( std::to_string( bitsize ) );
//     description.append( ")" );
// }

// if( subtypes.size() > 0 )
// {
//     u1 flag = 0;
//     description.append( "( " );
//     for( auto subtype : subtypes )
//     {
//         if( flag )
//         {
//             description.append( ", " );
//         }
//         else
//         {
//             flag = 1;
//         }
//         description.append( subtype->getName() );
//     }
//     description.append( " )" );
// }

// type_uid_hash = std::hash< std::string >()( description );

// return description.c_str();
// }

// const i16 Type::getBitsize( void )
// {
//     return bitsize;
// }

// const std::vector< Type* >& Type::getParameters( void ) const
// {
//     return parameters;
// }

// const std::vector< Type* >& Type::getSubTypes( void ) const
// {
//     return subtypes;
// }

// void Type::addParameter( Type* parameter )
// {
//     assert( parameter );
//     assert( type_state != Type::STATE::LOCKED );
//     parameters.push_back( parameter );

//     type_state = Type::STATE::CHANGED;
//     getName();
//     type_state = Type::STATE::UNCHANGED;
// }

// void Type::addSubType( Type* subtype )
// {
//     assert( subtype );
//     assert( type_state != Type::STATE::LOCKED );
//     subtypes.push_back( subtype );

//     type_state = Type::STATE::CHANGED;
//     getName();
//     type_state = Type::STATE::UNCHANGED;
// }

Type* Type::getResult( void ) const
{
    if( getID() == Type::RELATION )
    {
        const RelationType* rt = static_cast< const RelationType* >( this );
        return (Type*)rt->getResult();
    }
    return (Type*)this;
}

// Type* Type::getResultType( void )
// {
//     if( subtypes.size() == 0 )
//     {
//         if( type_id == Type::AGENT )
//         {
//             return &AgentType;
//         }
//         else if( type_id == Type::RULE_POINTER )
//         {
//             return &RulePointerType;
//         }
//         else if( type_id == Type::BOOLEAN )
//         {
//             return &BooleanType;
//         }
//         else if( type_id == Type::INTEGER )
//         {
//             return &IntegerType;
//         }
//         else if( type_id == Type::STRING )
//         {
//             return &StringType;
//         }
//         else if( type_id == Type::BIT )
//         {
//             return new Type( type_id, bitsize );
//         }
//         else
//         {
//             assert( 0 && "unimplemented result type with subtypes size zero!"
//             );
//             return 0;
//         }
//     }
//     else
//     {
//         Type* t = new Type( type_id );
//         for( auto subtype : subtypes )
//         {
//             t->addSubType( subtype );
//         }
//         return t;
//     }
// }

Type* Type::getAgent( void )
{
    static AgentType cache = AgentType();
    return id2str().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getRuleReference( void )
{
    static RuleReferenceType cache = RuleReferenceType();
    return id2str().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getBoolean( void )
{
    static BooleanType cache = BooleanType();
    return id2str().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getInteger( void )
{
    static IntegerType cache = IntegerType();
    return id2str().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getBit( u8 size )
{
    BitType tmp( size );

    auto cache = id2str().find( tmp.getName() );
    if( cache != id2str().end() )
    {
        return cache->second;
    }

    Type* ptr = new BitType( tmp );
    id2str()[ tmp.getName() ] = ptr;
    return ptr;
}

Type* Type::getString( void )
{
    static StringType cache = StringType();
    return id2str().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getFloating( void )
{
    static FloatingType cache = FloatingType();
    return id2str().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getRational( void )
{
    static RationalType cache = RationalType();
    return id2str().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getEnumeration( const char* name )
{
    // static std::unordered_map< u8, EnumerationType* > cache;
    // auto result = cache.find( size );
    // if( result != cache.end() )
    // {
    //     assert( result->second );
    //     return result->second;
    // }

    // Type* tmp = new BitType( size );
    // assert( tmp );
    // cache[ size ] = tmp;
    // return tmp;
    assert( !" TODO " );

    return 0;
}

Type* Type::getRelation( Type* result, std::vector< Type* > arguments )
{
    RelationType tmp( result, arguments );

    auto cache = id2str().find( tmp.getName() );
    if( cache != id2str().end() )
    {
        return cache->second;
    }

    Type* ptr = new RelationType( tmp );
    id2str()[ tmp.getName() ] = ptr;
    return ptr;
}

PrimitiveType::PrimitiveType( const char* name, ID id )
: Type( name, id )
{
}

const char* PrimitiveType::getName( void )
{
    return name;
}

AgentType::AgentType()
: PrimitiveType( "Agent", AGENT )
{
}

RuleReferenceType::RuleReferenceType()
: PrimitiveType( "RuleRef", RULE_REFERENCE )
{
}

BooleanType::BooleanType()
: PrimitiveType( "Boolean", BOOLEAN )
{
}

IntegerType::IntegerType()
: PrimitiveType( "Integer", INTEGER )

{
}

BitType::BitType( u8 size )
: PrimitiveType(
      libstdhl::Allocator::string( "Bit(" + std::to_string( size ) + ")" ), BIT )
, size( size )
{
}

StringType::StringType()
: PrimitiveType( "String", STRING )
{
}

FloatingType::FloatingType()
: PrimitiveType( "Floating", FLOATING )
{
}

RationalType::RationalType()
: PrimitiveType( "Rational", RATIONAL )
{
}

EnumerationType::EnumerationType( const char* name )
: PrimitiveType( name, ENUMERATION )
{
}

RelationType::RelationType( Type* result, std::vector< Type* > arguments )
: Type( " -> ", RELATION )
, result( result )
, arguments( arguments )
{
    assert( result );
}

const char* RelationType::getName( void )
{
    static const char* cache = 0;
    if( not cache )
    {
        u1 first = true;
        std::string tmp;
        for( auto argument : arguments )
        {
            if( not first )
            {
                tmp += " x ";
            }
            tmp += argument->getName();
            first = true;
        }

        tmp += name;
        tmp += result->getName();

        cache = libstdhl::Allocator::string( tmp );
    }

    return cache;
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

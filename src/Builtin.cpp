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

#include "Builtin.h"

using namespace libcasm_ir;

static Builtin pre_defined[]
    = { AsBooleanBuiltin(), AsIntegerBuiltin( &IntegerType )
        //, AsIntegerBuiltin( ... ) // ranged Integer is dynamically created!
        //, AsBitBuiltin() // is dynamically created!
        //, AsEnumerationBuiltin() // is dynamically created!
        ,
        AsStringBuiltin(), AsFloatingBuiltin() };

Builtin::Builtin( const char* name, Type* result, Type::ID ret_type,
    std::vector< std::vector< Type::ID > > arg_type, Value::ID id )
: User( name, result, id )
, ret_type( ret_type )
, arg_type( arg_type )
{
    getSymbols()[ ".builtin" ].insert( this );

    id2obj()[ id ] = this;
    str2obj()[ std::string( name ) ] = this;
}

Builtin::~Builtin( void )
{
    getSymbols()[ ".builtin" ].erase( this );
}

const Type::ID Builtin::getTypeIDsOfResult( void ) const
{
    return ret_type;
}

const std::vector< std::vector< Type::ID > >& Builtin::getTypeIDsOfArguments(
    void ) const
{
    return arg_type;
}

void Builtin::dump( void ) const
{
    printf( "[Builtin] " );
    debug();
}

bool Builtin::classof( Value const* obj )
{
    return obj->getValueID() == classid() or CastingBuiltin::classof( obj );
}

// Casting built-ins:

CastingBuiltin::CastingBuiltin( const char* name, Type* result,
    Type::ID ret_type, std::vector< std::vector< Type::ID > > arg_type,
    Value::ID id )
: Builtin( name, result, ret_type, arg_type, id )
{
}

bool CastingBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid() or AsBooleanBuiltin::classof( obj )
           or AsIntegerBuiltin::classof( obj ) or AsBitBuiltin::classof( obj )
           or AsEnumerationBuiltin::classof( obj )
           or AsStringBuiltin::classof( obj )
           or AsFloatingBuiltin::classof( obj );
}

AsBooleanBuiltin::AsBooleanBuiltin( void )
: CastingBuiltin( "asBoolean", &BooleanType, Type::BOOLEAN,
      { { Type::INTEGER, Type::BIT } }, Value::AS_INTEGER_BUILTIN )
{
}

bool AsBooleanBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

AsIntegerBuiltin::AsIntegerBuiltin( Type* result )
: CastingBuiltin( "asInteger", result, Type::INTEGER,
      { {
          Type::BOOLEAN,
          // TODO: PPA: Type::FLOATING,
          Type::BIT,
          // TODO: PPA: Type::ENUMERATION,
      } },
      Value::AS_INTEGER_BUILTIN )
{
}

bool AsIntegerBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

AsBitBuiltin::AsBitBuiltin( Type* result )
: CastingBuiltin( "asBit", result, Type::BIT,
      { {
            Type::INTEGER, Type::BOOLEAN, Type::FLOATING,
            // TODO: PPA: Type::ENUMERATION
        },
          { Type::INTEGER } },
      Value::AS_BIT_BUILTIN )
{
}

bool AsBitBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

AsEnumerationBuiltin::AsEnumerationBuiltin( Type* result, const char* token )
: CastingBuiltin( token, result, Type::ENUM,
      { {
            Type::INTEGER, Type::BOOLEAN, Type::FLOATING,
            // TODO: PPA: Type::ENUMERATION
        },
          { Type::INTEGER } },
      Value::AS_ENUM_BUILTIN )
{
}

bool AsEnumerationBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

AsStringBuiltin::AsStringBuiltin( void )
: CastingBuiltin( "asString", &StringType, Type::STRING,
      { {
          Type::BOOLEAN
          // TODO: PPA: Type::ENUMERATION
      } },
      Value::AS_STRING_BUILTIN )
{
}

bool AsStringBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

// asFloating : Integer  -> Floating, int to float converstion!
// asFloating : Boolean  -> Floating, false -> 0.0, true -> 1.0
// asFloating : Bit( n ) -> Floating, SHALL NOT BE POSSIBLE
// asFloating : e        -> Floating, e -> index(e).0, e !in index(e) -> undef,
// undef -> undef
//                                  , 'e' is a enumeration value of type 'e'

AsFloatingBuiltin::AsFloatingBuiltin( void )
: CastingBuiltin( "asFloating", &FloatingType, Type::FLOATING,
      { {
          Type::INTEGER, Type::BOOLEAN
          // TODO: PPA: Type::ENUMERATION
      } },
      Value::AS_FLOATING_BUILTIN )
{
}

bool AsFloatingBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

// asRational : TODO: PPA: !!!

// Stringify built-ins:

// dec  : Integer  -> String , decimal representation of integer
// dec  : Boolean  -> String , decimal representation of boolean
// dec  : Floating -> String , decimal representation of floating point value
// dec  : Bit( n ) -> String , decimal representation of bit-vector
// dec  : e        -> String , decimal representation of enumeration value of
// type 'e'

// hex  : Integer  -> String , hexadecimal representation of integer WITHOUT
// prefix '0x'
// hex  : Boolean  -> String , hexadecimal representation of boolean WITHOUT
// prefix '0x'
// hex  : Floating -> String , hexadecimal representation of floating point
// value WITHOUT prefix '0x'
// hex  : Bit( n ) -> String , hexadecimal representation of bit-vector WITHOUT
// prefix '0x'
// hex  : e        -> String , hexadecimal representation of enumeration value
// of type 'e' WITHOUT prefix '0x'

// bin  : Integer  -> String , binary representation of integer WITHOUT prefix
// '0b'
// bin  : Boolean  -> String , binary representation of boolean WITHOUT prefix
// '0b'
// bin  : Floating -> String , binary representation of floating point value
// WITHOUT prefix '0b'
// bin  : Bit( n ) -> String , binary representation of bit-vector WITHOUT
// prefix '0b'
// bin  : e        -> String , binary representation of enumeration value of
// type 'e' WITHOUT prefix '0b'

// Math built-ins:

// pow  : Integer * Integer -> Integer
// //     base      exponent
// rand : Integer * Integer -> Integer
// //     start     end

// Bit Operation built-ins:

// zext  : Bit( n ) * Integer (const, m) -> Bit( m ), zero extend to new size,
// if m < n then error!
// sext  : Bit( n ) * Integer (const, m) -> Bit( m ), sign extend to new size,
// if m < n then error!
// trunc : Bit( n ) * Integer (const, m) -> Bit( m ), truncate to new size, if m
// > n then error!

// shl   : Bit( n ) * Integer  -> Bit( n ), logic shift left of Integer value
// positions
// shl   : Bit( n ) * Bit( n ) -> Bit( n ), logic shift left of Bit(n) value
// positions

// shr   : Bit( n ) * Integer  -> Bit( n ), logic shift right of Integer value
// positions
// shr   : Bit( n ) * Bit( n ) -> Bit( n ), logic shift right of Bit(n) value
// positions

// ashr  : Bit( n ) * Integer  -> Bit( n ), arithmetic shift right of Integer
// value positions
// ashr  : Bit( n ) * Bit( n ) -> Bit( n ), arithmetic shift right of Bit(n)
// value positions

// clz   : Bit( n ) -> Integer, count leading zeros
// clo   : Bit( n ) -> Integer, count leading ones
// cls   : Bit( n ) -> Integer, count leading sign bits

// Generic built-ins

// isSymbolic : [TYPE*]  -> Boolean // if symbol of type TYPE is symbolic ->
// true, else -> false

// List built-ins:

//             NTH_BUILTIN,
//             CONS_BUILTIN,
//             APP_BUILTIN,
//             LEN_BUILTIN,
//             TAIL_BUILTIN,
//             PEEK_BUILTIN,

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

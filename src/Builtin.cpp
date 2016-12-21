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

Builtin::Builtin(
    const char* name, Type* result, const TypeAnnotation& info, Value::ID id )
: User( name, result, id )
, TypeAnnotation( info )
{
    getSymbols()[ ".builtin" ].insert( this );

    id2obj()[ id ] = this;
    str2obj()[ std::string( name ) ] = this;
}

Builtin::~Builtin( void )
{
    getSymbols()[ ".builtin" ].erase( this );
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

//
// CastingBuiltin
//

CastingBuiltin::CastingBuiltin(
    const char* name, Type* result, const TypeAnnotation& info, Value::ID id )
: Builtin( name, result, info, id )
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

//
// AsBooleanBuiltin
//

AsBooleanBuiltin::AsBooleanBuiltin( void )
: CastingBuiltin( "asBoolean", &BooleanType, info, Value::AS_BOOLEAN_BUILTIN )
{
}
const TypeAnnotation AsBooleanBuiltin::info( TypeAnnotation::Data{
    { Type::BOOLEAN, { Type::BOOLEAN } }, { Type::BOOLEAN, { Type::INTEGER } },
    { Type::BOOLEAN, { Type::FLOATING } }, { Type::BOOLEAN, { Type::BIT } },
    { Type::BOOLEAN, { Type::ENUMERATION } } } );
bool AsBooleanBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsIntegerBuiltin
//

AsIntegerBuiltin::AsIntegerBuiltin( Type* result )
: CastingBuiltin( "asInteger", result, info, Value::AS_INTEGER_BUILTIN )
{
}
const TypeAnnotation AsIntegerBuiltin::info( TypeAnnotation::Data{
    { Type::INTEGER, { Type::INTEGER } }, { Type::INTEGER, { Type::BOOLEAN } },
    { Type::INTEGER, { Type::FLOATING } }, { Type::INTEGER, { Type::BIT } },
    { Type::INTEGER, { Type::ENUMERATION } } } );
bool AsIntegerBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsBitBuiltin
//

AsBitBuiltin::AsBitBuiltin( Type* result )
: CastingBuiltin( "asBit", result, info, Value::AS_BIT_BUILTIN )
{
}
const TypeAnnotation AsBitBuiltin::info(
    TypeAnnotation::Data{ { Type::BIT, { Type::BIT, Type::INTEGER } },
        { Type::BIT, { Type::INTEGER, Type::INTEGER } },
        { Type::BIT, { Type::BOOLEAN, Type::INTEGER } },
        { Type::BIT, { Type::FLOATING, Type::INTEGER } },
        { Type::BIT, { Type::ENUMERATION, Type::INTEGER } } } );
bool AsBitBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsEnumerationBuiltin
//

AsEnumerationBuiltin::AsEnumerationBuiltin( Type* result, const char* token )
: CastingBuiltin( token, result, info, Value::AS_ENUMERATION_BUILTIN )
{
}
const TypeAnnotation AsEnumerationBuiltin::info(
    TypeAnnotation::Data{ { Type::ENUMERATION, { Type::INTEGER } },
        { Type::ENUMERATION, { Type::BIT } } } );
bool AsEnumerationBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsStringBuiltin
//

AsStringBuiltin::AsStringBuiltin( void )
: CastingBuiltin( "asString", &StringType, info, Value::AS_STRING_BUILTIN )
{
}
const TypeAnnotation AsStringBuiltin::info( TypeAnnotation::Data{
    { Type::STRING, { Type::STRING } }, { Type::STRING, { Type::INTEGER } },
    { Type::STRING, { Type::BOOLEAN } }, { Type::STRING, { Type::FLOATING } },
    { Type::STRING, { Type::BIT } },
    { Type::STRING, { Type::ENUMERATION } } } );
bool AsStringBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsFloatingBuiltin
//

AsFloatingBuiltin::AsFloatingBuiltin( void )
: CastingBuiltin(
      "asFloating", &FloatingType, info, Value::AS_FLOATING_BUILTIN )
{
}
const TypeAnnotation AsFloatingBuiltin::info( TypeAnnotation::Data{
    { Type::FLOATING, { Type::FLOATING } },
    { Type::FLOATING, { Type::INTEGER } },
    { Type::FLOATING, { Type::BOOLEAN } }, { Type::FLOATING, { Type::BIT } },
    { Type::FLOATING, { Type::ENUMERATION } } } );
bool AsFloatingBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

// asRational : TODO: PPA: !!!

// Stringify built-ins:

// dec  : Integer  -> String , decimal representation of integer
// dec  : Boolean  -> String , decimal representation of boolean
// dec  : Floating -> String , decimal representation of floating point
// value
// dec  : Bit( n ) -> String , decimal representation of bit-vector
// dec  : e        -> String , decimal representation of enumeration value
// of
// type 'e'

// hex  : Integer  -> String , hexadecimal representation of integer WITHOUT
// prefix '0x'
// hex  : Boolean  -> String , hexadecimal representation of boolean WITHOUT
// prefix '0x'
// hex  : Floating -> String , hexadecimal representation of floating point
// value WITHOUT prefix '0x'
// hex  : Bit( n ) -> String , hexadecimal representation of bit-vector
// WITHOUT
// prefix '0x'
// hex  : e        -> String , hexadecimal representation of enumeration
// value
// of type 'e' WITHOUT prefix '0x'

// bin  : Integer  -> String , binary representation of integer WITHOUT
// prefix
// '0b'
// bin  : Boolean  -> String , binary representation of boolean WITHOUT
// prefix
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

// zext  : Bit( n ) * Integer (const, m) -> Bit( m ), zero extend to new
// size,
// if m < n then error!
// sext  : Bit( n ) * Integer (const, m) -> Bit( m ), sign extend to new
// size,
// if m < n then error!
// trunc : Bit( n ) * Integer (const, m) -> Bit( m ), truncate to new size,
// if
// m
// > n then error!

// shl   : Bit( n ) * Integer  -> Bit( n ), logic shift left of Integer
// value
// positions
// shl   : Bit( n ) * Bit( n ) -> Bit( n ), logic shift left of Bit(n) value
// positions

// shr   : Bit( n ) * Integer  -> Bit( n ), logic shift right of Integer
// value
// positions
// shr   : Bit( n ) * Bit( n ) -> Bit( n ), logic shift right of Bit(n)
// value
// positions

// ashr  : Bit( n ) * Integer  -> Bit( n ), arithmetic shift right of
// Integer
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

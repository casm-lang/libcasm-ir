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

#include "Builtin.h"

#include "../stdhl/cpp/Log.h"

using namespace libcasm_ir;

Builtin::Builtin(
    const char* name, Type* result, const TypeAnnotation& info, Value::ID id )
: User( name, result, id )
, TypeAnnotation( info )
, description( 0 )
{
}

Builtin::~Builtin( void )
{
}

const char* Builtin::getDescription( void )
{
    if( not description )
    {
        std::string tmp = "";
        tmp += getType()->getName();
        tmp += " ";
        tmp += getName();

        description = libstdhl::Allocator::string( tmp );
    }

    return description;
}

u1 Builtin::classof( Value const* obj )
{
    return obj->getValueID() == classid() or CastingBuiltin::classof( obj )
           or StringifyBuiltin::classof( obj )
           // or MathBuiltin::classof( obj )
           or OperatorBuiltin::classof( obj ) or BitBuiltin::classof( obj );
}

Builtin* Builtin::find( const char* name, Type* result )
{
    std::string tmp = name;
    if( tmp.rfind( "as", 0 ) == 0 )
    {
        return getAsBuiltin( result );
    }
    else if( tmp.compare( "dec" ) == 0 )
    {
        return get< DecBuiltin >( result );
    }
    else if( tmp.compare( "hex" ) == 0 )
    {
        return get< HexBuiltin >( result );
    }
    else if( tmp.compare( "oct" ) == 0 )
    {
        return get< OctBuiltin >( result );
    }
    else if( tmp.compare( "bin" ) == 0 )
    {
        return get< BinBuiltin >( result );
    }

    else if( tmp.compare( "addu" ) == 0 )
    {
        return get< AdduBuiltin >( result );
    }
    else if( tmp.compare( "adds" ) == 0 )
    {
        return get< AddsBuiltin >( result );
    }
    else if( tmp.compare( "subu" ) == 0 )
    {
        return get< SubuBuiltin >( result );
    }
    else if( tmp.compare( "subs" ) == 0 )
    {
        return get< SubsBuiltin >( result );
    }
    else if( tmp.compare( "mulu" ) == 0 )
    {
        return get< MuluBuiltin >( result );
    }
    else if( tmp.compare( "muls" ) == 0 )
    {
        return get< MulsBuiltin >( result );
    }

    else if( tmp.compare( "lesu" ) == 0 )
    {
        return get< LesuBuiltin >( result );
    }
    else if( tmp.compare( "less" ) == 0 )
    {
        return get< LessBuiltin >( result );
    }
    else if( tmp.compare( "lequ" ) == 0 )
    {
        return get< LequBuiltin >( result );
    }
    else if( tmp.compare( "leqs" ) == 0 )
    {
        return get< LeqsBuiltin >( result );
    }
    else if( tmp.compare( "greu" ) == 0 )
    {
        return get< GreuBuiltin >( result );
    }
    else if( tmp.compare( "gres" ) == 0 )
    {
        return get< GresBuiltin >( result );
    }
    else if( tmp.compare( "gequ" ) == 0 )
    {
        return get< GequBuiltin >( result );
    }
    else if( tmp.compare( "geqs" ) == 0 )
    {
        return get< GeqsBuiltin >( result );
    }

    else if( tmp.compare( "zext" ) == 0 )
    {
        return get< ZextBuiltin >( result );
    }
    else if( tmp.compare( "sext" ) == 0 )
    {
        return get< SextBuiltin >( result );
    }
    else if( tmp.compare( "trunc" ) == 0 )
    {
        return get< TruncBuiltin >( result );
    }
    else if( tmp.compare( "shl" ) == 0 )
    {
        return get< ShlBuiltin >( result );
    }
    else if( tmp.compare( "shr" ) == 0 )
    {
        return get< ShrBuiltin >( result );
    }
    else if( tmp.compare( "ashr" ) == 0 )
    {
        return get< AshrBuiltin >( result );
    }
    else if( tmp.compare( "clz" ) == 0 )
    {
        return get< ClzBuiltin >( result );
    }
    else if( tmp.compare( "clo" ) == 0 )
    {
        return get< CloBuiltin >( result );
    }
    else if( tmp.compare( "cls" ) == 0 )
    {
        return get< ClsBuiltin >( result );
    }

    else
    {
        libstdhl::Log::error(
            "could not find a builtin for '%s'", tmp.c_str() );
        return 0;
    }
}

Builtin* Builtin::getAsBuiltin( Type* result )
{
    assert( result );

    std::string tmp = "as";
    tmp += result->getDescription();

    auto cache = str2obj().find( tmp );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    switch( result->getResult()->getID() )
    {
        case Type::BOOLEAN:
        {
            return get< AsBooleanBuiltin >( result );
        }
        case Type::INTEGER:
        {
            return get< AsIntegerBuiltin >( result );
        }
        case Type::BIT:
        {
            return get< AsBitBuiltin >( result );
        }
        case Type::STRING:
        {
            return get< AsStringBuiltin >( result );
        }
        case Type::FLOATING:
        {
            return get< AsFloatingBuiltin >( result );
        }
        case Type::RATIONAL:
        {
            return get< AsRationalBuiltin >( result );
        }
        case Type::ENUMERATION:
        {
            return get< AsEnumerationBuiltin >( result );
        }
        default:
        {
            libstdhl::Log::error(
                "could not find a builtin for '%s'", tmp.c_str() );
            return 0;
        }
    }
}

template < typename T >
Builtin* Builtin::get( Type* result )
{
    T tmp = T( result );

    std::string key = tmp.getDescription();

    auto cache = str2obj().find( key );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Builtin* ptr = new T( tmp );
    return str2obj().emplace( key, ptr ).first->second;
}

//------------------------------------------------------------------------------

//
// CastingBuiltin
//

CastingBuiltin::CastingBuiltin(
    const char* name, Type* result, const TypeAnnotation& info, Value::ID id )
: Builtin( name, result, info, id )
{
}

u1 CastingBuiltin::classof( Value const* obj )
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

AsBooleanBuiltin::AsBooleanBuiltin( Type* result )
: CastingBuiltin( "asBoolean", result, info, classid() )
{
}
const TypeAnnotation AsBooleanBuiltin::info( TypeAnnotation::Data{

    { Type::BOOLEAN,
        {
            Type::BOOLEAN,
        } },
    { Type::BOOLEAN,
        {
            Type::INTEGER,
        } },
    { Type::BOOLEAN, // PPA: I think this should not be allowed
        {
            Type::FLOATING,
        } },
    { Type::BOOLEAN,
        {
            Type::BIT,
        } },
    { Type::BOOLEAN,
        {
            Type::ENUMERATION,
        } }

} );
u1 AsBooleanBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsIntegerBuiltin
//

AsIntegerBuiltin::AsIntegerBuiltin( Type* result )
: CastingBuiltin( "asInteger", result, info, classid() )
{
}
const TypeAnnotation AsIntegerBuiltin::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::INTEGER,
        } },
    { Type::INTEGER,
        {
            Type::BOOLEAN,
        } },
    { Type::INTEGER,
        {
            Type::FLOATING,
        } },
    { Type::INTEGER,
        {
            Type::BIT,
        } },
    { Type::INTEGER,
        {
            Type::ENUMERATION,
        } }

} );
u1 AsIntegerBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsBitBuiltin
//

AsBitBuiltin::AsBitBuiltin( Type* result )
: CastingBuiltin( "asBit", result, info, classid() )
{
}
const TypeAnnotation AsBitBuiltin::info( TypeAnnotation::Data{

    { Type::BIT,
        {
            Type::BIT, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::INTEGER, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::BOOLEAN, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::FLOATING, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::ENUMERATION, Type::INTEGER,
        } }

} );
u1 AsBitBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsStringBuiltin
//

AsStringBuiltin::AsStringBuiltin( Type* result )
: CastingBuiltin( "asString", result, info, classid() )
{
}
const TypeAnnotation AsStringBuiltin::info( TypeAnnotation::Data{

    { Type::STRING,
        {
            Type::STRING,
        } },
    { Type::STRING,
        {
            Type::INTEGER,
        } },
    { Type::STRING,
        {
            Type::BOOLEAN,
        } },
    { Type::STRING,
        {
            Type::FLOATING,
        } },
    { Type::STRING,
        {
            Type::BIT,
        } },
    { Type::STRING,
        {
            Type::ENUMERATION,
        } }

} );
u1 AsStringBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsFloatingBuiltin
//

AsFloatingBuiltin::AsFloatingBuiltin( Type* result )
: CastingBuiltin( "asFloating", result, info, classid() )
{
}
const TypeAnnotation AsFloatingBuiltin::info( TypeAnnotation::Data{

    { Type::FLOATING,
        {
            Type::FLOATING,
        } },
    { Type::FLOATING,
        {
            Type::INTEGER,
        } },
    { Type::FLOATING,
        {
            Type::BOOLEAN,
        } },
    { Type::FLOATING,
        {
            Type::BIT,
        } },
    { Type::FLOATING,
        {
            Type::ENUMERATION,
        } }

} );
u1 AsFloatingBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsRationalBuiltin
//

AsRationalBuiltin::AsRationalBuiltin( Type* result )
: CastingBuiltin( "asRational", result, info, classid() )
{
}
const TypeAnnotation AsRationalBuiltin::info( TypeAnnotation::Data{

    { Type::RATIONAL, { Type::RATIONAL } }

    // TODO: PPA: add more relations for possible input types!
} );
u1 AsRationalBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AsEnumerationBuiltin
//

AsEnumerationBuiltin::AsEnumerationBuiltin( Type* result )
: CastingBuiltin( libstdhl::Allocator::string(
                      "as" + std::string( result->getDescription() ) ),
      result, info, classid() )
{
}
const TypeAnnotation AsEnumerationBuiltin::info( TypeAnnotation::Data{

    { Type::ENUMERATION,
        {
            Type::INTEGER,
        } },
    { Type::ENUMERATION,
        {
            Type::BIT,
        } }

} );
u1 AsEnumerationBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// StringifyBuiltin
//

StringifyBuiltin::StringifyBuiltin(
    const char* name, Type* result, Value::ID id )
: Builtin( name, result, info, id )
{
}
const TypeAnnotation StringifyBuiltin::info( TypeAnnotation::Data{

    { Type::STRING,
        {
            Type::BOOLEAN,
        } },
    { Type::STRING,
        {
            Type::INTEGER,
        } },
    { Type::STRING,
        {
            Type::BIT,
        } },
    { Type::STRING,
        {
            Type::FLOATING,
        } },
    { Type::STRING,
        {
            Type::RATIONAL,
        } },
    { Type::STRING,
        {
            Type::ENUMERATION,
        } }

} );
u1 StringifyBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid() or DecBuiltin::classof( obj )
           or HexBuiltin::classof( obj ) or OctBuiltin::classof( obj )
           or BinBuiltin::classof( obj );
}

//
// DecBuiltin
//

DecBuiltin::DecBuiltin( Type* result )
: StringifyBuiltin( "dec", result, classid() )
{
}
u1 DecBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// HexBuiltin
//

HexBuiltin::HexBuiltin( Type* result )
: StringifyBuiltin( "hex", result, classid() )
{
}
u1 HexBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// OctBuiltin
//

OctBuiltin::OctBuiltin( Type* result )
: StringifyBuiltin( "oct", result, classid() )
{
}
u1 OctBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// BinBuiltin
//

BinBuiltin::BinBuiltin( Type* result )
: StringifyBuiltin( "bin", result, classid() )
{
}
u1 BinBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

// Math built-ins:

// pow  : Integer * Integer -> Integer
// //     base      exponent
// rand : Integer * Integer -> Integer
// //     start     end

//
// OperatorBuiltin
//

OperatorBuiltin::OperatorBuiltin( const char* name, Type* result, Value::ID id )
: Builtin( name, result, info, id )
{
}
const TypeAnnotation OperatorBuiltin::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::INTEGER, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );
u1 OperatorBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid() or ArithmeticBuiltin::classof( obj )
           or CompareBuiltin::classof( obj );
}

//
// ArithmeticBuiltin
//

ArithmeticBuiltin::ArithmeticBuiltin(
    const char* name, Type* result, Value::ID id )
: OperatorBuiltin( name, result, id )
{
}
u1 ArithmeticBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid() or AdduBuiltin::classof( obj )
           or AddsBuiltin::classof( obj ) or SubuBuiltin::classof( obj )
           or SubsBuiltin::classof( obj ) or MuluBuiltin::classof( obj )
           or MulsBuiltin::classof( obj );
}

//
// AdduBuiltin
//

AdduBuiltin::AdduBuiltin( Type* result )
: ArithmeticBuiltin( "addu", result, classid() )
{
}
u1 AdduBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AddsBuiltin
//

AddsBuiltin::AddsBuiltin( Type* result )
: ArithmeticBuiltin( "adds", result, classid() )
{
}
u1 AddsBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// SubuBuiltin
//

SubuBuiltin::SubuBuiltin( Type* result )
: ArithmeticBuiltin( "subu", result, classid() )
{
}
u1 SubuBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// SubsBuiltin
//

SubsBuiltin::SubsBuiltin( Type* result )
: ArithmeticBuiltin( "subs", result, classid() )
{
}
u1 SubsBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// MuluBuiltin
//

MuluBuiltin::MuluBuiltin( Type* result )
: ArithmeticBuiltin( "mulu", result, classid() )
{
}
u1 MuluBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// MulsBuiltin
//

MulsBuiltin::MulsBuiltin( Type* result )
: ArithmeticBuiltin( "muls", result, classid() )
{
}
u1 MulsBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// CompareBuiltin
//

CompareBuiltin::CompareBuiltin( const char* name, Type* result, Value::ID id )
: OperatorBuiltin( name, result, id )
{
}
u1 CompareBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid() or LesuBuiltin::classof( obj )
           or LessBuiltin::classof( obj ) or LequBuiltin::classof( obj )
           or LeqsBuiltin::classof( obj ) or GreuBuiltin::classof( obj )
           or GresBuiltin::classof( obj ) or GequBuiltin::classof( obj )
           or GeqsBuiltin::classof( obj );
}

//
// LesuBuiltin
//

LesuBuiltin::LesuBuiltin( Type* result )
: CompareBuiltin( "lesu", result, classid() )
{
}
u1 LesuBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// LessBuiltin
//

LessBuiltin::LessBuiltin( Type* result )
: CompareBuiltin( "less", result, classid() )
{
}
u1 LessBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// LequBuiltin
//

LequBuiltin::LequBuiltin( Type* result )
: CompareBuiltin( "lequ", result, classid() )
{
}
u1 LequBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// LeqsBuiltin
//

LeqsBuiltin::LeqsBuiltin( Type* result )
: CompareBuiltin( "leqs", result, classid() )
{
}
u1 LeqsBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// GreuBuiltin
//

GreuBuiltin::GreuBuiltin( Type* result )
: CompareBuiltin( "greu", result, classid() )
{
}
u1 GreuBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// GresBuiltin
//

GresBuiltin::GresBuiltin( Type* result )
: CompareBuiltin( "gres", result, classid() )
{
}
u1 GresBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// GequBuiltin
//

GequBuiltin::GequBuiltin( Type* result )
: CompareBuiltin( "gequ", result, classid() )
{
}
u1 GequBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// GeqsBuiltin
//

GeqsBuiltin::GeqsBuiltin( Type* result )
: CompareBuiltin( "geqs", result, classid() )
{
}
u1 GeqsBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// BitBuiltin
//

BitBuiltin::BitBuiltin(
    const char* name, Type* result, const TypeAnnotation& info, Value::ID id )
: Builtin( name, result, info, id )
{
}

u1 BitBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid() or ZextBuiltin::classof( obj )
           or SextBuiltin::classof( obj ) or TruncBuiltin::classof( obj )
           or ShlBuiltin::classof( obj ) or ShrBuiltin::classof( obj )
           or AshrBuiltin::classof( obj ) or ClzBuiltin::classof( obj )
           or CloBuiltin::classof( obj ) or ClsBuiltin::classof( obj );
}

//
// ZextBuiltin
//

ZextBuiltin::ZextBuiltin( Type* result )
: BitBuiltin( "zext", result, info, classid() )
{
}
const TypeAnnotation ZextBuiltin::info( TypeAnnotation::Data{

    { Type::BIT,
        {
            Type::BIT, Type::INTEGER,
        } }

} );
u1 ZextBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// SextBuiltin
//

SextBuiltin::SextBuiltin( Type* result )
: BitBuiltin( "sext", result, info, classid() )
{
}
const TypeAnnotation SextBuiltin::info( TypeAnnotation::Data{

    { Type::BIT,
        {
            Type::BIT, Type::INTEGER,
        } }

} );
u1 SextBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// TruncBuiltin
//

TruncBuiltin::TruncBuiltin( Type* result )
: BitBuiltin( "trunc", result, info, classid() )
{
}
const TypeAnnotation TruncBuiltin::info( TypeAnnotation::Data{

    { Type::BIT,
        {
            Type::BIT, Type::INTEGER,
        } }

} );
u1 TruncBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// ShlBuiltin
//

ShlBuiltin::ShlBuiltin( Type* result )
: BitBuiltin( "shl", result, info, classid() )
{
}
const TypeAnnotation ShlBuiltin::info( TypeAnnotation::Data{

    { Type::BIT,
        {
            Type::BIT, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );
u1 ShlBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// ShrBuiltin
//

ShrBuiltin::ShrBuiltin( Type* result )
: BitBuiltin( "shr", result, info, classid() )
{
}
const TypeAnnotation ShrBuiltin::info( TypeAnnotation::Data{

    { Type::BIT,
        {
            Type::BIT, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );
u1 ShrBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// AshrBuiltin
//

AshrBuiltin::AshrBuiltin( Type* result )
: BitBuiltin( "ashr", result, info, classid() )
{
}
const TypeAnnotation AshrBuiltin::info( TypeAnnotation::Data{

    { Type::BIT,
        {
            Type::BIT, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );
u1 AshrBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// ClzBuiltin
//

ClzBuiltin::ClzBuiltin( Type* result )
: BitBuiltin( "clz", result, info, classid() )
{
}
const TypeAnnotation ClzBuiltin::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::BIT,
        } }

} );
u1 ClzBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// CloBuiltin
//

CloBuiltin::CloBuiltin( Type* result )
: BitBuiltin( "clo", result, info, classid() )
{
}
const TypeAnnotation CloBuiltin::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::BIT,
        } }

} );
u1 CloBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
// ClsBuiltin
//

ClsBuiltin::ClsBuiltin( Type* result )
: BitBuiltin( "cls", result, info, classid() )
{
}
const TypeAnnotation ClsBuiltin::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::BIT,
        } }

} );
u1 ClsBuiltin::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

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

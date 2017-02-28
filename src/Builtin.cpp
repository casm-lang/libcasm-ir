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

using namespace libcasm_ir;

Builtin::Builtin( const std::string& name, const Type::Ptr& type,
    const TypeAnnotation& info, Value::ID id )
: User( name, type, id )
, TypeAnnotation( info )
{
}

Builtin::~Builtin( void )
{
}

u1 Builtin::classof( Value const* obj )
{
    return obj->id() == classid() or CastingBuiltin::classof( obj )
           or StringifyBuiltin::classof( obj )
           // or MathBuiltin::classof( obj )
           or OperatorBuiltin::classof( obj ) or BitBuiltin::classof( obj );
}

Builtin::Ptr Builtin::find( const std::string& name, const Type::Ptr& type )
{
    if( name.rfind( "as", 0 ) == 0 )
    {
        return asBuiltin( type );
    }
    else if( name.compare( "dec" ) == 0 )
    {
        return libstdhl::get< DecBuiltin >( type );
    }
    else if( name.compare( "hex" ) == 0 )
    {
        return libstdhl::get< HexBuiltin >( type );
    }
    else if( name.compare( "oct" ) == 0 )
    {
        return libstdhl::get< OctBuiltin >( type );
    }
    else if( name.compare( "bin" ) == 0 )
    {
        return libstdhl::get< BinBuiltin >( type );
    }

    else if( name.compare( "addu" ) == 0 )
    {
        return libstdhl::get< AdduBuiltin >( type );
    }
    else if( name.compare( "adds" ) == 0 )
    {
        return libstdhl::get< AddsBuiltin >( type );
    }
    else if( name.compare( "subu" ) == 0 )
    {
        return libstdhl::get< SubuBuiltin >( type );
    }
    else if( name.compare( "subs" ) == 0 )
    {
        return libstdhl::get< SubsBuiltin >( type );
    }
    else if( name.compare( "mulu" ) == 0 )
    {
        return libstdhl::get< MuluBuiltin >( type );
    }
    else if( name.compare( "muls" ) == 0 )
    {
        return libstdhl::get< MulsBuiltin >( type );
    }

    else if( name.compare( "lesu" ) == 0 )
    {
        return libstdhl::get< LesuBuiltin >( type );
    }
    else if( name.compare( "less" ) == 0 )
    {
        return libstdhl::get< LessBuiltin >( type );
    }
    else if( name.compare( "lequ" ) == 0 )
    {
        return libstdhl::get< LequBuiltin >( type );
    }
    else if( name.compare( "leqs" ) == 0 )
    {
        return libstdhl::get< LeqsBuiltin >( type );
    }
    else if( name.compare( "greu" ) == 0 )
    {
        return libstdhl::get< GreuBuiltin >( type );
    }
    else if( name.compare( "gres" ) == 0 )
    {
        return libstdhl::get< GresBuiltin >( type );
    }
    else if( name.compare( "gequ" ) == 0 )
    {
        return libstdhl::get< GequBuiltin >( type );
    }
    else if( name.compare( "geqs" ) == 0 )
    {
        return libstdhl::get< GeqsBuiltin >( type );
    }

    else if( name.compare( "zext" ) == 0 )
    {
        return libstdhl::get< ZextBuiltin >( type );
    }
    else if( name.compare( "sext" ) == 0 )
    {
        return libstdhl::get< SextBuiltin >( type );
    }
    else if( name.compare( "trunc" ) == 0 )
    {
        return libstdhl::get< TruncBuiltin >( type );
    }
    else if( name.compare( "shl" ) == 0 )
    {
        return libstdhl::get< ShlBuiltin >( type );
    }
    else if( name.compare( "shr" ) == 0 )
    {
        return libstdhl::get< ShrBuiltin >( type );
    }
    else if( name.compare( "ashr" ) == 0 )
    {
        return libstdhl::get< AshrBuiltin >( type );
    }
    else if( name.compare( "clz" ) == 0 )
    {
        return libstdhl::get< ClzBuiltin >( type );
    }
    else if( name.compare( "clo" ) == 0 )
    {
        return libstdhl::get< CloBuiltin >( type );
    }
    else if( name.compare( "cls" ) == 0 )
    {
        return libstdhl::get< ClsBuiltin >( type );
    }

    else
    {
        libstdhl::Log::error(
            "could not find a builtin for '%s'", name.c_str() );
        return nullptr;
    }
}

Builtin::Ptr Builtin::asBuiltin( const Type::Ptr& type )
{
    switch( type->result().id() )
    {
        case Type::BOOLEAN:
        {
            return libstdhl::get< AsBooleanBuiltin >( type );
        }
        case Type::INTEGER:
        {
            return libstdhl::get< AsIntegerBuiltin >( type );
        }
        case Type::BIT:
        {
            return libstdhl::get< AsBitBuiltin >( type );
        }
        case Type::STRING:
        {
            return libstdhl::get< AsStringBuiltin >( type );
        }
        case Type::FLOATING:
        {
            return libstdhl::get< AsFloatingBuiltin >( type );
        }
        case Type::RATIONAL:
        {
            return libstdhl::get< AsRationalBuiltin >( type );
        }
        case Type::ENUMERATION:
        {
            return libstdhl::get< AsEnumerationBuiltin >( type );
        }
        default:
        {
            libstdhl::Log::error(
                "could not find a builtin for '%s'", type->name() );

            return nullptr;
        }
    }
}

//------------------------------------------------------------------------------

//
// CastingBuiltin
//

CastingBuiltin::CastingBuiltin( const std::string& name, const Type::Ptr& type,
    const TypeAnnotation& info, Value::ID id )
: Builtin( name, type, info, id )
{
}

u1 CastingBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or AsBooleanBuiltin::classof( obj )
           or AsIntegerBuiltin::classof( obj ) or AsBitBuiltin::classof( obj )
           or AsEnumerationBuiltin::classof( obj )
           or AsStringBuiltin::classof( obj )
           or AsFloatingBuiltin::classof( obj );
}

//
// AsBooleanBuiltin
//

AsBooleanBuiltin::AsBooleanBuiltin( const Type::Ptr& type )
: CastingBuiltin( "asBoolean", type, info, classid() )
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
    return obj->id() == classid();
}

//
// AsIntegerBuiltin
//

AsIntegerBuiltin::AsIntegerBuiltin( const Type::Ptr& type )
: CastingBuiltin( "asInteger", type, info, classid() )
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
    return obj->id() == classid();
}

//
// AsBitBuiltin
//

AsBitBuiltin::AsBitBuiltin( const Type::Ptr& type )
: CastingBuiltin( "asBit", type, info, classid() )
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
    return obj->id() == classid();
}

//
// AsStringBuiltin
//

AsStringBuiltin::AsStringBuiltin( const Type::Ptr& type )
: CastingBuiltin( "asString", type, info, classid() )
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
    return obj->id() == classid();
}

//
// AsFloatingBuiltin
//

AsFloatingBuiltin::AsFloatingBuiltin( const Type::Ptr& type )
: CastingBuiltin( "asFloating", type, info, classid() )
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
    return obj->id() == classid();
}

//
// AsRationalBuiltin
//

AsRationalBuiltin::AsRationalBuiltin( const Type::Ptr& type )
: CastingBuiltin( "asRational", type, info, classid() )
{
}
const TypeAnnotation AsRationalBuiltin::info( TypeAnnotation::Data{

    { Type::RATIONAL, { Type::RATIONAL } }

    // TODO: PPA: add more relations for possible input types!
} );
u1 AsRationalBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsEnumerationBuiltin
//

AsEnumerationBuiltin::AsEnumerationBuiltin( const Type::Ptr& type )
: CastingBuiltin( "as" + type->str_name(), type, info, classid() )
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
    return obj->id() == classid();
}

//
// StringifyBuiltin
//

StringifyBuiltin::StringifyBuiltin(
    const std::string& name, const Type::Ptr& type, Value::ID id )
: Builtin( name, type, info, id )
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
    return obj->id() == classid() or DecBuiltin::classof( obj )
           or HexBuiltin::classof( obj ) or OctBuiltin::classof( obj )
           or BinBuiltin::classof( obj );
}

//
// DecBuiltin
//

DecBuiltin::DecBuiltin( const Type::Ptr& type )
: StringifyBuiltin( "dec", type, classid() )
{
}
u1 DecBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// HexBuiltin
//

HexBuiltin::HexBuiltin( const Type::Ptr& type )
: StringifyBuiltin( "hex", type, classid() )
{
}
u1 HexBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// OctBuiltin
//

OctBuiltin::OctBuiltin( const Type::Ptr& type )
: StringifyBuiltin( "oct", type, classid() )
{
}
u1 OctBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// BinBuiltin
//

BinBuiltin::BinBuiltin( const Type::Ptr& type )
: StringifyBuiltin( "bin", type, classid() )
{
}
u1 BinBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

// Math built-ins:

// pow  : Integer * Integer -> Integer
// //     base      exponent
// rand : Integer * Integer -> Integer
// //     start     end

//
// OperatorBuiltin
//

OperatorBuiltin::OperatorBuiltin(
    const std::string& name, const Type::Ptr& type, Value::ID id )
: Builtin( name, type, info, id )
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
    return obj->id() == classid() or ArithmeticBuiltin::classof( obj )
           or CompareBuiltin::classof( obj );
}

//
// ArithmeticBuiltin
//

ArithmeticBuiltin::ArithmeticBuiltin(
    const std::string& name, const Type::Ptr& type, Value::ID id )
: OperatorBuiltin( name, type, id )
{
}
u1 ArithmeticBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or AdduBuiltin::classof( obj )
           or AddsBuiltin::classof( obj ) or SubuBuiltin::classof( obj )
           or SubsBuiltin::classof( obj ) or MuluBuiltin::classof( obj )
           or MulsBuiltin::classof( obj );
}

//
// AdduBuiltin
//

AdduBuiltin::AdduBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( "addu", type, classid() )
{
}
u1 AdduBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AddsBuiltin
//

AddsBuiltin::AddsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( "adds", type, classid() )
{
}
u1 AddsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SubuBuiltin
//

SubuBuiltin::SubuBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( "subu", type, classid() )
{
}
u1 SubuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SubsBuiltin
//

SubsBuiltin::SubsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( "subs", type, classid() )
{
}
u1 SubsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// MuluBuiltin
//

MuluBuiltin::MuluBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( "mulu", type, classid() )
{
}
u1 MuluBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// MulsBuiltin
//

MulsBuiltin::MulsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( "muls", type, classid() )
{
}
u1 MulsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// CompareBuiltin
//

CompareBuiltin::CompareBuiltin(
    const std::string& name, const Type::Ptr& type, Value::ID id )
: OperatorBuiltin( name, type, id )
{
}
u1 CompareBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or LesuBuiltin::classof( obj )
           or LessBuiltin::classof( obj ) or LequBuiltin::classof( obj )
           or LeqsBuiltin::classof( obj ) or GreuBuiltin::classof( obj )
           or GresBuiltin::classof( obj ) or GequBuiltin::classof( obj )
           or GeqsBuiltin::classof( obj );
}

//
// LesuBuiltin
//

LesuBuiltin::LesuBuiltin( const Type::Ptr& type )
: CompareBuiltin( "lesu", type, classid() )
{
}
u1 LesuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LessBuiltin
//

LessBuiltin::LessBuiltin( const Type::Ptr& type )
: CompareBuiltin( "less", type, classid() )
{
}
u1 LessBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LequBuiltin
//

LequBuiltin::LequBuiltin( const Type::Ptr& type )
: CompareBuiltin( "lequ", type, classid() )
{
}
u1 LequBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LeqsBuiltin
//

LeqsBuiltin::LeqsBuiltin( const Type::Ptr& type )
: CompareBuiltin( "leqs", type, classid() )
{
}
u1 LeqsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GreuBuiltin
//

GreuBuiltin::GreuBuiltin( const Type::Ptr& type )
: CompareBuiltin( "greu", type, classid() )
{
}
u1 GreuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GresBuiltin
//

GresBuiltin::GresBuiltin( const Type::Ptr& type )
: CompareBuiltin( "gres", type, classid() )
{
}
u1 GresBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GequBuiltin
//

GequBuiltin::GequBuiltin( const Type::Ptr& type )
: CompareBuiltin( "gequ", type, classid() )
{
}
u1 GequBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GeqsBuiltin
//

GeqsBuiltin::GeqsBuiltin( const Type::Ptr& type )
: CompareBuiltin( "geqs", type, classid() )
{
}
u1 GeqsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// BitBuiltin
//

BitBuiltin::BitBuiltin( const std::string& name, const Type::Ptr& type,
    const TypeAnnotation& info, Value::ID id )
: Builtin( name, type, info, id )
{
}

u1 BitBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or ZextBuiltin::classof( obj )
           or SextBuiltin::classof( obj ) or TruncBuiltin::classof( obj )
           or ShlBuiltin::classof( obj ) or ShrBuiltin::classof( obj )
           or AshrBuiltin::classof( obj ) or ClzBuiltin::classof( obj )
           or CloBuiltin::classof( obj ) or ClsBuiltin::classof( obj );
}

//
// ZextBuiltin
//

ZextBuiltin::ZextBuiltin( const Type::Ptr& type )
: BitBuiltin( "zext", type, info, classid() )
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
    return obj->id() == classid();
}

//
// SextBuiltin
//

SextBuiltin::SextBuiltin( const Type::Ptr& type )
: BitBuiltin( "sext", type, info, classid() )
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
    return obj->id() == classid();
}

//
// TruncBuiltin
//

TruncBuiltin::TruncBuiltin( const Type::Ptr& type )
: BitBuiltin( "trunc", type, info, classid() )
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
    return obj->id() == classid();
}

//
// ShlBuiltin
//

ShlBuiltin::ShlBuiltin( const Type::Ptr& type )
: BitBuiltin( "shl", type, info, classid() )
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
    return obj->id() == classid();
}

//
// ShrBuiltin
//

ShrBuiltin::ShrBuiltin( const Type::Ptr& type )
: BitBuiltin( "shr", type, info, classid() )
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
    return obj->id() == classid();
}

//
// AshrBuiltin
//

AshrBuiltin::AshrBuiltin( const Type::Ptr& type )
: BitBuiltin( "ashr", type, info, classid() )
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
    return obj->id() == classid();
}

//
// ClzBuiltin
//

ClzBuiltin::ClzBuiltin( const Type::Ptr& type )
: BitBuiltin( "clz", type, info, classid() )
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
    return obj->id() == classid();
}

//
// CloBuiltin
//

CloBuiltin::CloBuiltin( const Type::Ptr& type )
: BitBuiltin( "clo", type, info, classid() )
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
    return obj->id() == classid();
}

//
// ClsBuiltin
//

ClsBuiltin::ClsBuiltin( const Type::Ptr& type )
: BitBuiltin( "cls", type, info, classid() )
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
    return obj->id() == classid();
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

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

#ifndef _LIB_CASMIR_BUILTIN_H_
#define _LIB_CASMIR_BUILTIN_H_

#include "Type.h"
#include "TypeAnnotation.h"
#include "User.h"

namespace libcasm_ir
{
    class Identifier;

    class Builtin : public User, public TypeAnnotation
    {
      private:
        static std::unordered_map< std::string, Builtin* >& str2obj( void )
        {
            static std::unordered_map< std::string, Builtin* > cache;
            return cache;
        };

        const char* m_description;

      public:
        Builtin( const char* name, Type* result, const TypeAnnotation& info,
            Value::ID id = classid() );

        ~Builtin( void );

        const char* description( void );

        static inline Value::ID classid( void )
        {
            return Value::BUILTIN;
        }

        static u1 classof( Value const* obj );

        virtual const char* labelName( void ) override final
        {
            return "@b";
        }

        virtual u64 labelId( void ) override final
        {
            static u64 cnt = 0;
            return cnt++;
        }

        static Builtin* find( const char* name, Type* result );

        static Builtin* asBuiltin( Type* result );

      private:
        template < typename T >
        static Builtin* get( Type* result );
    };

    class CastingBuiltin : public Builtin
    {
      public:
        CastingBuiltin( const char* name, Type* result,
            const TypeAnnotation& info, Value::ID id = Value::CASTING_BUILTIN );

        static inline Value::ID classid( void )
        {
            return Value::CASTING_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AsBooleanBuiltin : public CastingBuiltin
    {
      public:
        AsBooleanBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_BOOLEAN_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsIntegerBuiltin : public CastingBuiltin
    {
      public:
        AsIntegerBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_INTEGER_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsBitBuiltin : public CastingBuiltin
    {
      public:
        AsBitBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_BIT_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsStringBuiltin : public CastingBuiltin
    {
      public:
        AsStringBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_STRING_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsFloatingBuiltin : public CastingBuiltin
    {
      public:
        AsFloatingBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_FLOATING_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsRationalBuiltin : public CastingBuiltin
    {
      public:
        AsRationalBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_RATIONAL_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsEnumerationBuiltin : public CastingBuiltin
    {
      public:
        AsEnumerationBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_ENUMERATION_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    //
    // StringifyBuiltin
    //

    class StringifyBuiltin : public Builtin
    {
      public:
        StringifyBuiltin( const char* name, Type* result,
            Value::ID id = Value::STRINGIFY_BUILTIN );

        static inline Value::ID classid( void )
        {
            return Value::STRINGIFY_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class DecBuiltin : public StringifyBuiltin
    {
      public:
        DecBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::DEC_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class HexBuiltin : public StringifyBuiltin
    {
      public:
        HexBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::HEX_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class OctBuiltin : public StringifyBuiltin
    {
      public:
        OctBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::OCT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class BinBuiltin : public StringifyBuiltin
    {
      public:
        BinBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::BIN_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    //
    // OperatorBuiltin
    //

    class OperatorBuiltin : public Builtin
    {
      public:
        OperatorBuiltin( const char* name, Type* result,
            Value::ID id = Value::OPERATOR_BUILTIN );

        static inline Value::ID classid( void )
        {
            return Value::OPERATOR_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    //
    // ArithmeticBuiltin
    //

    class ArithmeticBuiltin : public OperatorBuiltin
    {
      public:
        ArithmeticBuiltin( const char* name, Type* result,
            Value::ID id = Value::ARITHMETIC_BUILTIN );

        static inline Value::ID classid( void )
        {
            return Value::ARITHMETIC_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AdduBuiltin : public ArithmeticBuiltin
    {
      public:
        AdduBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::ADDU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AddsBuiltin : public ArithmeticBuiltin
    {
      public:
        AddsBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::ADDS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class SubuBuiltin : public ArithmeticBuiltin
    {
      public:
        SubuBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::SUBU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class SubsBuiltin : public ArithmeticBuiltin
    {
      public:
        SubsBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::SUBS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class MuluBuiltin : public ArithmeticBuiltin
    {
      public:
        MuluBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::MULU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class MulsBuiltin : public ArithmeticBuiltin
    {
      public:
        MulsBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::MULS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    //
    // CompareBuiltin
    //

    class CompareBuiltin : public OperatorBuiltin
    {
      public:
        CompareBuiltin( const char* name, Type* result,
            Value::ID id = Value::COMPARE_BUILTIN );

        static inline Value::ID classid( void )
        {
            return Value::COMPARE_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class LesuBuiltin : public CompareBuiltin
    {
      public:
        LesuBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::LESU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class LessBuiltin : public CompareBuiltin
    {
      public:
        LessBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::LESS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class LequBuiltin : public CompareBuiltin
    {
      public:
        LequBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::LEQU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class LeqsBuiltin : public CompareBuiltin
    {
      public:
        LeqsBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::LEQS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class GreuBuiltin : public CompareBuiltin
    {
      public:
        GreuBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::GREU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class GresBuiltin : public CompareBuiltin
    {
      public:
        GresBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::GRES_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class GequBuiltin : public CompareBuiltin
    {
      public:
        GequBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::GEQU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class GeqsBuiltin : public CompareBuiltin
    {
      public:
        GeqsBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::GEQS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    //
    // BitBuiltin
    //

    class BitBuiltin : public Builtin
    {
      public:
        BitBuiltin( const char* name, Type* result, const TypeAnnotation& info,
            Value::ID id = Value::BIT_BUILTIN );

        static inline Value::ID classid( void )
        {
            return Value::BIT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class ZextBuiltin : public BitBuiltin
    {
      public:
        ZextBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::ZEXT_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class SextBuiltin : public BitBuiltin
    {
      public:
        SextBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::SEXT_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class TruncBuiltin : public BitBuiltin
    {
      public:
        TruncBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::TRUNC_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class ShlBuiltin : public BitBuiltin
    {
      public:
        ShlBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::SHL_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class ShrBuiltin : public BitBuiltin
    {
      public:
        ShrBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::SHR_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AshrBuiltin : public BitBuiltin
    {
      public:
        AshrBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::ASHR_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class ClzBuiltin : public BitBuiltin
    {
      public:
        ClzBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::CLZ_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class CloBuiltin : public BitBuiltin
    {
      public:
        CloBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::CLO_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class ClsBuiltin : public BitBuiltin
    {
      public:
        ClsBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::CLS_BUILTIN;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    //
    // MathBuiltin
    //

    // TODO: PPA:

    //
    // ListBuiltin
    //

    // TODO: PPA:
}

#endif /* _LIB_CASMIR_BUILTIN_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

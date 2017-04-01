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

#include "User.h"

#include "Annotation.h"

namespace libcasm_ir
{
    class Builtin : public User
    {
      public:
        using Ptr = std::shared_ptr< Builtin >;

        Builtin( const Type::Ptr& type, const Annotation& info,
            Value::ID id = classid() );

        const Annotation& annotation( void ) const;

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::BUILTIN;
        }

        static u1 classof( Value const* obj );

        static u1 available(
            const std::string& token, std::size_t argumentSize );

        static Builtin::Ptr find(
            const std::string& name, const Type::Ptr& type );

        static Builtin::Ptr asBuiltin( const Type::Ptr& type );

        std::unordered_map< std::string, Builtin::Ptr >& make_cache( void )
        {
            static std::unordered_map< std::string, Builtin::Ptr > cache;
            return cache;
        }

      private:
        const Annotation& m_annotation;
    };

    using Builtins = ValueList< Builtin >;

    class GeneralBuiltin : public Builtin
    {
      public:
        using Ptr = std::shared_ptr< GeneralBuiltin >;

        GeneralBuiltin( const Type::Ptr& type, const Annotation& info,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::GENERAL_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class SelfBuiltin : public GeneralBuiltin
    {
      public:
        using Ptr = std::shared_ptr< SelfBuiltin >;

        SelfBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::SELF_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AssertBuiltin : public GeneralBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AssertBuiltin >;

        AssertBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::ASSERT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class OutputBuiltin : public GeneralBuiltin
    {
      public:
        using Ptr = std::shared_ptr< OutputBuiltin >;

        OutputBuiltin( const Type::Ptr& type, const Annotation& info,
            const std::string& channel, u1 newline, Value::ID id = classid() );

        std::string channel( void ) const;

        u1 newline( void ) const;

      private:
        std::string m_channel;

        u1 m_newline;

      public:
        static inline Value::ID classid( void )
        {
            return Value::OUTPUT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class PrintBuiltin : public OutputBuiltin
    {
      public:
        using Ptr = std::shared_ptr< PrintBuiltin >;

        PrintBuiltin( void );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::PRINT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class PrintLnBuiltin : public OutputBuiltin
    {
      public:
        using Ptr = std::shared_ptr< PrintLnBuiltin >;

        PrintLnBuiltin( void );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::PRINTLN_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class CastingBuiltin : public Builtin
    {
      public:
        using Ptr = std::shared_ptr< CastingBuiltin >;

        CastingBuiltin( const Type::Ptr& type, const Annotation& info,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::CASTING_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AsBooleanBuiltin : public CastingBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AsBooleanBuiltin >;

        AsBooleanBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::AS_BOOLEAN_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AsIntegerBuiltin : public CastingBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AsIntegerBuiltin >;

        AsIntegerBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::AS_INTEGER_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AsBitBuiltin : public CastingBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AsBitBuiltin >;

        AsBitBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::AS_BIT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AsStringBuiltin : public CastingBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AsStringBuiltin >;

        AsStringBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::AS_STRING_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AsFloatingBuiltin : public CastingBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AsFloatingBuiltin >;

        AsFloatingBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::AS_FLOATING_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AsRationalBuiltin : public CastingBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AsRationalBuiltin >;

        AsRationalBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::AS_RATIONAL_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AsEnumerationBuiltin : public CastingBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AsEnumerationBuiltin >;

        AsEnumerationBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::AS_ENUMERATION_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    //
    // StringifyBuiltin
    //

    class StringifyBuiltin : public Builtin
    {
      public:
        using Ptr = std::shared_ptr< StringifyBuiltin >;

        StringifyBuiltin( const Type::Ptr& type, const Annotation& info,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::STRINGIFY_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class DecBuiltin : public StringifyBuiltin
    {
      public:
        using Ptr = std::shared_ptr< DecBuiltin >;

        DecBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::DEC_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class HexBuiltin : public StringifyBuiltin
    {
      public:
        using Ptr = std::shared_ptr< HexBuiltin >;

        HexBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::HEX_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class OctBuiltin : public StringifyBuiltin
    {
      public:
        using Ptr = std::shared_ptr< OctBuiltin >;

        OctBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::OCT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class BinBuiltin : public StringifyBuiltin
    {
      public:
        using Ptr = std::shared_ptr< BinBuiltin >;

        BinBuiltin( const Type::Ptr& type );

        static const Annotation info;

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
        using Ptr = std::shared_ptr< OperatorBuiltin >;

        OperatorBuiltin( const Type::Ptr& type, const Annotation& info,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::OPERATOR_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    //
    // ArithmeticBuiltin
    //

    class ArithmeticBuiltin : public OperatorBuiltin
    {
      public:
        using Ptr = std::shared_ptr< ArithmeticBuiltin >;

        ArithmeticBuiltin( const Type::Ptr& type, const Annotation& info,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::ARITHMETIC_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AdduBuiltin : public ArithmeticBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AdduBuiltin >;

        AdduBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::ADDU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AddsBuiltin : public ArithmeticBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AddsBuiltin >;

        AddsBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::ADDS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class SubuBuiltin : public ArithmeticBuiltin
    {
      public:
        using Ptr = std::shared_ptr< SubuBuiltin >;

        SubuBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::SUBU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class SubsBuiltin : public ArithmeticBuiltin
    {
      public:
        using Ptr = std::shared_ptr< SubsBuiltin >;

        SubsBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::SUBS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class MuluBuiltin : public ArithmeticBuiltin
    {
      public:
        using Ptr = std::shared_ptr< MuluBuiltin >;

        MuluBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::MULU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class MulsBuiltin : public ArithmeticBuiltin
    {
      public:
        using Ptr = std::shared_ptr< MulsBuiltin >;

        MulsBuiltin( const Type::Ptr& type );

        static const Annotation info;

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
        using Ptr = std::shared_ptr< CompareBuiltin >;

        CompareBuiltin( const Type::Ptr& type, const Annotation& info,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::COMPARE_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class LesuBuiltin : public CompareBuiltin
    {
      public:
        using Ptr = std::shared_ptr< LesuBuiltin >;

        LesuBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::LESU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class LessBuiltin : public CompareBuiltin
    {
      public:
        using Ptr = std::shared_ptr< LessBuiltin >;

        LessBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::LESS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class LequBuiltin : public CompareBuiltin
    {
      public:
        using Ptr = std::shared_ptr< LequBuiltin >;

        LequBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::LEQU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class LeqsBuiltin : public CompareBuiltin
    {
      public:
        using Ptr = std::shared_ptr< LeqsBuiltin >;

        LeqsBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::LEQS_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class GreuBuiltin : public CompareBuiltin
    {
      public:
        using Ptr = std::shared_ptr< GreuBuiltin >;

        GreuBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::GREU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class GresBuiltin : public CompareBuiltin
    {
      public:
        using Ptr = std::shared_ptr< GresBuiltin >;

        GresBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::GRES_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class GequBuiltin : public CompareBuiltin
    {
      public:
        using Ptr = std::shared_ptr< GequBuiltin >;

        GequBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::GEQU_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class GeqsBuiltin : public CompareBuiltin
    {
      public:
        using Ptr = std::shared_ptr< GeqsBuiltin >;

        GeqsBuiltin( const Type::Ptr& type );

        static const Annotation info;

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
        using Ptr = std::shared_ptr< BitBuiltin >;

        BitBuiltin( const Type::Ptr& type, const Annotation& info,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::BIT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class ZextBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< ZextBuiltin >;

        ZextBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::ZEXT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class SextBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< SextBuiltin >;

        SextBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::SEXT_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class TruncBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< TruncBuiltin >;

        TruncBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::TRUNC_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class ShlBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< ShlBuiltin >;

        ShlBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::SHL_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class ShrBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< ShrBuiltin >;

        ShrBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::SHR_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class AshrBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< AshrBuiltin >;

        AshrBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::ASHR_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class ClzBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< ClzBuiltin >;

        ClzBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::CLZ_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class CloBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< CloBuiltin >;

        CloBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::CLO_BUILTIN;
        }

        static u1 classof( Value const* obj );
    };

    class ClsBuiltin : public BitBuiltin
    {
      public:
        using Ptr = std::shared_ptr< ClsBuiltin >;

        ClsBuiltin( const Type::Ptr& type );

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::CLS_BUILTIN;
        }

        static u1 classof( Value const* obj );
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

#endif // _LIB_CASMIR_BUILTIN_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

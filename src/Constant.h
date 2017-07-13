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

#ifndef _LIB_CASMIR_CONSTANT_H_
#define _LIB_CASMIR_CONSTANT_H_

#include "Value.h"

#include "Enumeration.h"
#include "Range.h"
#include "Rule.h"

#include "../stdhl/cpp/type/Binary.h"
#include "../stdhl/cpp/type/FloatingPoint.h"
#include "../stdhl/cpp/type/Integer.h"
#include "../stdhl/cpp/type/Layout.h"
#include "../stdhl/cpp/type/Rational.h"

namespace libcasm_ir
{
    class Constant : public Value
    {
      public:
        using Ptr = std::shared_ptr< Constant >;

      protected:
        Constant( const Type::Ptr& type, const libstdhl::Type::Layout& data,
            const Value::Ptr& value, u1 defined, u1 symbolic, Value::ID id );

      public:
        explicit Constant( void );

        virtual ~Constant( void ) = default;

        u1 defined( void ) const;

        u1 symbolic( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const;

        Constant choose( void ) const;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        }

        static u1 classof( Value const* obj );

      protected:
        libstdhl::Type::Layout m_data;

        Value::Ptr m_value;

      private:
        u1 m_defined;

        u1 m_symbolic;

      public:
        std::unordered_map< std::string, Constant::Ptr >& make_cache( void )
        {
            static std::unordered_map< std::string, Constant::Ptr > cache;
            return cache;
        }

        static Constant undef( const Type::Ptr& type );
    };

    using Constants = ValueList< Constant >;

    class VoidConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< VoidConstant >;

        VoidConstant( void );

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::VOID_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class BooleanConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< BooleanConstant >;

      private:
        BooleanConstant( u1 value, u1 defined, u1 symbolic );

      public:
        BooleanConstant( u1 value );
        BooleanConstant( void );

        u1 value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::BOOLEAN_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class IntegerConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< IntegerConstant >;

      private:
        IntegerConstant(
            const libstdhl::Type::Layout& value, u1 defined, u1 symbolic );

      public:
        IntegerConstant( const std::string& value,
            const libstdhl::Type::Radix radix = libstdhl::Type::DECIMAL );
        IntegerConstant( const BitConstant& value );
        IntegerConstant( const libstdhl::Type::Integer& value );
        IntegerConstant( i64 value );
        IntegerConstant( void );

        std::string literal(
            libstdhl::Type::Radix radix = libstdhl::Type::DECIMAL ) const;

        i64 value_i64( void ) const;

        const libstdhl::Type::Integer& value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::INTEGER_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class BitConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< BitConstant >;

      private:
        BitConstant(
            const BitType::Ptr& type, u64 value, u1 defined, u1 symbolic );

        BitConstant( const BitType::Ptr& type, const std::string& value,
            const libstdhl::Type::Radix radix );

      public:
        BitConstant(
            const Type::Ptr& type, const libstdhl::Type::Binary& value );

        BitConstant( const BitType::Ptr& type, u64 value );

        BitConstant( const BitType::Ptr& type );

        BitConstant( const std::string& value,
            const libstdhl::Type::Radix radix = libstdhl::Type::BINARY );

        BitConstant( u16 bitsize, u64 value );

        BitConstant( u16 bitsize );

        std::string literal(
            libstdhl::Type::Radix radix = libstdhl::Type::HEXADECIMAL ) const;

        u64 value_u64( void ) const;

        const libstdhl::Type::Binary& value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::BIT_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class StringConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< StringConstant >;

      private:
        StringConstant( const std::string& value, u1 defined, u1 symbolic );

      public:
        StringConstant( const std::string& value );
        StringConstant( void );

        std::string value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::STRING_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class FloatingConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< FloatingConstant >;

      private:
        FloatingConstant(
            const libstdhl::Type::Layout& value, u1 defined, u1 symbolic );

      public:
        FloatingConstant( const libstdhl::Type::Layout& value );
        FloatingConstant( const std::string& value );
        FloatingConstant( const double value );
        FloatingConstant( void );

        const libstdhl::Type::FloatingPoint& value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::FLOATING_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class RationalConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< RationalConstant >;

      private:
        RationalConstant(
            const libstdhl::Type::Layout& value, u1 defined, u1 symbolic );

      public:
        RationalConstant( const libstdhl::Type::Layout& value );
        RationalConstant( const std::string& value );
        RationalConstant( void );

        const libstdhl::Type::Rational& value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::RATIONAL_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class EnumerationConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< EnumerationConstant >;

      private:
        EnumerationConstant( const EnumerationType::Ptr& type,
            const std::string& value, u1 defined, u1 symbolic,
            Value::ID id = classid() );

      public:
        EnumerationConstant(
            const EnumerationType::Ptr& type, const std::string& value );

        EnumerationConstant( const EnumerationType::Ptr& type );

        EnumerationConstant(
            const Enumeration::Ptr& kind, const std::string& value );

        EnumerationConstant( const Enumeration::Ptr& kind );

        const libstdhl::Type::Binary& value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::ENUMERATION_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class RangeConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< RangeConstant >;

      private:
        RangeConstant( const Type::Ptr& type, const Range::Ptr& value,
            u1 defined, u1 symbolic, Value::ID id = classid() );

      public:
        RangeConstant( const RangeType::Ptr& type, const Range::Ptr& value );

        RangeConstant( const RangeType::Ptr& type );

        RangeConstant(
            const Type::Ptr& type, const Constant& from, const Constant& to );

        Range::Ptr value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const;

        Constant choose( void ) const;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::RANGE_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    /**
       @class ReferenceConstant
     */
    template < typename T >
    class ReferenceConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< ReferenceConstant >;

      protected:
        inline ReferenceConstant( const Type::Ptr& type,
            const typename T::Ptr& value, u1 defined, u1 symbolic,
            Value::ID id )
        : Constant(
              type, libstdhl::Type::Layout(), value, defined, symbolic, id )
        {
        }

      public:
        typename T::Ptr value( void ) const
        {
            return std::static_pointer_cast< T >( m_value );
        }
    };

    class RuleReferenceConstant final : public ReferenceConstant< Rule >
    {
      public:
        using Ptr = std::shared_ptr< RuleReferenceConstant >;

      private:
        RuleReferenceConstant( const Type::Ptr& type, const Rule::Ptr& value,
            u1 defined, u1 symbolic );

      public:
        RuleReferenceConstant( const Rule::Ptr& value );
        RuleReferenceConstant( const Type::Ptr& type );

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::RULE_REFERENCE_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    // class FunctionReferenceConstant final : public ReferenceConstant<
    // Function >
    // {
    //   public:
    //     using Ptr = std::shared_ptr< FunctionReferenceConstant >;

    //   private:
    //     FunctionReferenceConstant( const Type::Ptr& type,
    //         const Rule::Ptr& value, u1 defined, u1 symbolic );

    //   public:
    //     FunctionReferenceConstant( const Rule::Ptr& value );
    //     FunctionReferenceConstant( const Type::Ptr& type );

    //     Rule::Ptr value( void ) const;

    //     std::string name( void ) const override;

    //     void accept( Visitor& visitor ) override;

    //     std::size_t hash( void ) const override;

    //     static inline Value::ID classid( void )
    //     {
    //         return Value::FUNCTION_REFERENCE_CONSTANT;
    //     }

    //     static u1 classof( Value const* obj );
    // };

    class Identifier final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< Identifier >;

        Identifier( const std::string& value, const Type::Ptr& type );

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::IDENTIFIER;
        }

        static u1 classof( Value const* obj );
    };
}

namespace libstdhl
{
    namespace Hash
    {
        inline std::size_t value( const libcasm_ir::Constant& constant )
        {
            return constant.hash();
        }

        inline std::size_t value(
            const std::vector< libcasm_ir::Constant >& constants )
        {
            std::size_t hash = constants.size();

            for( const auto& constant : constants )
            {
                hash = libstdhl::Hash::combine( hash, constant.hash() );
            }

            return hash;
        }
    }
}

#endif // _LIB_CASMIR_CONSTANT_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

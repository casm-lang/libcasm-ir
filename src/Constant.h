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

#include "../stdhl/cpp/Type.h"
#include "../stdhl/cpp/type/Boolean.h"
#include "../stdhl/cpp/type/Data.h"
#include "../stdhl/cpp/type/Floating.h"
#include "../stdhl/cpp/type/Integer.h"
#include "../stdhl/cpp/type/Layout.h"
#include "../stdhl/cpp/type/Natural.h"
#include "../stdhl/cpp/type/Rational.h"
#include "../stdhl/cpp/type/String.h"

namespace libcasm_ir
{
    class Constant : public Value
    {
      public:
        using Ptr = std::shared_ptr< Constant >;

      protected:
        Constant( const Type::Ptr& type,
            const libstdhl::Type::Data& data,
            Value::ID id );

        Constant( const Type::Ptr& type, Value::ID id );

      public:
        explicit Constant( void );

        virtual ~Constant( void );

        Constant( const Constant& other );

        Constant( Constant&& other ) noexcept;

        Constant& operator=( const Constant& other );

        Constant& operator=( Constant&& other ) noexcept;

        u1 defined( void ) const;

        u1 symbolic( void ) const;

        const libstdhl::Type::Data& data( void ) const;

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
        libstdhl::Type::Data m_data;

      public:
        std::unordered_map< std::string, Constant::Ptr >& make_cache( void )
        {
            static std::unordered_map< std::string, Constant::Ptr > cache;
            return cache;
        }

        static Constant undef( const Type::Ptr& type );

        class Registry
        {
        };

        template < typename T >
        static Registry registerConstant( void )
        {
            auto& registeredConstants = m_registeredConstants();

            auto result = registeredConstants.emplace(
                T::classid(), []( const Constant& constant ) -> std::string {
                    return static_cast< const T& >( constant ).name();
                } );

            assert(
                result.second == true && " already registered constant ID " );

            return Registry{};
        }

      private:
        static std::string name( const Constant& constant )
        {
            const auto& registeredConstants = m_registeredConstants();
            const auto result = registeredConstants.find( constant.id() );
            assert( result != registeredConstants.end() );
            return result->second( constant );
        }

        static std::unordered_map< u64,
            const std::function< std::string( const Constant& ) > >&
        m_registeredConstants( void )
        {
            static std::unordered_map< u64,
                const std::function< std::string( const Constant& ) > >
                cache;
            return cache;
        }
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

      public:
        BooleanConstant( const std::string& value );

        BooleanConstant( const libstdhl::Type::Boolean& value );

        BooleanConstant( const u1 value );

        BooleanConstant( void );

        const libstdhl::Type::Boolean& value( void ) const;

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

      public:
        IntegerConstant( const std::string& value,
            const libstdhl::Type::Radix radix = libstdhl::Type::DECIMAL );

        IntegerConstant( const BitConstant& value );

        IntegerConstant( const libstdhl::Type::Integer& value );

        IntegerConstant( const i64 value );

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

      public:
        BitConstant( const std::string& value,
            const libstdhl::Type::Radix radix = libstdhl::Type::BINARY );

        BitConstant(
            const Type::Ptr& type, const libstdhl::Type::Natural& value );

        BitConstant( const BitType::Ptr& type, u64 value );

        BitConstant( const BitType::Ptr& type );

        BitConstant( const u16 bitsize, const u64 value );

        BitConstant( const u16 bitsize );

        std::string literal(
            libstdhl::Type::Radix radix = libstdhl::Type::HEXADECIMAL ) const;

        u64 value_u64( void ) const;

        const libstdhl::Type::Natural& value( void ) const;

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

      public:
        StringConstant( const libstdhl::Type::String& value );

        StringConstant( const std::string& value );

        StringConstant( void );

        const libstdhl::Type::String& value( void ) const;

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

      public:
        FloatingConstant( const libstdhl::Type::Floating& value );

        FloatingConstant( const std::string& value );

        FloatingConstant( const double value );

        FloatingConstant( void );

        const libstdhl::Type::Floating& value( void ) const;

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

      public:
        RationalConstant( const std::string& value );

        RationalConstant( const libstdhl::Type::Rational& value );

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

      public:
        EnumerationConstant(
            const EnumerationType::Ptr& type, const std::string& value );

        EnumerationConstant(
            const Enumeration::Ptr& kind, const std::string& value );

        EnumerationConstant( const EnumerationType::Ptr& type );

        EnumerationConstant( const Enumeration::Ptr& kind );

        const libstdhl::Type::Natural& value( void ) const;

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
        inline ReferenceConstant(
            const Type::Ptr& type, const T* value, Value::ID id )
        : Constant( type, libstdhl::Type::Data( ( u64 )( value ), false ), id )
        {
        }

        inline ReferenceConstant( const Type::Ptr& type, Value::ID id )
        : Constant( type, id )
        {
        }

      public:
        T* value( void ) const
        {
            return (T*)( m_data.value() );
        }
    };

    class RuleReferenceConstant final : public ReferenceConstant< Rule >
    {
      public:
        using Ptr = std::shared_ptr< RuleReferenceConstant >;

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

        Identifier( const Type::Ptr& type, const std::string& value );

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

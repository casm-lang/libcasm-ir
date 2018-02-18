//
//  Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
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
//  Additional permission under GNU GPL version 3 section 7
//
//  libcasm-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcasm-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcasm-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcasm-ir give you permission to link libcasm-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcasm-ir. If you modify libcasm-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#ifndef _LIBCASM_IR_CONSTANT_H_
#define _LIBCASM_IR_CONSTANT_H_

#include <libcasm-ir/Value>

#include <libcasm-ir/Builtin>
#include <libcasm-ir/Derived>
#include <libcasm-ir/Enumeration>
#include <libcasm-ir/Function>
#include <libcasm-ir/List>
#include <libcasm-ir/Range>
#include <libcasm-ir/Rule>
#include <libcasm-ir/Tuple>

#include <libstdhl/type/Boolean>
#include <libstdhl/type/Data>
#include <libstdhl/type/Decimal>
#include <libstdhl/type/Integer>
#include <libstdhl/type/Natural>
#include <libstdhl/type/Rational>
#include <libstdhl/type/String>

#include <cassert>

namespace libcasm_ir
{
    class Constant;

    /**
       @extends CasmIR
    */
    class ConstantHandler
    {
      public:
        virtual u1 name( const Constant& constant, std::string& result ) const = 0;

        virtual u1 foreach(
            const Constant& constant,
            const std::function< void( const Constant& constant ) >& callback ) const = 0;

        virtual u1 choose( const Constant& constant, Constant& result ) const = 0;

        virtual u1 hash( const Constant& constant, std::size_t& result ) const = 0;

        virtual u1 compare( const Constant& lhs, const Value& rhs, u1& result ) const = 0;
    };

    /**
       @extends CasmIR
    */
    class ConstantHandlerManager
    {
      public:
        static ConstantHandlerManager& instance( void )
        {
            static ConstantHandlerManager cache;
            return cache;
        }

      public:
        void registerConstantHandler( std::unique_ptr< ConstantHandler > constantHandler );

        void processConstantHandlers(
            const std::function< u1( const ConstantHandler& ) >& process );

      protected:
        ConstantHandlerManager( void ) = default;

        ConstantHandlerManager( ConstantHandlerManager const& ) = delete;
        ConstantHandlerManager( ConstantHandlerManager&& ) = delete;
        ConstantHandlerManager& operator=( ConstantHandlerManager const& ) = delete;
        ConstantHandlerManager& operator=( ConstantHandlerManager&& ) = delete;

      private:
        std::vector< std::unique_ptr< ConstantHandler > > m_constantHandlers;
    };

    /**

    */
    class Constant : public Value
    {
      public:
        using Ptr = std::shared_ptr< Constant >;

      protected:
        Constant( const Type::Ptr& type, const libstdhl::Type::Data& data, Value::ID id );

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

        void foreach( const std::function< void( const Constant& constant ) >& callback ) const;

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
        std::unordered_map< std::size_t, Constant::Ptr >& cache( void )
        {
            static std::unordered_map< std::size_t, Constant::Ptr > s_cache;
            return s_cache;
        }

        static Constant undef( const Type::Ptr& type );

      private:
        static std::string name( const Constant& constant )
        {
            const auto& registeredConstants = m_registeredConstants();
            const auto result = registeredConstants.find( constant.id() );
            assert( result != registeredConstants.end() );
            return result->second( constant );
        }

        static std::unordered_map< u64, const std::function< std::string( const Constant& ) > >&
        m_registeredConstants( void )
        {
            static std::unordered_map< u64, const std::function< std::string( const Constant& ) > >
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

    class BinaryConstant;

    class IntegerConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< IntegerConstant >;

      public:
        IntegerConstant(
            const std::string& value, const libstdhl::Type::Radix radix = libstdhl::Type::DECIMAL );

        IntegerConstant( const BinaryConstant& value );

        IntegerConstant( const libstdhl::Type::Integer& value );

        IntegerConstant( const i64 value );

        IntegerConstant( void );

        std::string literal( libstdhl::Type::Radix radix = libstdhl::Type::DECIMAL ) const;

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

    class BinaryConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< BinaryConstant >;

      public:
        BinaryConstant(
            const std::string& value, const libstdhl::Type::Radix radix = libstdhl::Type::BINARY );

        BinaryConstant( const Type::Ptr& type, const libstdhl::Type::Natural& value );

        BinaryConstant( const BinaryType::Ptr& type, u64 value );

        BinaryConstant( const BinaryType::Ptr& type );

        BinaryConstant( const u16 bitsize, const u64 value );

        BinaryConstant( const u16 bitsize );

        std::string literal( libstdhl::Type::Radix radix = libstdhl::Type::HEXADECIMAL ) const;

        u64 value_u64( void ) const;

        const libstdhl::Type::Natural& value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::BINARY_CONSTANT;
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

    class DecimalConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< DecimalConstant >;

      public:
        DecimalConstant( const libstdhl::Type::Decimal& value );

        DecimalConstant( const libstdhl::Type::Integer& value );

        DecimalConstant( const libstdhl::Type::Natural& value );

        DecimalConstant( const std::string& value );

        DecimalConstant( const double value );

        DecimalConstant( void );

        const libstdhl::Type::Decimal& value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::DECIMAL_CONSTANT;
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
        EnumerationConstant( const EnumerationType::Ptr& type, const std::string& value );

        EnumerationConstant( const Enumeration::Ptr& kind, const std::string& value );

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

        RangeConstant( const Type::Ptr& type, const Constant& from, const Constant& to );

        Range::Ptr value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        void foreach( const std::function< void( const Constant& constant ) >& callback ) const;

        Constant choose( void ) const;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::RANGE_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class TupleConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< TupleConstant >;

      public:
        TupleConstant( const TupleType::Ptr& type, const std::vector< Constant >& elements );

        TupleConstant( const TupleType::Ptr& type );

        TupleConstant(
            const RecordType::Ptr& type,
            const std::unordered_map< std::string, Constant >& elements );

        TupleConstant( const RecordType::Ptr& type );

        const Tuple* value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        void foreach( const std::function< void( const Constant& constant ) >& callback ) const;

        Constant choose( void ) const;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::TUPLE_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    using RecordConstant = TupleConstant;

    class ListConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< ListConstant >;

      public:
        ListConstant( const ListType::Ptr& type, const List::Ptr& value );

        ListConstant( const ListType::Ptr& type );

        List::Ptr value( void ) const;

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        void foreach( const std::function< void( const Constant& constant ) >& callback ) const;

        Constant choose( void ) const;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::LIST_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class DomainConstant final : public Constant
    {
      public:
        using Ptr = std::shared_ptr< DomainConstant >;

      public:
        DomainConstant( const Type::Ptr& type );

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        void foreach( const std::function< void( const Constant& constant ) >& callback ) const;

        Constant choose( void ) const;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::DOMAIN_CONSTANT;
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
        inline ReferenceConstant( const Type::Ptr& type, const T* value, Value::ID id )
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

    class FunctionReferenceConstant final : public ReferenceConstant< Value >
    {
      public:
        using Ptr = std::shared_ptr< FunctionReferenceConstant >;

      public:
        FunctionReferenceConstant( const Function::Ptr& value );

        FunctionReferenceConstant( const Derived::Ptr& value );

        FunctionReferenceConstant( const Builtin::Ptr& value );

        FunctionReferenceConstant( const Type::Ptr& type );

        std::string name( void ) const override;

        void accept( Visitor& visitor ) override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        static inline Value::ID classid( void )
        {
            return Value::FUNCTION_REFERENCE_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

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

        inline std::size_t value( const std::vector< libcasm_ir::Constant >& constants )
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

#endif  // _LIBCASM_IR_CONSTANT_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

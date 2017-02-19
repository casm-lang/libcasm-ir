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

#include "Agent.h"
#include "Enumeration.h"
#include "Rule.h"
#include "Value.h"

namespace libcasm_ir
{
    class Constant : public Value
    {
      public:
        using Ptr = std::shared_ptr< Constant >;

      protected:
        Constant( const std::string& name, const Type::Ptr& type, u1 defined,
            u1 symbolic, Value::ID id = classid() );

      public:
        u1 defined( void ) const;
        u1 symbolic( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        }

        static u1 classof( Value const* obj );

      private:
        u1 m_defined;
        u1 m_symbolic;

      protected:
        // PPA: this will be replaced later with a std::variant mechanism!
        union {
            u1 m_u1;
            u64 m_u64;
            u64* m_ptr;
            double m_dfp;
        } m_value;

        Value::Ptr m_value_ptr;

      public:
        std::unordered_map< std::string, Constant::Ptr >& make_cache( void )
        {
            static std::unordered_map< std::string, Constant::Ptr > cache;
            return cache;
        }
    };

    class VoidConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< VoidConstant >;

        VoidConstant( void );

        static inline Value::ID classid( void )
        {
            return Value::VOID_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class AgentConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< AgentConstant >;

      private:
        AgentConstant( const Agent::Ptr& value, u1 defined, u1 symbolic );

      public:
        AgentConstant( const Agent::Ptr& value );
        AgentConstant( void );

        Agent::Ptr value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::AGENT_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class RuleReferenceConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< RuleReferenceConstant >;

      private:
        RuleReferenceConstant(
            const Rule::Ptr& value, u1 defined, u1 symbolic );

      public:
        RuleReferenceConstant( const Rule::Ptr& value );
        RuleReferenceConstant( void );

        Rule::Ptr value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::RULE_REFERENCE_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class BooleanConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< BooleanConstant >;

      private:
        BooleanConstant( u1 value, u1 defined, u1 symbolic );

      public:
        BooleanConstant( u1 value );
        BooleanConstant( void );

        u1 value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BOOLEAN_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class IntegerConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< IntegerConstant >;

      private:
        IntegerConstant( i64 value, u1 defined, u1 symbolic );

      public:
        IntegerConstant( i64 value );
        IntegerConstant( void );

        i64 value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::INTEGER_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class BitConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< BitConstant >;

      private:
        BitConstant(
            const BitType::Ptr& type, u64 value, u1 defined, u1 symbolic );

      public:
        BitConstant( const BitType::Ptr& type, u64 value );
        BitConstant( const BitType::Ptr& type );

        BitConstant( u16 bitsize, u64 value );
        BitConstant( u16 bitsize );

        u64 value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BIT_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class StringConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< StringConstant >;

      private:
        StringConstant( const std::string& value, u1 defined, u1 symbolic );

      public:
        StringConstant( const std::string& value );
        StringConstant( void );

        std::string value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRING_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class FloatingConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< FloatingConstant >;

      private:
        FloatingConstant( const double value, u1 defined, u1 symbolic );

      public:
        FloatingConstant( const double value );
        FloatingConstant( void );

        double value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::RATIONAL_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class RationalConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< RationalConstant >;

      private:
        RationalConstant( const std::string& value, u1 defined, u1 symbolic );

      public:
        RationalConstant( const std::string& value );
        RationalConstant( void );

        std::string value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::RATIONAL_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class EnumerationConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< EnumerationConstant >;

      private:
        EnumerationConstant( const EnumerationType::Ptr& type,
            const std::string& value, u1 defined, u1 symbolic );

      public:
        EnumerationConstant(
            const EnumerationType::Ptr& type, const std::string& value );

        EnumerationConstant( const EnumerationType::Ptr& type );

        EnumerationConstant(
            const Enumeration::Ptr& kind, const std::string& value );

        EnumerationConstant( const Enumeration::Ptr& kind );

        u64 value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::ENUMERATION_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    // class Identifier : public Constant
    // {
    //   public:
    //     using Ptr = std::shared_ptr< Identifier >;

    //   private:
    //     Identifier( const Type::Ptr& type, const std::string& value );

    //   public:
    //     ~Identifier( void );

    //     static Identifier* create( const Type::Ptr& type,
    //         const std::string& value, const Value::Ptr& scope = nullptr );

    //     static void forgetSymbol( const std::string& value );

    //     static inline Value::ID classid( void )
    //     {
    //         return Value::IDENTIFIER;
    //     }

    //     static u1 classof( Value const* obj );

    //     static std::unordered_map< std::string, Identifier* >& ident2obj(
    //     void )
    //     {
    //         static std::unordered_map< std::string, Identifier* > cache;
    //         return cache;
    //     }

    // };
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

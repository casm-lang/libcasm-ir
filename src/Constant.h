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

#include "User.h"

#include "../stdhl/cpp/Allocator.h"

namespace libcasm_ir
{
    class Statement;

    class Constant : public User
    {
      private:
        static std::unordered_map< std::string, Value* >& m_str2obj( void )
        {
            static std::unordered_map< std::string, Value* > cache;
            return cache;
        };

      public:
        Constant( const char* name, Type* type, Value::ID id = classid() )
        : User( name, type, id ){};

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        };
        static u1 classof( Value const* obj );

        virtual const char* labelName( void ) override final
        {
            return "@c";
        }

        virtual u64 labelId( void ) override final
        {
            static u64 cnt = 0;
            return cnt++;
        }

        static Value* Undef( Type* result );

        static Value* Agent( Type::AgentTy value );
        static Value* RuleReference( Type::RuleReferenceTy value );
        static Value* RuleReference( const char* value );
        static Value* Boolean( Type::BooleanTy value );
        static Value* Integer( Type::IntegerTy value );
        static Value* Bit( Type* result, u64 value );
        static Value* String( const char* value );
        // static Value* get( void );
    };

    template < typename V >
    class ConstantOf : public Constant
    {
      private:
        V m_value;
        u1 m_defined;
        const char* m_description;

      protected:
        ConstantOf( const char* name, Type* type, V value, u1 defined,
            Value::ID id = Value::CONSTANT )
        : Constant( name, type, id )
        , m_value( value )
        , m_defined( defined )
        , m_description( 0 )
        {
        }

      public:
        ~ConstantOf( void )
        {
        }

        const V value( void ) const
        {
            return m_value;
        }

        const u1 isDefined( void ) const
        {
            return m_defined;
        }

        const u1 isUndef( void ) const
        {
            return not m_defined;
        }

        const char* description( void )
        {
            if( not m_description )
            {
                std::string tmp = "";
                tmp += type().name();
                tmp += " ";
                tmp += name();

                m_description = libstdhl::Allocator::string( tmp );
            }

            return m_description;
        }

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        }

        static u1 classof( Value const* obj )
        {
            return Constant::classof( obj );
        }

      protected:
        void setValue( V val )
        {
            m_value = val;
        }
    };

    class AgentConstant : public ConstantOf< Type::AgentTy >
    {
      public:
        using Ptr = std::shared_ptr< AgentConstant >;

      private:
        AgentConstant( Type::AgentTy value, u1 defined );

      public:
        AgentConstant( Type::AgentTy value );
        AgentConstant( void );

        static inline Value::ID classid( void )
        {
            return Value::AGENT_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class RuleReferenceConstant : public ConstantOf< Type::RuleReferenceTy >
    {
      public:
        using Ptr = std::shared_ptr< RuleReferenceConstant >;

      private:
        const char* m_resolve_identifier;
        RuleReferenceConstant(
            Type::RuleReferenceTy value, const char* name, u1 defined );

      public:
        RuleReferenceConstant( Type::RuleReferenceTy value );
        RuleReferenceConstant( const char* name );
        RuleReferenceConstant( void );

        void setResolveIdentifier( const char* name );
        void resolve( void );

        static void checking( void );

        static inline Value::ID classid( void )
        {
            return Value::RULE_REFERENCE_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class BooleanConstant : public ConstantOf< Type::BooleanTy >
    {
      public:
        using Ptr = std::shared_ptr< BooleanConstant >;

      private:
        BooleanConstant( Type::BooleanTy value, u1 defined );

      public:
        BooleanConstant( Type::BooleanTy value );
        BooleanConstant( void );

        static inline Value::ID classid( void )
        {
            return Value::BOOLEAN_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class IntegerConstant : public ConstantOf< Type::IntegerTy >
    {
      public:
        using Ptr = std::shared_ptr< IntegerConstant >;

      private:
        IntegerConstant( Type::IntegerTy value, u1 defined );

      public:
        IntegerConstant( Type::IntegerTy value );
        IntegerConstant( void );

        static inline Value::ID classid( void )
        {
            return Value::INTEGER_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class BitConstant : public ConstantOf< Type::BitTy >
    {
      public:
        using Ptr = std::shared_ptr< BitConstant >;

      private:
        BitConstant( Type* result, u64 value, u1 defined );

      public:
        BitConstant( Type* result, u64 value );
        BitConstant( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::BIT_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class StringConstant : public ConstantOf< Type::StringTy >
    {
      public:
        using Ptr = std::shared_ptr< StringConstant >;

      private:
        StringConstant( Type::StringTy value, u1 defined );

      public:
        StringConstant( Type::StringTy value );
        StringConstant( const char* value );
        StringConstant( void );

        static inline Value::ID classid( void )
        {
            return Value::STRING_CONSTANT;
        }

        static u1 classof( Value const* obj );
    };

    class Identifier : public ConstantOf< const char* >
    {
      public:
        using Ptr = std::shared_ptr< Identifier >;

      private:
        static std::unordered_map< std::string, Identifier* >& ident2obj( void )
        {
            static std::unordered_map< std::string, Identifier* > cache;
            return cache;
        }

        Identifier( Type* type, const char* value );

      public:
        ~Identifier( void );

        static Identifier* create(
            Type* type, const char* value, Value* scope = 0 );

        static void forgetSymbol( const char* value );

        static inline Value::ID classid( void )
        {
            return Value::IDENTIFIER;
        }

        static u1 classof( Value const* obj );
    };
}

#endif /* _LIB_CASMIR_CONSTANT_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

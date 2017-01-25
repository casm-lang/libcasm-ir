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

namespace libcasm_ir
{
    class Statement;

    class Constant : public User
    {
      private:
        static std::unordered_map< std::string, Value* >& str2obj( void )
        {
            static std::unordered_map< std::string, Value* > cache;
            return cache;
        };

      public:
        Constant( const char* name, Type* type, Value::ID id = Value::CONSTANT )
        : User( name, type, id ){};

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        };
        static bool classof( Value const* obj );

        virtual const char* getLabelName( void ) override final
        {
            return "@c";
        }

        virtual u64 getLabelID( void ) override final
        {
            static u64 cnt = 0;
            return cnt++;
        }

        static Value* getUndef( Type* result );

        static Value* getAgent( Type::Agent value );
        static Value* getRuleReference( Type::RuleReference value );
        static Value* getRuleReference( const char* value );
        static Value* getBoolean( Type::Boolean value );
        static Value* getInteger( Type::Integer value );
        static Value* getBit( Type* result, u64 value );
        static Value* getString( const char* value );
        // static Value* get( void );
    };

    template < typename V >
    class ConstantOf : public Constant
    {
      private:
        V value;
        u1 defined;
        const char* description;

      protected:
        ConstantOf( const char* name, Type* type, V value, u1 defined,
            Value::ID id = Value::CONSTANT )
        : Constant( name, type, id )
        , value( value )
        , defined( defined )
        , description( 0 )
        {
        }

      public:
        ~ConstantOf( void )
        {
        }

        const V getValue( void ) const
        {
            return value;
        }

        const u1 isDefined( void ) const
        {
            return defined;
        }

        const u1 isUndef( void ) const
        {
            return not defined;
        }

        const char* getDescription( void )
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

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        }

        static bool classof( Value const* obj )
        {
            return Constant::classof( obj );
        }

      protected:
        void setValue( V val )
        {
            value = val;
        }
    };

    class AgentConstant : public ConstantOf< Type::Agent >
    {
      private:
        AgentConstant( Type::Agent value, u1 defined );

      public:
        AgentConstant( Type::Agent value );
        AgentConstant( void );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::AGENT_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class RuleReferenceConstant : public ConstantOf< Type::RuleReference >
    {
      private:
        const char* resolve_identifier;
        RuleReferenceConstant(
            Type::RuleReference value, const char* name, u1 defined );

      public:
        RuleReferenceConstant( Type::RuleReference value );
        RuleReferenceConstant( const char* name );
        RuleReferenceConstant( void );

        void setResolveIdentifier( const char* name );
        void resolve( void );

        static void checking( void );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::RULE_REFERENCE_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class BooleanConstant : public ConstantOf< Type::Boolean >
    {
      private:
        BooleanConstant( Type::Boolean value, u1 defined );

      public:
        BooleanConstant( Type::Boolean value );
        BooleanConstant( void );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BOOLEAN_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class IntegerConstant : public ConstantOf< Type::Integer >
    {
      private:
        IntegerConstant( Type::Integer value, u1 defined );

      public:
        IntegerConstant( Type::Integer value );
        IntegerConstant( void );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::INTEGER_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class BitConstant : public ConstantOf< Type::Bit >
    {
      private:
        BitConstant( Type* result, u64 value, u1 defined );

      public:
        BitConstant( Type* result, u64 value );
        BitConstant( Type* result );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BIT_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class StringConstant : public ConstantOf< Type::String >
    {
      private:
        StringConstant( Type::String value, u1 defined );

      public:
        StringConstant( Type::String value );
        StringConstant( const char* value );
        StringConstant( void );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRING_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class Identifier : public ConstantOf< const char* >
    {
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

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::IDENTIFIER;
        };
        static bool classof( Value const* obj );
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

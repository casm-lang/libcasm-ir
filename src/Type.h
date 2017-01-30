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

/**
   @brief    TODO

   TODO
*/

#ifndef _LIB_CASMIR_TYPE_H_
#define _LIB_CASMIR_TYPE_H_

#include "../stdhl/cpp/Type.h"
#include "CasmIR.h"

namespace libcasm_ir
{
    class Rule;

    class Type : public CasmIR
    {
      public:
        using AgentTy = void*;
        using RuleReferenceTy = Rule*;
        using BooleanTy = u1;
        using IntegerTy = i64;
        using BitTy = u64;
        using StringTy = char*;

        enum ID : u8
        {
            _BOTTOM_ = 0,
            LABEL,

            AGENT,
            RULE_REFERENCE,
            BOOLEAN,
            INTEGER,
            BIT,
            STRING,
            FLOATING,
            RATIONAL,
            ENUMERATION,
            RELATION,
            _TOP_
        };

      protected:
        const char* m_name;
        const char* m_description;

      private:
        ID m_id;

      protected:
        static std::unordered_map< std::string, Type* >& m_str2obj( void )
        {
            static std::unordered_map< std::string, Type* > cache;
            return cache;
        }

      public:
        Type( const char* name, const char* description, ID id );
        ~Type() = default;

        const ID id( void ) const;

        virtual const char* name( void ) = 0;
        virtual const char* description( void ) = 0;
        virtual const std::vector< Type* >& arguments( void ) = 0;

        Type* result( void ) const;

        inline u1 operator==( const Type& rhs )
        {
            if( this != &rhs )
            {
                if( strcmp( this->name(), ( (Type&)rhs ).name() ) )
                {
                    return false;
                }
            }
            return true;
        }
        inline u1 operator!=( const Type& rhs )
        {
            return !operator==( rhs );
        }

        u1 isLabel( void ) const;
        u1 isAgent( void ) const;
        u1 isRuleReference( void ) const;
        u1 isBoolean( void ) const;
        u1 isInteger( void ) const;
        u1 isBit( void ) const;
        u1 isString( void ) const;
        u1 isFloating( void ) const;
        u1 isRational( void ) const;
        u1 isEnumeration( void ) const;
        u1 isRelation( void ) const;

        static Type* Label( void );
        static Type* Agent( void );
        static Type* RuleReference( void );
        static Type* Boolean( void );
        static Type* Integer( void );
        static Type* Bit( u16 size );
        static Type* String( void );
        static Type* Floating( void );
        static Type* Rational( void );
        static Type* Enumeration( const char* name );
        static Type* Relation( Type* result, std::vector< Type* > arguments );
    };

    class PrimitiveType : public Type
    {
      public:
        PrimitiveType( const char* name, const char* description, Type::ID id );

        const char* name( void ) override final;
        const char* description( void ) override final;
        const std::vector< Type* >& arguments( void ) override final;
    };

    class LabelType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< LabelType >;

        LabelType();
    };

    class AgentType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< AgentType >;

        AgentType();
    };

    class RuleReferenceType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< RuleReferenceType >;

        RuleReferenceType();
    };

    class BooleanType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BooleanType >;

        BooleanType();
    };

    class IntegerType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< IntegerType >;

        IntegerType();
    };

    class BitType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BitType >;

        static const u16 SizeMax = 64;

      private:
        u16 m_size;

      public:
        BitType( u16 size );

        const u16 bitsize( void ) const;
    };

    class StringType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< StringType >;

        StringType();
    };

    class FloatingType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< FloatingType >;

        FloatingType();
    };

    class RationalType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< RationalType >;

        RationalType();
    };

    class EnumerationType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< EnumerationType >;

      private:
        // PPA: TODO IDENTIFIERs from the Enum!
      public:
        EnumerationType( const char* name );
    };

    class RelationType : public Type
    {
      public:
        using Ptr = std::shared_ptr< RelationType >;

      private:
        Type* m_result;
        std::vector< Type* > m_arguments;

      public:
        RelationType( Type* result, std::vector< Type* > arguments );

        const char* name( void ) override final;
        const char* description( void ) override final;
        const std::vector< Type* >& arguments( void ) override final;

        const Type* result( void ) const;
    };
}

#endif /* _LIB_CASMIR_TYPE_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

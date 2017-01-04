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

#include "CasmIR.h"

namespace libcasm_ir
{
    class Rule;

    class Type : public CasmIR
    {
      public:
        typedef void* Agent;
        typedef Rule* RuleReference;
        typedef u1 Boolean;
        typedef i64 Integer;
        typedef u64* Bit;
        typedef char* String;

        enum ID : u8
        {
            _BOTTOM_ = 0,
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

        // enum STATE : u8
        // {
        //     UNCHANGED,
        //     CHANGED,
        //     LOCKED
        // };

      protected:
        const char* name;
        const char* description;

      private:
        ID id;

        // u64 type_uid_hash;
        // STATE type_state;
        // i16 bitsize;
        // std::string description;
        // std::vector< Type* > parameters;
        // std::vector< Type* > subtypes;

      protected:
        static std::unordered_map< std::string, Type* >& str2obj( void )
        {
            static std::unordered_map< std::string, Type* > cache;
            return cache;
        }

      public:
        Type( const char* name, const char* description, ID id );
        ~Type() = default;

        const ID getID( void ) const;

        virtual const char* getName( void ) = 0;
        virtual const char* getDescription( void ) = 0;

        Type* getResult( void ) const;

        // PPA: add isTYPEetc. CHECKS

        static Type* getAgent( void );
        static Type* getRuleReference( void );
        static Type* getBoolean( void );
        static Type* getInteger( void );
        static Type* getBit( u8 size );
        static Type* getString( void );
        static Type* getFloating( void );
        static Type* getRational( void );
        static Type* getEnumeration( const char* name );
        static Type* getRelation(
            Type* result, std::vector< Type* > arguments );

      private:
        void setID( ID id );
    };

    class PrimitiveType : public Type
    {
      public:
        PrimitiveType( const char* name, const char* description, Type::ID id );

        const char* getName( void ) override final;
        const char* getDescription( void ) override final;
    };

    class AgentType : public PrimitiveType
    {
      public:
        AgentType();
    };

    class RuleReferenceType : public PrimitiveType
    {
      public:
        RuleReferenceType();
    };

    class BooleanType : public PrimitiveType
    {
      public:
        BooleanType();
    };

    class IntegerType : public PrimitiveType
    {
      public:
        IntegerType();
    };

    class BitType : public PrimitiveType
    {
      private:
        u8 size;

      public:
        BitType( u8 size );

        const u8 getSize( void ) const;
    };

    class StringType : public PrimitiveType
    {
      public:
        StringType();
    };

    class FloatingType : public PrimitiveType
    {
      public:
        FloatingType();
    };

    class RationalType : public PrimitiveType
    {
      public:
        RationalType();
    };

    class EnumerationType : public PrimitiveType
    {
      private:
        // PPA: TODO IDENTIFIERs from the Enum!
      public:
        EnumerationType( const char* name );
    };

    class RelationType : public Type
    {
      private:
        Type* result;
        std::vector< Type* > arguments;

      public:
        RelationType( Type* result, std::vector< Type* > arguments );

        const char* getName( void ) override final;
        const char* getDescription( void ) override final;

        const Type* getResult( void ) const;
        const std::vector< Type* >& getArguments( void ) const;
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

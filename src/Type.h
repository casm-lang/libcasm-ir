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

#include "../stdhl/cpp/List.h"
#include "../stdhl/cpp/Log.h"
#include "../stdhl/cpp/Math.h"

namespace libcasm_ir
{
    class Enumeration;
    class IntegerConstant;

    class Type;
    using Types = libstdhl::List< Type >;

    /**
       @extends CasmIR
     */
    class Type : public std::enable_shared_from_this< Type >
    {
      public:
        using Ptr = std::shared_ptr< Type >;

        enum ID : u8
        {
            _BOTTOM_ = 0,

            RANGE,

            VOID,
            LABEL,
            LOCATION,

            BOOLEAN,
            INTEGER,
            BIT,
            STRING,
            FLOATING,
            RATIONAL,
            ENUMERATION,

            RELATION,

            RULE_REFERENCE,
            FUNCTION_REFERENCE,

            _TOP_
        };

        Type( ID id );

        ~Type( void ) = default;

        virtual std::string name( void ) const = 0;

        virtual std::string description( void ) const = 0;

        ID id( void ) const;

        const Type& result( void ) const;

        Type::Ptr ptr_result( void );

        Types arguments( void ) const;

        std::string make_hash( void ) const;

        inline u1 operator==( const Type& rhs ) const
        {
            if( this != &rhs )
            {
                if( this->id() != rhs.id()
                    or this->name().compare( rhs.name() ) )
                {
                    return false;
                }
            }
            return true;
        }

        inline u1 operator!=( const Type& rhs ) const
        {
            return !operator==( rhs );
        }

        u1 isRange( void ) const;
        u1 isVoid( void ) const;
        u1 isLabel( void ) const;
        u1 isLocation( void ) const;
        u1 isBoolean( void ) const;
        u1 isInteger( void ) const;
        u1 isBit( void ) const;
        u1 isString( void ) const;
        u1 isFloating( void ) const;
        u1 isRational( void ) const;
        u1 isEnumeration( void ) const;
        u1 isRelation( void ) const;
        u1 isReference( void ) const;
        u1 isRuleReference( void ) const;
        u1 isFunctionReference( void ) const;

      protected:
        template < typename T >
        inline typename T::Ptr ptr_this( void )
        {
            return std::static_pointer_cast< T >( shared_from_this() );
        }

        template < typename T >
        inline typename T::Ptr ptr_this( void ) const
        {
            return std::static_pointer_cast< T >( shared_from_this() );
        }

        std::string m_name;
        std::string m_description;

        Type::Ptr m_result;
        Types m_arguments;

      private:
        ID m_id;

      public:
        static std::string token( const Type::ID id );

        std::unordered_map< std::string, Type::Ptr >& make_cache( void )
        {
            static std::unordered_map< std::string, Type::Ptr > cache;
            return cache;
        }
    };

    class RangeType final : public Type
    {
      public:
        using Ptr = std::shared_ptr< RangeType >;

        RangeType( const std::shared_ptr< IntegerConstant >& from,
            const std::shared_ptr< IntegerConstant >& to );

        u1 increasing( void ) const;

        std::string name( void ) const override;

        std::string description( void ) const override;

      private:
        std::shared_ptr< IntegerConstant > m_from;
        std::shared_ptr< IntegerConstant > m_to;

        u1 m_increasing;
    };

    class PrimitiveType : public Type
    {
      public:
        PrimitiveType( Type::ID id );
    };

    class VoidType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< VoidType >;

        VoidType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class LabelType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< LabelType >;

        LabelType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class LocationType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< LocationType >;

        LocationType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class BooleanType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BooleanType >;

        BooleanType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class IntegerType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< IntegerType >;

        IntegerType( void );

        IntegerType( const RangeType::Ptr& range );

        u1 constrained( void ) const;

        RangeType::Ptr range( void ) const;

        std::string name( void ) const override;

        std::string description( void ) const override;

      private:
        RangeType::Ptr m_range;
    };

    class BitType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BitType >;

        static const u16 SizeMax = 64;

        BitType( u16 bitsize );

        BitType( const std::shared_ptr< IntegerConstant >& bitsize );

        BitType( const std::string& value, const libstdhl::Type::Radix radix );

        u16 bitsize( void ) const;

        std::string name( void ) const override;

        std::string description( void ) const override;

      private:
        u16 m_bitsize;
    };

    class StringType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< StringType >;

        StringType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class FloatingType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< FloatingType >;

        FloatingType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class RationalType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< RationalType >;

        RationalType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class EnumerationType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< EnumerationType >;

        EnumerationType( const std::shared_ptr< Enumeration >& kind );

        Enumeration& kind( void ) const;

        std::shared_ptr< Enumeration > ptr_kind( void ) const;

        std::string name( void ) const override;

        std::string description( void ) const override;

      private:
        std::shared_ptr< Enumeration > m_kind;
    };

    class RelationType final : public Type
    {
      public:
        using Ptr = std::shared_ptr< RelationType >;

        RelationType( const Type::Ptr& result, const Types& arguments = {} );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class ReferenceType : public Type
    {
      public:
        using Ptr = std::shared_ptr< ReferenceType >;

        ReferenceType( Type::ID id, const RelationType::Ptr& type );

        std::string name( void ) const override;

        std::string description( void ) const override;
    };

    class RuleReferenceType final : public ReferenceType
    {
      public:
        using Ptr = std::shared_ptr< RuleReferenceType >;

        RuleReferenceType( const RelationType::Ptr& type );

        RuleReferenceType( const Type::Ptr& result, const Types& arguments );

        RuleReferenceType( void );
    };

    class FunctionReferenceType final : public ReferenceType
    {
      public:
        using Ptr = std::shared_ptr< FunctionReferenceType >;

        FunctionReferenceType( const RelationType::Ptr& type );

        FunctionReferenceType(
            const Type::Ptr& result, const Types& arguments );
    };
}

#endif // _LIB_CASMIR_TYPE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

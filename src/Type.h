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

/**
   @brief    TODO

   TODO
*/

#ifndef _LIBCASM_IR_TYPE_H_
#define _LIBCASM_IR_TYPE_H_

#include <libcasm-ir/CasmIR>

#include <libstdhl/List>
#include <libstdhl/Log>
#include <libstdhl/Math>

#include <functional>
#include <unordered_map>

namespace libcasm_ir
{
    class Constant;
    class IntegerConstant;

    class Enumeration;
    class Range;
    class RangeType;

    class Type;
    using Types = libstdhl::List< Type >;

    /**
       @extends CasmIR
     */
    class Type : public std::enable_shared_from_this< Type >
    {
      public:
        using Ptr = std::shared_ptr< Type >;

        using ID = u64;

        enum class Kind
        {
            // synthetic
            VOID,
            LABEL,
            LOCATION,
            RELATION,

            // primitive
            BOOLEAN,
            INTEGER,
            BIT,
            STRING,
            FLOATING,
            RATIONAL,

            // composed
            ENUMERATION,
            RANGE,
            TUPLE,
            LIST,

            // reference
            RULE_REFERENCE,
            FUNCTION_REFERENCE,

            // abstraction
            FILE,
            PORT,

            _TOP_,
        };

        Type( Kind kind );

        virtual ~Type( void ) = default;

        virtual std::string name( void ) const = 0;

        virtual std::string description( void ) const = 0;

        virtual void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const = 0;

        virtual Constant choose( void ) const = 0;

        virtual void validate( const Constant& constant ) const = 0;

        Kind kind( void ) const;

        ID id( void );

        const Type& result( void ) const;

        Type::Ptr ptr_result( void ) const;

        Type::Ptr ptr_type( void ) const;

        const Types& arguments( void ) const;

        virtual std::size_t hash( void ) const = 0;

        inline u1 operator==( const Type& rhs ) const
        {
            if( this != &rhs )
            {
                if( this->m_id != rhs.m_id or this->hash() != rhs.hash() )
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

        u1 isSynthetic( void ) const;
        u1 isVoid( void ) const;
        u1 isLabel( void ) const;
        u1 isLocation( void ) const;
        u1 isRelation( void ) const;

        u1 isPrimitive( void ) const;
        u1 isBoolean( void ) const;
        u1 isInteger( void ) const;
        u1 isBit( void ) const;
        u1 isString( void ) const;
        u1 isFloating( void ) const;
        u1 isRational( void ) const;

        u1 isComposed( void ) const;
        u1 isEnumeration( void ) const;
        u1 isRange( void ) const;
        u1 isTuple( void ) const;
        u1 isList( void ) const;

        u1 isReference( void ) const;
        u1 isRuleReference( void ) const;
        u1 isFunctionReference( void ) const;

        u1 isAbstraction( void ) const;
        u1 isFile( void ) const;
        u1 isPort( void ) const;

        std::unordered_map< std::size_t, Type::Ptr >& cache( void )
        {
            static std::unordered_map< std::size_t, Type::Ptr > obj = {};
            return obj;
        }

      protected:
        template < typename T >
        inline typename T::Ptr ptr_this( void )
        {
            return std::static_pointer_cast< T >( shared_from_this() );
        }

        template < typename T >
        inline typename T::Ptr ptr_this( void ) const
        {
            return std::const_pointer_cast< T >( shared_from_this() );
        }

        Type::Ptr m_result;
        Types m_arguments;

      private:
        Kind m_kind;
        ID m_id;

      public:
        static Type::Ptr fromID( const Type::ID id );

        static std::string token( const Type::Kind kind );

      private:
        static std::unordered_map< std::size_t, Type::Ptr >
            s_registered_type_hash2ptr;
        static std::unordered_map< u64, std::size_t > s_registered_type_id2hash;
    };

    class SyntheticType : public Type
    {
      public:
        SyntheticType( Kind kind );
    };

    class VoidType final : public SyntheticType
    {
      public:
        using Ptr = std::shared_ptr< VoidType >;

        VoidType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::VOID;
        }
    };

    class LabelType final : public SyntheticType
    {
      public:
        using Ptr = std::shared_ptr< LabelType >;

        LabelType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::LABEL;
        }
    };

    class LocationType final : public SyntheticType
    {
      public:
        using Ptr = std::shared_ptr< LocationType >;

        LocationType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::LOCATION;
        }
    };

    class RelationType final : public SyntheticType
    {
      public:
        using Ptr = std::shared_ptr< RelationType >;

        RelationType( const Type::Ptr& result, const Types& arguments = {} );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::RELATION;
        }
    };

    class PrimitiveType : public Type
    {
      public:
        PrimitiveType( Kind kind );
    };

    class BooleanType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BooleanType >;

        BooleanType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::BOOLEAN;
        }
    };

    class IntegerType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< IntegerType >;

        IntegerType( void );

        IntegerType( const std::shared_ptr< RangeType >& range );

        u1 constrained( void ) const;

        std::shared_ptr< RangeType > range( void ) const;

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::INTEGER;
        }

      private:
        std::shared_ptr< RangeType > m_range;
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

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::BIT;
        }

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

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::STRING;
        }
    };

    class FloatingType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< FloatingType >;

        FloatingType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::FLOATING;
        }
    };

    class RationalType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< RationalType >;

        RationalType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::RATIONAL;
        }
    };

    class ComposedType : public Type
    {
      public:
        ComposedType( Kind kind );
    };

    class EnumerationType final : public ComposedType
    {
      public:
        using Ptr = std::shared_ptr< EnumerationType >;

        EnumerationType( const std::shared_ptr< Enumeration >& kind );

        Enumeration& kind( void ) const;

        std::shared_ptr< Enumeration > ptr_kind( void ) const;

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::ENUMERATION;
        }

      private:
        std::shared_ptr< Enumeration > m_kind;
    };

    class RangeType final : public ComposedType
    {
      public:
        using Ptr = std::shared_ptr< RangeType >;

        RangeType( const std::shared_ptr< Range >& range );

        RangeType( const Type::Ptr& type );

        Range& range( void ) const;

        std::shared_ptr< Range > ptr_range( void ) const;

        void setRange( const std::shared_ptr< Range >& range );

        Type& type( void ) const;

        Type::Ptr ptr_type( void ) const;

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::RANGE;
        }

      private:
        std::shared_ptr< Range > m_range;
    };

    class TupleType final : public ComposedType
    {
      public:
        using Ptr = std::shared_ptr< TupleType >;

        explicit TupleType( const Types& types );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::TUPLE;
        }
    };

    class ListType final : public ComposedType
    {
      public:
        using Ptr = std::shared_ptr< ListType >;

        ListType( const Type::Ptr& type );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::LIST;
        }
    };

    class ReferenceType : public Type
    {
      public:
        using Ptr = std::shared_ptr< ReferenceType >;

        ReferenceType( Kind kind, const RelationType::Ptr& type );

        Type::Ptr dereference( void ) const;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;
    };

    class RuleReferenceType final : public ReferenceType
    {
      public:
        using Ptr = std::shared_ptr< RuleReferenceType >;

        RuleReferenceType( const RelationType::Ptr& type );

        RuleReferenceType( const Type::Ptr& result, const Types& arguments );

        RuleReferenceType( void );

        std::string name( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::RULE_REFERENCE;
        }
    };

    class FunctionReferenceType final : public ReferenceType
    {
      public:
        using Ptr = std::shared_ptr< FunctionReferenceType >;

        FunctionReferenceType( const RelationType::Ptr& type );

        FunctionReferenceType(
            const Type::Ptr& result, const Types& arguments );

        std::string name( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::FUNCTION_REFERENCE;
        }
    };

    class AbstractionType : public Type
    {
      public:
        AbstractionType( Kind kind );
    };

    class FileType final : public AbstractionType
    {
      public:
        using Ptr = std::shared_ptr< FileType >;

        FileType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::FILE;
        }
    };

    class PortType final : public AbstractionType
    {
      public:
        using Ptr = std::shared_ptr< PortType >;

        PortType( void );

        std::string name( void ) const override;

        std::string description( void ) const override;

        void foreach(
            const std::function< void( const Constant& constant ) >& callback )
            const override;

        Constant choose( void ) const override;

        void validate( const Constant& constant ) const override;

        std::size_t hash( void ) const override;

        static inline Type::Kind classid( void )
        {
            return Type::Kind::PORT;
        }
    };
}

namespace std
{
    static std::string to_string( const libcasm_ir::Type::Kind value )
    {
        return libcasm_ir::Type::token( value );
    };

    template <>
    struct hash< libcasm_ir::Type::Kind >
    {
        inline size_t operator()( const libcasm_ir::Type::Kind value ) const
        {
            return static_cast< size_t >( value );
        }
    };
}

#endif // _LIBCASM_IR_TYPE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

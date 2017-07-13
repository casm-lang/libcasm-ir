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

#ifndef _LIB_CASMIR_VALUE_H_
#define _LIB_CASMIR_VALUE_H_

#include "CasmIR.h"

#include "Type.h"
#include "Visitor.h"

#include "../stdhl/cpp/Hash.h"

namespace libcasm_ir
{
    class Value : public CasmIR, public std::enable_shared_from_this< Value >
    {
      public:
        using Ptr = std::shared_ptr< Value >;

        enum ID : u8
        {
            VALUE = 0,
            VALUE_LIST,
            USER

            ,
            SPECIFICATION,
            AGENT,
            RULE,
            DERIVED,
            FUNCTION,
            ENUMERATION,
            RANGE,

            BLOCK,
            EXECUTION_SEMANTICS_BLOCK,
            PARALLEL_BLOCK,
            SEQUENTIAL_BLOCK

            ,
            STATEMENT,
            TRIVIAL_STATEMENT,
            BRANCH_STATEMENT

            ,
            CONSTANT,
            VOID_CONSTANT,
            RULE_REFERENCE_CONSTANT,
            BOOLEAN_CONSTANT,
            INTEGER_CONSTANT,
            BIT_CONSTANT,
            STRING_CONSTANT,
            FLOATING_CONSTANT,
            RATIONAL_CONSTANT,
            ENUMERATION_CONSTANT,
            RANGE_CONSTANT,
            IDENTIFIER

            ,
            INSTRUCTION,
            UNARY_INSTRUCTION,
            BINARY_INSTRUCTION,

            SELECT_INSTRUCTION,

            SKIP_INSTRUCTION,
            FORK_INSTRUCTION,
            MERGE_INSTRUCTION,
            LOOKUP_INSTRUCTION,
            UPDATE_INSTRUCTION,
            LOCATION_INSTRUCTION,
            CALL_INSTRUCTION,
            LOCAL_INSTRUCTION

            ,
            OPERATOR_INSTRUCTION,
            ARITHMETIC_INSTRUCTION,
            INV_INSTRUCTION,
            ADD_INSTRUCTION,
            SUB_INSTRUCTION,
            MUL_INSTRUCTION,
            DIV_INSTRUCTION,
            POW_INSTRUCTION,
            MOD_INSTRUCTION,

            COMPARE_INSTRUCTION,
            EQU_INSTRUCTION,
            NEQ_INSTRUCTION,
            LTH_INSTRUCTION,
            LEQ_INSTRUCTION,
            GTH_INSTRUCTION,
            GEQ_INSTRUCTION,

            LOGICAL_INSTRUCTION,
            OR_INSTRUCTION,
            XOR_INSTRUCTION,
            AND_INSTRUCTION,
            IMP_INSTRUCTION,
            NOT_INSTRUCTION,

            BUILTIN,

            GENERAL_BUILTIN,
            IS_SYMBOLIC_BUILTIN,
            ABORT_BUILTIN,
            ASSERT_BUILTIN,

            OUTPUT_BUILTIN,
            PRINT_BUILTIN,
            PRINTLN_BUILTIN,

            CASTING_BUILTIN,
            AS_BOOLEAN_BUILTIN,
            AS_INTEGER_BUILTIN,
            AS_BIT_BUILTIN,
            AS_STRING_BUILTIN,
            AS_FLOATING_BUILTIN,
            AS_RATIONAL_BUILTIN,
            AS_ENUMERATION_BUILTIN,

            STRINGIFY_BUILTIN,
            DEC_BUILTIN,
            HEX_BUILTIN,
            OCT_BUILTIN,
            BIN_BUILTIN,

            OPERATOR_BUILTIN,
            ARITHMETIC_BUILTIN,
            ADDU_BUILTIN,
            ADDS_BUILTIN,
            SUBU_BUILTIN,
            SUBS_BUILTIN,
            MULU_BUILTIN,
            MULS_BUILTIN,

            COMPARE_BUILTIN,
            LESU_BUILTIN,
            LESS_BUILTIN,
            LEQU_BUILTIN,
            LEQS_BUILTIN,
            GREU_BUILTIN,
            GRES_BUILTIN,
            GEQU_BUILTIN,
            GEQS_BUILTIN,

            BIT_BUILTIN,
            ZEXT_BUILTIN,
            SEXT_BUILTIN,
            TRUNC_BUILTIN,
            SHL_BUILTIN,
            SHR_BUILTIN,
            ASHR_BUILTIN,
            CLZ_BUILTIN,
            CLO_BUILTIN,
            CLS_BUILTIN,

            // MATH_BUILTIN,
            // POW_BUILTIN,
            // RAND_BUILTIN,

            // LIST_BUILTIN, // TODO: PPA: when List (type)domain is ready
            // NTH_BUILTIN,
            // CONS_BUILTIN,
            // APP_BUILTIN,
            // LEN_BUILTIN,
            // TAIL_BUILTIN,
            // PEEK_BUILTIN,

            _SIZE_
        };

        Value( const Type::Ptr& type, ID id );

        virtual ~Value( void ) = default;

        virtual std::string name( void ) const = 0;

        std::string description( void ) const;

        const Type& type( void ) const;

        Type::Ptr ptr_type( void ) const;

        ID id() const;

        std::string dump( void ) const;

        std::string make_hash( void ) const;

        std::string label( void ) const;

        virtual std::size_t hash( void ) const = 0;

        virtual u1 operator==( const Value& rhs ) const;

        inline u1 operator!=( const Value& rhs ) const
        {
            return !operator==( rhs );
        }

        virtual void iterate(
            const Traversal order, std::function< void( Value& ) > callback )
            final;

        virtual void accept( Visitor& visitor ) = 0;

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

      private:
        Type::Ptr m_type;

        ID m_id;

      public:
        /**
           encodes the Value::ID to a human readable std::string
           this function can be used by front-ends to display the
           e.g. instruction as a operator symbol
         */
        static std::string token( const Value::ID id );

        static inline ID classid( void )
        {
            return Value::VALUE;
        }

        static inline u1 classof( Value const* )
        {
            return true;
        }
    };

    template < typename T >
    class ValueList : public Value, public libstdhl::List< T >
    {
      public:
        using Ptr = std::shared_ptr< ValueList >;

        ValueList( void )
        : Value( libstdhl::get< VoidType >(), Value::VALUE_LIST )
        {
        }

        std::string name( void ) const override final
        {
            std::stringstream s;

            u1 first = true;
            for( const auto& value : *this )
            {
                s << value->name() << ( first ? "" : "," );
                first = false;
            }

            return "[" + s.str() + "]";
        }

        std::size_t hash( void ) const override final
        {
            std::size_t h = Value::VALUE_LIST;

            for( const auto& value : *this )
            {
                h = libstdhl::Hash::combine( h, value->hash() );
            }

            return h;
        }

        u1 operator==( const Value& rhs ) const override final
        {
            if( this == &rhs )
            {
                return true;
            }

            if( not Value::operator==( rhs ) )
            {
                return false;
            }

            const auto& other = static_cast< const ValueList& >( rhs );
            if( this->size() != other.size() )
            {
                return false;
            }

            const auto end = this->end();
            for( auto it1 = this->begin(), it2 = other.begin(); it1 != end;
                 ++it1, ++it2 )
            {
                if( **it1 != **it2 )
                {
                    return false;
                }
            }

            return true;
        }

        void accept( Visitor& visitor ) override final
        {
            for( auto& value : *this )
            {
                value->accept( visitor );
            }
        }
    };

    using Values = ValueList< Value >;
}

#endif // _LIB_CASMIR_VALUE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

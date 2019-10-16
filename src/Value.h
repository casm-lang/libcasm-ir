//
//  Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
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

/**
   @brief    TODO

   TODO
*/

#ifndef _LIBCASM_IR_VALUE_H_
#define _LIBCASM_IR_VALUE_H_

#include <libcasm-ir/CasmIR>
#include <libcasm-ir/Type>
#include <libcasm-ir/Visitor>

#include <libstdhl/Hash>
#include <libstdhl/Memory>
#include <libstdhl/Variadic>

#include <sstream>

namespace libcasm_ir
{
    /**
       @extends CasmIR
     */
    class Value
    : public CasmIR
    , public std::enable_shared_from_this< Value >
    {
      public:
        using Ptr = std::shared_ptr< Value >;

        enum ID : u8
        {
            VALUE = 0,
            VALUE_LIST,
            USER,

            SPECIFICATION,
            AGENT,
            RULE,
            DERIVED,
            FUNCTION,
            ENUMERATION,
            RANGE,
            TUPLE,
            LIST,

            BLOCK,
            EXECUTION_SEMANTICS_BLOCK,
            PARALLEL_BLOCK,
            SEQUENTIAL_BLOCK,

            STATEMENT,
            TRIVIAL_STATEMENT,
            BRANCH_STATEMENT,

            CONSTANT,
            VOID_CONSTANT,
            RULE_REFERENCE_CONSTANT,
            FUNCTION_REFERENCE_CONSTANT,
            BOOLEAN_CONSTANT,
            INTEGER_CONSTANT,
            BINARY_CONSTANT,
            STRING_CONSTANT,
            DECIMAL_CONSTANT,
            RATIONAL_CONSTANT,
            ENUMERATION_CONSTANT,
            RANGE_CONSTANT,
            TUPLE_CONSTANT,
            LIST_CONSTANT,
            DOMAIN_CONSTANT,
            IDENTIFIER,

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
            LOCAL_INSTRUCTION,

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
            ASSURE_BUILTIN,
            SIZE_BUILTIN,
            AT_BUILTIN,

            OUTPUT_BUILTIN,
            PRINT_BUILTIN,
            PRINTLN_BUILTIN,

            CASTING_BUILTIN,
            AS_BOOLEAN_BUILTIN,
            AS_INTEGER_BUILTIN,
            AS_BINARY_BUILTIN,
            AS_STRING_BUILTIN,
            AS_DECIMAL_BUILTIN,
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

            BINARY_BUILTIN,
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

        Value( const Type::Ptr& type, const ID id );

        Value( const Type::ID type, const ID id );

        virtual ~Value( void ) = default;

        virtual std::string name( void ) const = 0;

        std::string description( void ) const;

        const Type& type( void ) const;

        Type::ID typeId( void ) const;

        ID id( void ) const;

        std::string dump( void ) const;

        std::string label( void ) const;

        virtual std::size_t hash( void ) const = 0;

        virtual u1 operator==( const Value& rhs ) const;

        inline u1 operator!=( const Value& rhs ) const
        {
            return !operator==( rhs );
        }

        virtual void iterate(
            const Traversal order, std::function< void( Value& ) > callback ) final;

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
        Type::ID m_type;

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
    class ValueList
    : public Value
    , public libstdhl::List< T >
    {
      public:
        using Ptr = std::shared_ptr< ValueList >;

        ValueList( void )
        : Value( libstdhl::Memory::get< VoidType >(), Value::VALUE_LIST )
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
            for( auto it1 = this->begin(), it2 = other.begin(); it1 != end; ++it1, ++it2 )
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

namespace std
{
    static std::string to_string( const libcasm_ir::Value::ID value )
    {
        return libcasm_ir::Value::token( value );
    };

    template <>
    struct hash< libcasm_ir::Value::ID >
    {
        inline std::size_t operator()( const libcasm_ir::Value::ID value ) const
        {
            return std::hash< std::size_t >()( (std::size_t)value );
        }
    };
}

#endif  // _LIBCASM_IR_VALUE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

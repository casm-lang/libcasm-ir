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

#include "../stdhl/cpp/Labeling.h"

namespace libcasm_ir
{
    class Type;

    class Value : public CasmIR, public libstdhl::Labeling
    {
      public:
        enum ID : u8
        {
            VALUE = 0,
            USER

            ,
            SPECIFICATION,
            AGENT,
            RULE,
            DERIVED,
            FUNCTION,

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
            AGENT_CONSTANT,
            RULE_REFERENCE_CONSTANT,
            BOOLEAN_CONSTANT,
            INTEGER_CONSTANT,
            BIT_CONSTANT,
            STRING_CONSTANT,
            IDENTIFIER

            ,
            INSTRUCTION,
            UNARY_INSTRUCTION,
            BINARY_INSTRUCTION

            ,
            ASSERT_INSTRUCTION,
            SELECT_INSTRUCTION

            ,
            SKIP_INSTRUCTION,
            FORK_INSTRUCTION,
            MERGE_INSTRUCTION,
            LOOKUP_INSTRUCTION,
            UPDATE_INSTRUCTION,
            LOCATION_INSTRUCTION,
            CALL_INSTRUCTION,
            PRINT_INSTRUCTION,
            LOCAL_INSTRUCTION

            ,
            OPERATOR_INSTRUCTION,
            ARITHMETIC_INSTRUCTION,
            ADD_INSTRUCTION,
            SUB_INSTRUCTION,
            MUL_INSTRUCTION,
            DIV_INSTRUCTION,
            RIV_INSTRUCTION,
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
            NOT_INSTRUCTION,

            BUILTIN,
            IS_SYMBOLIC_BUILTIN,

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

            MATH_BUILTIN,
            POW_BUILTIN,
            RAND_BUILTIN,

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

            // LIST_BUILTIN, // TODO: PPA: when List (type)domain is ready
            // NTH_BUILTIN,
            // CONS_BUILTIN,
            // APP_BUILTIN,
            // LEN_BUILTIN,
            // TAIL_BUILTIN,
            // PEEK_BUILTIN,
        };

      private:
        const char* m_hash;
        const char* m_name;
        Type* m_type;
        ID m_id;
        u1 m_type_lock;

      protected:
        static std::unordered_map< u8, std::unordered_set< Value* > >&
        m_id2objs( void )
        {
            static std::unordered_map< u8, std::unordered_set< Value* > > cache;
            return cache;
        }

      public:
        Value( const char* name, Type* type, ID id );

        virtual ~Value();

        const char* name( void ) const;

        Type& type( void ) const;

        u1 hasType( void ) const;

      protected:
        void setType( Type* type );

      public:
        ID id( void ) const;

        const char* c_str( void );

        void dump( void ) const;

        const char* make_hash( void );

        inline u1 operator==( const Value& rhs ) const
        {
            if( this != &rhs )
            {
                if( this->type() != rhs.type()
                    or strcmp( this->name(), rhs.name() ) )
                {
                    return false;
                }
            }
            return true;
        }
        inline u1 operator!=( const Value& rhs ) const
        {
            return !operator==( rhs );
        }

        static inline ID classid( void )
        {
            return Value::VALUE;
        }

        static inline u1 classof( Value const* )
        {
            return true;
        }

        virtual void iterate( Traversal order, Visitor* visitor = nullptr,
            Context* context = nullptr,
            std::function< void( Value&, Context& ) > action
            = []( Value&, Context& ) {} ) final;

        virtual void iterate( Traversal order,
            std::function< void( Value&, Context& ) > action ) final;
    };
}

#endif /* _LIB_CASMIR_VALUE_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

//
//  Copyright (c) 2015-2016 CASM Organization
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

namespace libcasm_ir
{
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

            BUILTIN,
            IS_SYMBOLIC_BUILTIN,

            CASTING_BUILTIN,
            AS_INTEGER_BUILTIN,
            AS_BOOLEAN_BUILTIN,
            AS_FLOATING_BUILTIN,
            AS_BIT_BUILTIN,
            AS_ENUMERATION_BUILTIN,
            AS_STRING_BUILTIN,
            AS_RATIONAL_BUILTIN,

            STRINGIFY_BUILTIN,
            DEC_BUILTIN,
            HEX_BUILTIN,
            BIN_BUILTIN,

            MATH_BUILTIN,
            POW_BUILTIN,
            RAND_BUILTIN,

            BIT_OPERATION_BUILTIN,
            ZEXT_BUILTIN,
            SEXT_BUILTIN,
            TRUNC_BUILTIN,
            SHL_BUILTIN,
            SHR_BUILTIN,
            ASHR_BUILTIN,
            CLZ_BUILTIN,
            CLO_BUILTIN,
            CLS_BUILTIN,
            LESU_BUILTIN,
            LESS_BUILTIN,
            LEQU_BUILTIN,
            LEQS_BUILTIN,
            GREU_BUILTIN,
            GRES_BUILTIN,
            GEQU_BUILTIN,
            GEQS_BUILTIN,

            // LIST_BUILTIN, // TODO: PPA: when List (type)domain is ready
            // NTH_BUILTIN,
            // CONS_BUILTIN,
            // APP_BUILTIN,
            // LEN_BUILTIN,
            // TAIL_BUILTIN,
            // PEEK_BUILTIN,

            BLOCK,
            EXECUTION_SEMANTICS_BLOCK,
            PARALLEL_BLOCK,
            SEQUENTIAL_BLOCK

            ,
            STATEMENT,
            TRIVIAL_STATEMENT,
            BRANCH_STATEMENT

            ,
            CONSTANT
            // , UNDEF_CONSTANT
            // , SELF_CONSTANT
            ,
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
            SWITCH_INSTRUCTION

            ,
            SKIP_INSTRUCTION,
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
            NOT_INSTRUCTION

            // MOV_INSTRUCTION
        };

        typedef std::unordered_map< const char*, std::unordered_set< Value* >,
            libstdhl::Hash, libstdhl::Equal >
            SymbolTable;

      private:
        const char* name;
        Type* type;
        ID id;
        u1 type_lock;

        std::vector< Type* > parameters;

      protected:
        static SymbolTable& getSymbols( void )
        {
            static SymbolTable symbols;
            return symbols;
        }

      public:
        Value( const char* name, Type* type, ID id );

        virtual ~Value();

        const char* getName( void ) const;

        Type* getType( void ) const;

      protected:
        void setType( Type* type );

      public:
        ID getValueID() const;

        void debug( void ) const;
        void dump( void ) const;

        // TODO: FIXME: PPA: setRef/getRef interface will be added soon!

        static inline ID classid( void )
        {
            return Value::VALUE;
        };

        static inline bool classof( Value const* )
        {
            return true;
        }

        template < class TO >
        static inline bool isa( Value* value )
        {
            return TO::classof( value );
        }

        template < class TO >
        static inline bool isa( const Value* value )
        {
            return isa< TO >( (Value*)value );
        }

        virtual void iterate( Traversal order, Visitor* visitor = 0,
            std::function< void( Value* ) > action = []( Value* ) {} ) final;

        virtual void iterate(
            Traversal order, std::function< void( Value* ) > action ) final;

        virtual u64 getLabelID( void )
        {
            static u64 cnt = -1;
            cnt++;
            return cnt;
        }
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

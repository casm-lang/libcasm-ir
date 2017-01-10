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

#include "Visitor.h"

#include "Builtin.h"
#include "Value.h"

using namespace libcasm_ir;

#define CASE_VALUE( VID, CLASS )                                               \
    case Value::ID::VID:                                                       \
        if( stage == Stage::PROLOG )                                           \
            visit_prolog( *( (CLASS*)value ) );                                \
        else if( stage == Stage::EPILOG )                                      \
            visit_epilog( *( (CLASS*)value ) );                                \
        else                                                                   \
            assert( !"invalid visitor stage value!" );                         \
        break

#define CASE_VALUE_INTER( VID, CLASS )                                         \
    case Value::ID::VID:                                                       \
        if( stage == Stage::PROLOG )                                           \
            visit_prolog( *( (CLASS*)value ) );                                \
        else if( stage == Stage::INTERLOG )                                    \
            visit_interlog( *( (CLASS*)value ) );                              \
        else if( stage == Stage::EPILOG )                                      \
            visit_epilog( *( (CLASS*)value ) );                                \
        else                                                                   \
            assert( !"invalid visitor stage value!" );                         \
        break

void Visitor::dispatch( Stage stage, Value* value )
{
    assert( value );

    switch( value->getValueID() )
    {
        CASE_VALUE( SPECIFICATION, Specification );
        CASE_VALUE( AGENT, Agent );

        CASE_VALUE( FUNCTION, Function );
        CASE_VALUE_INTER( DERIVED, Derived );
        CASE_VALUE_INTER( RULE, Rule );

        CASE_VALUE( PARALLEL_BLOCK, ParallelBlock );
        CASE_VALUE( SEQUENTIAL_BLOCK, SequentialBlock );

        CASE_VALUE( TRIVIAL_STATEMENT, TrivialStatement );
        CASE_VALUE_INTER( BRANCH_STATEMENT, BranchStatement );

        CASE_VALUE( LOCAL_INSTRUCTION, LocalInstruction );

        CASE_VALUE( ASSERT_INSTRUCTION, AssertInstruction );
        CASE_VALUE( SELECT_INSTRUCTION, SelectInstruction );

        CASE_VALUE( SKIP_INSTRUCTION, SkipInstruction );
        CASE_VALUE( FORK_INSTRUCTION, ForkInstruction );
        CASE_VALUE( MERGE_INSTRUCTION, MergeInstruction );

        CASE_VALUE( LOCATION_INSTRUCTION, LocationInstruction );
        CASE_VALUE( LOOKUP_INSTRUCTION, LookupInstruction );
        CASE_VALUE( UPDATE_INSTRUCTION, UpdateInstruction );

        CASE_VALUE( CALL_INSTRUCTION, CallInstruction );
        CASE_VALUE( PRINT_INSTRUCTION, PrintInstruction );

        CASE_VALUE( ADD_INSTRUCTION, AddInstruction );
        CASE_VALUE( SUB_INSTRUCTION, SubInstruction );
        CASE_VALUE( MUL_INSTRUCTION, MulInstruction );
        CASE_VALUE( MOD_INSTRUCTION, ModInstruction );
        CASE_VALUE( DIV_INSTRUCTION, DivInstruction );

        CASE_VALUE( AND_INSTRUCTION, AndInstruction );
        CASE_VALUE( XOR_INSTRUCTION, XorInstruction );
        CASE_VALUE( OR_INSTRUCTION, OrInstruction );

        CASE_VALUE( EQU_INSTRUCTION, EquInstruction );
        CASE_VALUE( NEQ_INSTRUCTION, NeqInstruction );
        CASE_VALUE( LTH_INSTRUCTION, LthInstruction );

        CASE_VALUE( AGENT_CONSTANT, AgentConstant );
        CASE_VALUE( RULE_REFERENCE_CONSTANT, RuleReferenceConstant );
        CASE_VALUE( BOOLEAN_CONSTANT, BooleanConstant );
        CASE_VALUE( INTEGER_CONSTANT, IntegerConstant );
        CASE_VALUE( BIT_CONSTANT, BitConstant );
        CASE_VALUE( STRING_CONSTANT, StringConstant );

        default:
        {
            if( isa< Builtin >( value ) )
            {
                if( stage == Stage::PROLOG )
                {
                    visit_prolog( *( (Builtin*)value ) );
                }
                else if( stage == Stage::EPILOG )
                {
                    visit_epilog( *( (Builtin*)value ) );
                }
                else
                {
                    assert( !"invalid visitor stage value!" );
                }
                break;
            }

            fprintf( stderr,
                "%s:%i: error: unimplemented value name '%s' with id '%i' to "
                "dispatch\n",
                __FILE__, __LINE__, value->getName(), value->getValueID() );
            assert( 0 );
            break;
        }
    }
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

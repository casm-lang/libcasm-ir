//
//  Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
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

#include "main.h"

using namespace libcasm_ir;

TEST( libcasm_ir_Value, id_to_token_string )
{
    for( u64 c = 0; c < Value::_SIZE_; c++ )
    {
        Value::ID id = (Value::ID)c;

        switch( id )
        {
            case Value::VALUE:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Value" );
                break;
            }
            case Value::VALUE_LIST:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "ValueList" );
                break;
            }
            case Value::USER:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "User" );
                break;
            }

            case Value::SPECIFICATION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Specification" );
                break;
            }
            case Value::AGENT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Agent" );
                break;
            }
            case Value::RULE:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Rule" );
                break;
            }
            case Value::DERIVED:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Derived" );
                break;
            }
            case Value::FUNCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Function" );
                break;
            }
            case Value::ENUMERATION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Enumeration" );
                break;
            }
            case Value::RANGE:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Range" );
                break;
            }
            case Value::TUPLE:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Tuple" );
                break;
            }
            case Value::LIST:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "List" );
                break;
            }

            case Value::BLOCK:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Block" );
                break;
            }
            case Value::EXECUTION_SEMANTICS_BLOCK:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "ExecutionSemanticsBlock" );
                break;
            }
            case Value::PARALLEL_BLOCK:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "ParallelBlock" );
                break;
            }
            case Value::SEQUENTIAL_BLOCK:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "SequentialBlock" );
                break;
            }
            case Value::STATEMENT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Statement" );
                break;
            }
            case Value::TRIVIAL_STATEMENT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "TrivialStatement" );
                break;
            }
            case Value::BRANCH_STATEMENT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "BranchStatement" );
                break;
            }

            case Value::CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Constant" );
                break;
            }
            case Value::VOID_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "VoidConstant" );
                break;
            }
            case Value::RULE_REFERENCE_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "RuleReferenceConstant" );
                break;
            }
            case Value::FUNCTION_REFERENCE_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "FunctionReferenceConstant" );
                break;
            }
            case Value::BOOLEAN_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "BooleanConstant" );
                break;
            }
            case Value::INTEGER_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "IntegerConstant" );
                break;
            }
            case Value::BINARY_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "BinaryConstant" );
                break;
            }
            case Value::STRING_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "StringConstant" );
                break;
            }
            case Value::DECIMAL_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "DecimalConstant" );
                break;
            }
            case Value::RATIONAL_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "RationalConstant" );
                break;
            }
            case Value::ENUMERATION_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "EnumerationConstant" );
                break;
            }
            case Value::RANGE_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "RangeConstant" );
                break;
            }
            case Value::TUPLE_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "TupleConstant" );
                break;
            }
            case Value::LIST_CONSTANT:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "ListConstant" );
                break;
            }
            case Value::IDENTIFIER:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Identifier" );
                break;
            }

            case Value::INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Instruction" );
                break;
            }
            case Value::UNARY_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "UnaryInstruction" );
                break;
            }
            case Value::BINARY_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "BinaryInstruction" );
                break;
            }

            case Value::SELECT_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "select" );
                break;
            }
            case Value::SKIP_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "skip" );
                break;
            }
            case Value::FORK_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "fork" );
                break;
            }
            case Value::MERGE_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "merge" );
                break;
            }
            case Value::LOOKUP_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "lookup" );
                break;
            }
            case Value::UPDATE_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "update" );
                break;
            }
            case Value::LOCATION_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "location" );
                break;
            }
            case Value::CALL_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "call" );
                break;
            }
            case Value::LOCAL_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "local" );
                break;
            }

            case Value::OPERATOR_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "OperatorInstruction" );
                break;
            }

            case Value::ARITHMETIC_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "ArithmethicInstruction" );
                break;
            }
            case Value::INV_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), " -" );
                break;
            }
            case Value::ADD_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "+" );
                break;
            }
            case Value::SUB_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "-" );
                break;
            }
            case Value::MUL_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "*" );
                break;
            }
            case Value::DIV_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "/" );
                break;
            }
            case Value::POW_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "^" );
                break;
            }
            case Value::MOD_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "%" );
                break;
            }

            case Value::COMPARE_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "CompareInstruction" );
                break;
            }
            case Value::EQU_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "=" );
                break;
            }
            case Value::NEQ_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "!=" );
                break;
            }
            case Value::LTH_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "<" );
                break;
            }
            case Value::LEQ_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "<=" );
                break;
            }
            case Value::GTH_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), ">" );
                break;
            }
            case Value::GEQ_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), ">=" );
                break;
            }

            case Value::LOGICAL_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "LogicalInstruction" );
                break;
            }
            case Value::OR_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "or" );
                break;
            }
            case Value::IMP_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "imp" );
                break;
            }
            case Value::XOR_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "xor" );
                break;
            }
            case Value::AND_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "and" );
                break;
            }
            case Value::NOT_INSTRUCTION:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "not" );
                break;
            }

            case Value::BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "Builtin" );
                break;
            }
            case Value::GENERAL_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "GeneralBuiltin" );
                break;
            }
            case Value::IS_SYMBOLIC_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "isSymbolic" );
                break;
            }
            case Value::ABORT_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "abort" );
                break;
            }
            case Value::ASSERT_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "assert" );
                break;
            }
            case Value::ASSURE_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "assure" );
                break;
            }

            case Value::OUTPUT_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "OutputBuiltin" );
                break;
            }
            case Value::PRINT_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "print" );
                break;
            }
            case Value::PRINTLN_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "println" );
                break;
            }

            case Value::CASTING_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "CastingBuiltin" );
                break;
            }
            case Value::AS_BOOLEAN_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "asBoolean" );
                break;
            }
            case Value::AS_INTEGER_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "asInteger" );
                break;
            }
            case Value::AS_BINARY_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "asBinary" );
                break;
            }
            case Value::AS_STRING_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "asString" );
                break;
            }
            case Value::AS_DECIMAL_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "asDecimal" );
                break;
            }
            case Value::AS_RATIONAL_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "asRational" );
                break;
            }
            case Value::AS_ENUMERATION_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "as<Enumeration>" );
                break;
            }

            case Value::STRINGIFY_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "StringifyBuiltin" );
                break;
            }
            case Value::DEC_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "dec" );
                break;
            }
            case Value::HEX_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "hex" );
                break;
            }
            case Value::OCT_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "oct" );
                break;
            }
            case Value::BIN_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "bin" );
                break;
            }

            case Value::OPERATOR_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "OperatorBuiltin" );
                break;
            }
            case Value::ARITHMETIC_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "ArithmethicBuiltin" );
                break;
            }
            case Value::ADDU_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "addu" );
                break;
            }
            case Value::ADDS_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "adds" );
                break;
            }
            case Value::SUBU_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "subu" );
                break;
            }
            case Value::SUBS_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "subs" );
                break;
            }
            case Value::MULU_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "mulu" );
                break;
            }
            case Value::MULS_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "muls" );
                break;
            }

            case Value::COMPARE_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "CompareBuiltin" );
                break;
            }
            case Value::LESU_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "lesu" );
                break;
            }
            case Value::LESS_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "less" );
                break;
            }
            case Value::LEQU_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "lequ" );
                break;
            }
            case Value::LEQS_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "leqs" );
                break;
            }
            case Value::GREU_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "greu" );
                break;
            }
            case Value::GRES_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "gres" );
                break;
            }
            case Value::GEQU_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "gequ" );
                break;
            }
            case Value::GEQS_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "geqs" );
                break;
            }

            case Value::BINARY_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "BinaryBuiltin" );
                break;
            }
            case Value::ZEXT_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "zext" );
                break;
            }
            case Value::SEXT_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "sext" );
                break;
            }
            case Value::TRUNC_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "trunc" );
                break;
            }
            case Value::SHL_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "shl" );
                break;
            }
            case Value::SHR_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "shr" );
                break;
            }
            case Value::ASHR_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "ashr" );
                break;
            }
            case Value::CLZ_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "clz" );
                break;
            }
            case Value::CLO_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "clo" );
                break;
            }
            case Value::CLS_BUILTIN:
            {
                EXPECT_STREQ( Value::token( id ).c_str(), "cls" );
                break;
            }

                // case Value::MATH_BUILTIN:
                // {
                //     EXPECT_STREQ( Value::token( id ).c_str(), "MathBuiltin" );
                //     break;
                // }
                // case Value::POW_BUILTIN:
                // {
                //     EXPECT_STREQ( Value::token( id ).c_str(), "pow" );
                //     break;
                // }
                // case Value::RAND_BUILTIN:
                // {
                //     EXPECT_STREQ( Value::token( id ).c_str(), "rand" );
                //     break;
                // }

            case Value::_SIZE_:
            {
                std::string s = "_SIZE_ = " + std::to_string( Value::_SIZE_ );
                EXPECT_STREQ( Value::token( id ).c_str(), s.c_str() );
                break;
            }
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

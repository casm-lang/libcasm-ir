//
//  Copyright (C) 2015-2023 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcasm-ir/graphs/contributors>
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

#include "Operation.h"

#include <libcasm-ir/Builtin>
#include <libcasm-ir/Constant>
#include <libcasm-ir/Exception>
#include <libcasm-ir/Instruction>

using namespace libcasm_ir;

void Operation::execute(
    const Value::ID id,
    const Type::Ptr& type,
    Constant& res,
    const Constant* reg,
    const std::size_t size )
{
    switch( id )
    {
        case Value::VALUE:                        // [[fallthrough]]
        case Value::VALUE_LIST:                   // [[fallthrough]]
        case Value::USER:                         // [[fallthrough]]
        case Value::SPECIFICATION:                // [[fallthrough]]
        case Value::AGENT:                        // [[fallthrough]]
        case Value::RULE:                         // [[fallthrough]]
        case Value::DERIVED:                      // [[fallthrough]]
        case Value::FUNCTION:                     // [[fallthrough]]
        case Value::ENUMERATION:                  // [[fallthrough]]
        case Value::RANGE:                        // [[fallthrough]]
        case Value::LIST:                         // [[fallthrough]]
        case Value::BLOCK:                        // [[fallthrough]]
        case Value::EXECUTION_SEMANTICS_BLOCK:    // [[fallthrough]]
        case Value::PARALLEL_BLOCK:               // [[fallthrough]]
        case Value::SEQUENTIAL_BLOCK:             // [[fallthrough]]
        case Value::STATEMENT:                    // [[fallthrough]]
        case Value::TRIVIAL_STATEMENT:            // [[fallthrough]]
        case Value::BRANCH_STATEMENT:             // [[fallthrough]]
        case Value::CONSTANT:                     // [[fallthrough]]
        case Value::VOID_CONSTANT:                // [[fallthrough]]
        case Value::RULE_REFERENCE_CONSTANT:      // [[fallthrough]]
        case Value::FUNCTION_REFERENCE_CONSTANT:  // [[fallthrough]]
        case Value::BOOLEAN_CONSTANT:             // [[fallthrough]]
        case Value::INTEGER_CONSTANT:             // [[fallthrough]]
        case Value::BINARY_CONSTANT:              // [[fallthrough]]
        case Value::STRING_CONSTANT:              // [[fallthrough]]
        case Value::DECIMAL_CONSTANT:             // [[fallthrough]]
        case Value::RATIONAL_CONSTANT:            // [[fallthrough]]
        case Value::ENUMERATION_CONSTANT:         // [[fallthrough]]
        case Value::RANGE_CONSTANT:               // [[fallthrough]]
        case Value::TUPLE_CONSTANT:               // [[fallthrough]]
        case Value::LIST_CONSTANT:                // [[fallthrough]]
        case Value::DOMAIN_CONSTANT:              // [[fallthrough]]
        case Value::SYMBOLIC_CONSTANT:            // [[fallthrough]]
        case Value::IDENTIFIER:                   // [[fallthrough]]
        case Value::INSTRUCTION:                  // [[fallthrough]]
        case Value::UNARY_INSTRUCTION:            // [[fallthrough]]
        case Value::BINARY_INSTRUCTION:           // [[fallthrough]]
        case Value::SELECT_INSTRUCTION:           // [[fallthrough]]
        case Value::SKIP_INSTRUCTION:             // [[fallthrough]]
        case Value::FORK_INSTRUCTION:             // [[fallthrough]]
        case Value::MERGE_INSTRUCTION:            // [[fallthrough]]
        case Value::LOOKUP_INSTRUCTION:           // [[fallthrough]]
        case Value::UPDATE_INSTRUCTION:           // [[fallthrough]]
        case Value::LOCATION_INSTRUCTION:         // [[fallthrough]]
        case Value::CALL_INSTRUCTION:             // [[fallthrough]]
        case Value::LOCAL_INSTRUCTION:
        {
            break;
        }
        case Value::SELF_INSTRUCTION:
        {
            execute< SelfInstruction >( type, res, reg[ 0 ] );
            return;
        }
        case Value::OPERATOR_INSTRUCTION:  // [[fallthrough]]
        case Value::ARITHMETIC_INSTRUCTION:
        {
            break;
        }
        case Value::INV_INSTRUCTION:
        {
            execute< InvInstruction >( type, res, reg[ 0 ] );
            return;
        }
        case Value::ADD_INSTRUCTION:
        {
            execute< AddInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::SUB_INSTRUCTION:
        {
            execute< SubInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::MUL_INSTRUCTION:
        {
            execute< MulInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::DIV_INSTRUCTION:
        {
            execute< DivInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::POW_INSTRUCTION:
        {
            execute< PowInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::MOD_INSTRUCTION:
        {
            execute< ModInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::COMPARE_INSTRUCTION:
        {
            break;
        }
        case Value::EQU_INSTRUCTION:
        {
            execute< EquInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::NEQ_INSTRUCTION:
        {
            execute< NeqInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::LTH_INSTRUCTION:
        {
            execute< LthInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::LEQ_INSTRUCTION:
        {
            execute< LeqInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::GTH_INSTRUCTION:
        {
            execute< GthInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::GEQ_INSTRUCTION:
        {
            execute< GeqInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::LOGICAL_INSTRUCTION:
        {
            break;
        }
        case Value::OR_INSTRUCTION:
        {
            execute< OrInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::XOR_INSTRUCTION:
        {
            execute< XorInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::AND_INSTRUCTION:
        {
            execute< AndInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::IMP_INSTRUCTION:
        {
            execute< ImpInstruction >( type, res, reg[ 0 ], reg[ 1 ] );
            return;
        }
        case Value::NOT_INSTRUCTION:
        {
            execute< NotInstruction >( type, res, reg[ 0 ] );
            return;
        }
        case Value::BUILTIN:
        case Value::GENERAL_BUILTIN:
        case Value::IS_SYMBOLIC_BUILTIN:
        {
            execute< IsSymbolicBuiltin >( type, res, reg, size );
            return;
        }
        case Value::ABORT_BUILTIN:
        {
            execute< AbortBuiltin >( type, res, reg, size );
            return;
        }
        case Value::ASSERT_BUILTIN:
        {
            execute< AssertBuiltin >( type, res, reg, size );
            return;
        }
        case Value::ASSURE_BUILTIN:
        {
            execute< AssureBuiltin >( type, res, reg, size );
            return;
        }
        case Value::SIZE_BUILTIN:
        {
            execute< SizeBuiltin >( type, res, reg, size );
            return;
        }
        case Value::AT_BUILTIN:
        {
            execute< AtBuiltin >( type, res, reg, size );
            return;
        }
        case Value::OUTPUT_BUILTIN:
        case Value::PRINT_BUILTIN:
        {
            execute< PrintBuiltin >( type, res, reg, size );
            return;
        }
        case Value::PRINTLN_BUILTIN:
        {
            execute< PrintLnBuiltin >( type, res, reg, size );
            return;
        }
        case Value::CASTING_BUILTIN:
        case Value::AS_BOOLEAN_BUILTIN:
        {
            execute< AsBooleanBuiltin >( type, res, reg, size );
            return;
        }
        case Value::AS_INTEGER_BUILTIN:
        {
            execute< AsIntegerBuiltin >( type, res, reg, size );
            return;
        }
        case Value::AS_BINARY_BUILTIN:
        {
            execute< AsBinaryBuiltin >( type, res, reg, size );
            return;
        }
        case Value::AS_STRING_BUILTIN:
        {
            execute< AsStringBuiltin >( type, res, reg, size );
            return;
        }
        case Value::AS_DECIMAL_BUILTIN:
        {
            execute< AsDecimalBuiltin >( type, res, reg, size );
            return;
        }
        case Value::AS_RATIONAL_BUILTIN:
        {
            execute< AsRationalBuiltin >( type, res, reg, size );
            return;
        }
        case Value::AS_ENUMERATION_BUILTIN:
        {
            execute< AsEnumerationBuiltin >( type, res, reg, size );
            return;
        }
        case Value::STRINGIFY_BUILTIN:
        case Value::DEC_BUILTIN:
        {
            execute< DecBuiltin >( type, res, reg, size );
            return;
        }
        case Value::HEX_BUILTIN:
        {
            execute< HexBuiltin >( type, res, reg, size );
            return;
        }
        case Value::OCT_BUILTIN:
        {
            execute< OctBuiltin >( type, res, reg, size );
            return;
        }
        case Value::BIN_BUILTIN:
        {
            execute< BinBuiltin >( type, res, reg, size );
            return;
        }
        case Value::OPERATOR_BUILTIN:
        case Value::ARITHMETIC_BUILTIN:
        case Value::ADDU_BUILTIN:
        {
            execute< AdduBuiltin >( type, res, reg, size );
            return;
        }
        case Value::ADDS_BUILTIN:
        {
            execute< AddsBuiltin >( type, res, reg, size );
            return;
        }
        case Value::SUBU_BUILTIN:
        {
            execute< SubuBuiltin >( type, res, reg, size );
            return;
        }
        case Value::SUBS_BUILTIN:
        {
            execute< SubsBuiltin >( type, res, reg, size );
            return;
        }
        case Value::MULU_BUILTIN:
        {
            execute< MuluBuiltin >( type, res, reg, size );
            return;
        }
        case Value::MULS_BUILTIN:
        {
            execute< MulsBuiltin >( type, res, reg, size );
            return;
        }
        case Value::COMPARE_BUILTIN:
        case Value::LESU_BUILTIN:
        {
            execute< LesuBuiltin >( type, res, reg, size );
            return;
        }
        case Value::LESS_BUILTIN:
        {
            execute< LessBuiltin >( type, res, reg, size );
            return;
        }
        case Value::LEQU_BUILTIN:
        {
            execute< LequBuiltin >( type, res, reg, size );
            return;
        }
        case Value::LEQS_BUILTIN:
        {
            execute< LeqsBuiltin >( type, res, reg, size );
            return;
        }
        case Value::GREU_BUILTIN:
        {
            execute< GreuBuiltin >( type, res, reg, size );
            return;
        }
        case Value::GRES_BUILTIN:
        {
            execute< GresBuiltin >( type, res, reg, size );
            return;
        }
        case Value::GEQU_BUILTIN:
        {
            execute< GequBuiltin >( type, res, reg, size );
            return;
        }
        case Value::GEQS_BUILTIN:
        {
            execute< GeqsBuiltin >( type, res, reg, size );
            return;
        }
        case Value::BINARY_BUILTIN:
        case Value::ZEXT_BUILTIN:
        {
            execute< ZextBuiltin >( type, res, reg, size );
            return;
        }
        case Value::SEXT_BUILTIN:
        {
            execute< SextBuiltin >( type, res, reg, size );
            return;
        }
        case Value::TRUNC_BUILTIN:
        {
            execute< TruncBuiltin >( type, res, reg, size );
            return;
        }
        case Value::SHL_BUILTIN:
        {
            execute< ShlBuiltin >( type, res, reg, size );
            return;
        }
        case Value::SHR_BUILTIN:
        {
            execute< ShrBuiltin >( type, res, reg, size );
            return;
        }
        case Value::ASHR_BUILTIN:
        {
            execute< AshrBuiltin >( type, res, reg, size );
            return;
        }
        case Value::CLZ_BUILTIN:
        {
            execute< ClzBuiltin >( type, res, reg, size );
            return;
        }
        case Value::CLO_BUILTIN:
        {
            execute< CloBuiltin >( type, res, reg, size );
            return;
        }
        case Value::CLS_BUILTIN:
        {
            execute< ClsBuiltin >( type, res, reg, size );
            return;
        }
        case Value::_SIZE_:
        {
            break;
        }
    }

    throw InternalException(
        "invalid ID '" + Value::token( id ) + "' to 'execute( res, reg*, size )'" );
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

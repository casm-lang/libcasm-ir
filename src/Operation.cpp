//
//  Copyright (C) 2015-2020 CASM Organization <https://casm-lang.org>
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

#include "Operation.h"

#include <libcasm-ir/Builtin>
#include <libcasm-ir/Constant>
#include <libcasm-ir/Exception>
#include <libcasm-ir/Instruction>

using namespace libcasm_ir;

void Operation::execute(
    const Value::ID id, const Type::Ptr& type, Constant& res, const Constant& lhs )
{
    switch( id )
    {
        //
        // Arithmethic Instruction
        //
        case Value::ID::INV_INSTRUCTION:
        {
            execute< InvInstruction >( type, res, lhs );
            break;
        }

        //
        // Logical Instruction
        //
        case Value::ID::NOT_INSTRUCTION:
        {
            execute< NotInstruction >( type, res, lhs );
            break;
        }

        //
        // General Builtins
        //
        case Value::ID::IS_SYMBOLIC_BUILTIN:
        {
            execute< IsSymbolicBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::ASSERT_BUILTIN:
        {
            execute< AssertBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::ASSURE_BUILTIN:
        {
            execute< AssureBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::SIZE_BUILTIN:
        {
            execute< SizeBuiltin >( type, res, &lhs, 1 );
            break;
        }

        //
        // Output Builtins
        //
        case Value::ID::PRINT_BUILTIN:
        {
            execute< PrintBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::PRINTLN_BUILTIN:
        {
            execute< PrintLnBuiltin >( type, res, &lhs, 1 );
            break;
        }

        //
        // Casting Builtins
        //
        case Value::ID::AS_BOOLEAN_BUILTIN:
        {
            execute< AsBooleanBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::AS_INTEGER_BUILTIN:
        {
            execute< AsIntegerBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::AS_BINARY_BUILTIN:
        {
            execute< AsBinaryBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::AS_STRING_BUILTIN:
        {
            execute< AsStringBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::AS_DECIMAL_BUILTIN:
        {
            execute< AsDecimalBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::AS_RATIONAL_BUILTIN:
        {
            execute< AsRationalBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::AS_ENUMERATION_BUILTIN:
        {
            execute< AsEnumerationBuiltin >( type, res, &lhs, 1 );
            break;
        }

        //
        // Stringify Builtins
        //
        case Value::ID::DEC_BUILTIN:
        {
            execute< DecBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::HEX_BUILTIN:
        {
            execute< HexBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::OCT_BUILTIN:
        {
            execute< OctBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::BIN_BUILTIN:
        {
            execute< BinBuiltin >( type, res, &lhs, 1 );
            break;
        }

        //
        // Binary Builtins
        //
        case Value::ID::CLZ_BUILTIN:
        {
            execute< ClzBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::CLO_BUILTIN:
        {
            execute< CloBuiltin >( type, res, &lhs, 1 );
            break;
        }
        case Value::ID::CLS_BUILTIN:
        {
            execute< ClsBuiltin >( type, res, &lhs, 1 );
            break;
        }

        default:
        {
            throw InternalException(
                "invalid ID '" + Value::token( id ) + "' to 'execute( res, lhs )'" );
            break;
        }
    }
}

void Operation::execute(
    const Value::ID id,
    const Type::Ptr& type,
    Constant& res,
    const Constant& lhs,
    const Constant& rhs )
{
    switch( id )
    {
        //
        // Arithmetic Instruction
        //
        case Value::ID::ADD_INSTRUCTION:
        {
            execute< AddInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::SUB_INSTRUCTION:
        {
            execute< SubInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::MUL_INSTRUCTION:
        {
            execute< MulInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::DIV_INSTRUCTION:
        {
            execute< DivInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::POW_INSTRUCTION:
        {
            execute< PowInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::MOD_INSTRUCTION:
        {
            execute< ModInstruction >( type, res, lhs, rhs );
            break;
        }

        //
        // Compare Instruction
        //
        case Value::ID::EQU_INSTRUCTION:
        {
            execute< EquInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::NEQ_INSTRUCTION:
        {
            execute< NeqInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::LTH_INSTRUCTION:
        {
            execute< LthInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::LEQ_INSTRUCTION:
        {
            execute< LeqInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::GTH_INSTRUCTION:
        {
            execute< GthInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::GEQ_INSTRUCTION:
        {
            execute< GeqInstruction >( type, res, lhs, rhs );
            break;
        }

        //
        // Logical Instruction
        //
        case Value::ID::OR_INSTRUCTION:
        {
            execute< OrInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::IMP_INSTRUCTION:
        {
            execute< ImpInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::XOR_INSTRUCTION:
        {
            execute< XorInstruction >( type, res, lhs, rhs );
            break;
        }
        case Value::ID::AND_INSTRUCTION:
        {
            execute< AndInstruction >( type, res, lhs, rhs );
            break;
        }

        //
        // General Builtins
        //

        //
        // Arithmetic Builtins
        //
        case Value::ID::ADDU_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< AdduBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::ADDS_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< AddsBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::SUBU_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< SubuBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::SUBS_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< SubsBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::MULU_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< MuluBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::MULS_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< MulsBuiltin >( type, res, reg, 2 );
            break;
        }

        //
        // Compare Builtins
        //
        case Value::ID::LESU_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< LesuBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::LESS_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< LessBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::LEQU_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< LequBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::LEQS_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< LeqsBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::GREU_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< GreuBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::GRES_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< GresBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::GEQU_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< GequBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::GEQS_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< GeqsBuiltin >( type, res, reg, 2 );
            break;
        }

        //
        // Binary Builtins
        //
        case Value::ID::ZEXT_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< ZextBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::SEXT_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< SextBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::TRUNC_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< TruncBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::SHL_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< ShlBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::SHR_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< ShrBuiltin >( type, res, reg, 2 );
            break;
        }
        case Value::ID::ASHR_BUILTIN:
        {
            const Constant reg[] = { lhs, rhs };
            execute< AshrBuiltin >( type, res, reg, 2 );
            break;
        }

        default:
        {
            throw InternalException(
                "invalid ID '" + Value::token( id ) + "' to 'execute( res, lhs, rhs )'" );
            break;
        }
    }
}

void Operation::execute(
    const Value::ID id,
    const Type::Ptr& type,
    Constant& res,
    const Constant* reg,
    const std::size_t size )
{
    switch( id )
    {
        //
        // General Builtins
        //
        case Value::ID::ABORT_BUILTIN:
        {
            execute< AbortBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::IS_SYMBOLIC_BUILTIN:
        {
            execute< IsSymbolicBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::ASSERT_BUILTIN:
        {
            execute< AssertBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::ASSURE_BUILTIN:
        {
            execute< AssureBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::SIZE_BUILTIN:
        {
            execute< SizeBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::AT_BUILTIN:
        {
            execute< AtBuiltin >( type, res, reg, size );
            break;
        }

        //
        // Output Builtins
        //
        case Value::ID::PRINT_BUILTIN:
        {
            execute< PrintBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::PRINTLN_BUILTIN:
        {
            execute< PrintLnBuiltin >( type, res, reg, size );
            break;
        }

        //
        // Casting Builtins
        //
        case Value::ID::AS_BOOLEAN_BUILTIN:
        {
            execute< AsBooleanBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::AS_INTEGER_BUILTIN:
        {
            execute< AsIntegerBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::AS_BINARY_BUILTIN:
        {
            execute< AsBinaryBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::AS_STRING_BUILTIN:
        {
            execute< AsStringBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::AS_DECIMAL_BUILTIN:
        {
            execute< AsDecimalBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::AS_RATIONAL_BUILTIN:
        {
            execute< AsRationalBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::AS_ENUMERATION_BUILTIN:
        {
            execute< AsEnumerationBuiltin >( type, res, reg, size );
            break;
        }

        //
        // Stringify Builtins
        //
        case Value::ID::DEC_BUILTIN:
        {
            execute< DecBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::HEX_BUILTIN:
        {
            execute< HexBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::OCT_BUILTIN:
        {
            execute< OctBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::BIN_BUILTIN:
        {
            execute< BinBuiltin >( type, res, reg, size );
            break;
        }

        //
        // Arithmetic Builtins
        //
        case Value::ID::ADDU_BUILTIN:
        {
            execute< AdduBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::ADDS_BUILTIN:
        {
            execute< AddsBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::SUBU_BUILTIN:
        {
            execute< SubuBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::SUBS_BUILTIN:
        {
            execute< SubsBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::MULU_BUILTIN:
        {
            execute< MuluBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::MULS_BUILTIN:
        {
            execute< MulsBuiltin >( type, res, reg, size );
            break;
        }

        //
        // Compare Builtins
        //
        case Value::ID::LESU_BUILTIN:
        {
            execute< LesuBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::LESS_BUILTIN:
        {
            execute< LessBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::LEQU_BUILTIN:
        {
            execute< LequBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::LEQS_BUILTIN:
        {
            execute< LeqsBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::GREU_BUILTIN:
        {
            execute< GreuBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::GRES_BUILTIN:
        {
            execute< GresBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::GEQU_BUILTIN:
        {
            execute< GequBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::GEQS_BUILTIN:
        {
            execute< GeqsBuiltin >( type, res, reg, size );
            break;
        }

        //
        // Binary Builtins
        //
        case Value::ID::ZEXT_BUILTIN:
        {
            execute< ZextBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::SEXT_BUILTIN:
        {
            execute< SextBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::TRUNC_BUILTIN:
        {
            execute< TruncBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::SHL_BUILTIN:
        {
            execute< ShlBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::SHR_BUILTIN:
        {
            execute< ShrBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::ASHR_BUILTIN:
        {
            execute< AshrBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::CLZ_BUILTIN:
        {
            execute< ClzBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::CLO_BUILTIN:
        {
            execute< CloBuiltin >( type, res, reg, size );
            break;
        }
        case Value::ID::CLS_BUILTIN:
        {
            execute< ClsBuiltin >( type, res, reg, size );
            break;
        }

            // //
            // // Math Builtins
            // //

            // case Value::ID::POW_BUILTIN:
            // {
            //     //execute< PowBuiltin
            //     >(
            //         type, res, reg, size );
            // break;
            // }
            // case Value::ID::RAND_BUILTIN:
            // {
            //     //execute< RandBuiltin
            //     >(
            //         type, res, reg, size );
            // break;
            // }

        default:
        {
            throw InternalException(
                "invalid ID '" + Value::token( id ) + "' to 'execute( res, reg*, size )'" );
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

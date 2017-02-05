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

#include "ConstantFoldingPass.h"

#include "analyze/CasmIRDumpPass.h"

#include "Instruction.h"
#include "Specification.h"
#include "Visitor.h"

#include "../stdhl/cpp/Log.h"

#include "../casm-rt/src/Value.h"

using namespace libcasm_ir;

char ConstantFoldingPass::id = 0;

static libpass::PassRegistration< ConstantFoldingPass > PASS(
    "Constant Folding Pass", "TBD", "ir-cf", 0 );

bool ConstantFoldingPass::run( libpass::PassResult& pr )
{
    Specification* value = (Specification*)pr.result< CasmIRDumpPass >();
    assert( value );

    value->iterate( Traversal::PREORDER, []( Value& value, Context& ) {
        if( auto instr = cast< Instruction >( value ) )
        {
            libstdhl::Log::info(
                "cf: %s = %s ...", instr->label(), instr->name() );

            u32 operand_pos = 0;

            if( auto call = cast< CallInstruction >( instr ) )
            {
                operand_pos = 1;

                if( not isa< Builtin >( call->callee() ) )
                {
                    // call instr does not call a builtin, abort constant
                    // folding
                    return;
                }

                libstdhl::Log::info( "  +--> call instr: %s, %s",
                    call->callee().name(), call->callee().type().name() );
            }
            else if( isa< OperatorInstruction >( instr ) )
            {
                libstdhl::Log::info( "  +--> operator instr" );
            }
            else
            {
                // instr is not a operator instr nor a call instr, abort
                // constant folding
                return;
            }

            for( ; operand_pos < instr->values().size(); operand_pos++ )
            {
                if( not isa< Constant >( instr->value( operand_pos ) ) )
                {
                    // non-constant instr operand found, abort constant folding
                    return;
                }
            }

            Value* result_ptr = libcasm_rt::Value::execute( *instr );
            assert( result_ptr );
            Value& result = static_cast< Value& >( *result_ptr );

            libstdhl::Log::info( "  +==> %s = %s %s", result.label(),
                result.type().name(), result.name() );

            instr->replaceAllUsesWith( result );
        }
    } );

    return true;
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

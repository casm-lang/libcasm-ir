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

// #include "../casm-rt/src/Instruction.h" // PPA: disabled for now at the
// master branch!!!

using namespace libcasm_ir;

char ConstantFoldingPass::id = 0;

static libpass::PassRegistration< ConstantFoldingPass > PASS(
    "Constant Folding Pass", "TBD", "ir-cf", 0 );

bool ConstantFoldingPass::run( libpass::PassResult& pr )
{
    Specification* value = (Specification*)pr.result< CasmIRDumpPass >();
    assert( value );

    value->iterate( Traversal::PREORDER, []( Value& value, Context& ) {
        if( isa< CallInstruction >( value ) )
        {
            CallInstruction& instr = static_cast< CallInstruction& >( value );

            Value& call_sym = *instr.value( 0 );

            if( isa< Builtin >( call_sym ) )
            {
                u1 is_constant_call = true;

                for( u32 i = 1; i < instr.values().size(); i++ )
                {
                    if( not isa< Constant >( instr.value( i ) ) )
                    {
                        is_constant_call = false;
                        break;
                    }
                }

                // if( is_constant_call ) // PPA: disabled for now at the master
                // branch!!!
                // {
                //     Value* result_ptr
                //         = libcasm_rt::Instruction::execute( instr );
                //     assert( result_ptr );
                //     Value& result = static_cast< Value& >( *result_ptr );

                //     libstdhl::Log::info( "%s, %s, %s, %s --> %s %s",
                //         instr.label(), instr.name(), call_sym.name(),
                //         call_sym.type().name(), result.name(),
                //         result.type().name() );
                // }
            }
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

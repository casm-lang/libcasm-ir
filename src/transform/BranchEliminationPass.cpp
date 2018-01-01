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

#include "BranchEliminationPass.h"

#include <libcasm-ir/Specification>
#include <libcasm-ir/analyze/ConsistencyCheckPass>

#include <libpass/PassLogger>
#include <libpass/PassRegistry>
#include <libpass/PassResult>
#include <libpass/PassUsage>

using namespace libcasm_ir;

char BranchEliminationPass::id = 0;

static libpass::PassRegistration< BranchEliminationPass > PASS(
    "IRBranchEliminationPass", "removes compile-time constant branches", "ir-be", 0 );

void BranchEliminationPass::usage( libpass::PassUsage& pu )
{
    pu.require< ConsistencyCheckPass >();
}

u1 BranchEliminationPass::run( libpass::PassResult& pr )
{
    libpass::PassLogger log( &id, stream() );

    const auto& data = pr.input< ConsistencyCheckPass >();
    const auto& specification = data->specification();

    for( auto rule : specification->rules() )
    {
        optimize( rule );
    }

    return true;
}

u64 BranchEliminationPass::optimize( Rule::Ptr& rule )
{
    libpass::PassLogger log( &id, stream() );

    u64 elimination = 0;

    rule->iterate( Traversal::POSTORDER, [&log, &elimination]( Value& value ) {

        if( isa< SelectInstruction >( value ) )
        {
            const auto instr = static_cast< SelectInstruction& >( value );

            log.info( instr.dump() );

            const auto lhs = instr.operand( 0 );

            if( isa< Constant >( lhs ) )
            {
                log.info( "    * lhs is constant" );

                for( std::size_t c = 1; c < instr.operands().size(); c += 2 )
                {
                    const auto rhs = instr.operand( c );

                    if( *lhs == *rhs )
                    {
                        // found constant block branch
                        const auto lbl = instr.operand( c + 1 );
                        assert( isa< ExecutionSemanticsBlock >( lbl ) );

                        log.info(
                            "    * lhs == rhs (op" + std::to_string( c ) + ") -> " + lbl->label() );

                        instr.statement()->replaceWith(
                            std::static_pointer_cast< ExecutionSemanticsBlock >( lbl ) );

                        elimination++;

                        return;
                    }
                }

                log.info( "    * rhs not in select -> skip" );

                auto stmt = libstdhl::Memory::make< TrivialStatement >();
                stmt->add< SkipInstruction >();

                instr.statement()->replaceWith( stmt );
                elimination++;
            }
        }

    } );

    return elimination;
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

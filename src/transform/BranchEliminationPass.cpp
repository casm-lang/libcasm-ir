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

#include "BranchEliminationPass.h"

#include "../Specification.h"

using namespace libcasm_ir;

char BranchEliminationPass::id = 0;

static libpass::PassRegistration< BranchEliminationPass > PASS(
    "IRBranchEliminationPass", "removes compile-time constant branches",
    "ir-be", 0 );

void BranchEliminationPass::usage( libpass::PassUsage& pu )
{
    pu.require< ConsistencyCheckPass >();
}

u1 BranchEliminationPass::run( libpass::PassResult& pr )
{
    libpass::PassLogger log( &id, stream() );

    auto data = pr.result< ConsistencyCheckPass >();
    auto specification = data->specification();

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

                        log.info( "    * lhs == rhs (op" + std::to_string( c )
                                  + ") -> "
                                  + lbl->label() );

                        instr.statement()->replaceWith(
                            std::static_pointer_cast< ExecutionSemanticsBlock >(
                                lbl ) );

                        elimination++;

                        return;
                    }
                }

                log.info( "    * rhs not in select -> skip" );

                auto stmt = libstdhl::make< TrivialStatement >();
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

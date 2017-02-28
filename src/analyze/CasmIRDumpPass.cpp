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

#include "CasmIRDumpPass.h"

#include "libcasm-ir.h"

using namespace libcasm_ir;

char CasmIRDumpPass::id = 0;

static libpass::PassRegistration< CasmIRDumpPass > PASS( "CASM IR Dumping Pass",
    "outputs from the CASM IR an ASCII representation for debugging",
    "ir-debug", 0 );

u1 CasmIRDumpPass::run( libpass::PassResult& pr )
{
    auto data = pr.result< CasmIRDumpPass >();
    assert( data );

    try
    {
        data->specification()->iterate(
            Traversal::PREORDER, [this]( Value& value, Context& ) {
                libstdhl::Log::info( "%p: %s%s", &value,
                    this->indention( value ).c_str(), value.dump().c_str() );
            } );
    }
    catch( ... )
    {
        libstdhl::Log::error( "unsuccessful dump of specification" );
        return false;
    }

    return true;
}

std::string CasmIRDumpPass::indention( Value& value ) const
{
    std::string ind = "";
    u8 cnt = 0;
    Value* p = ( &value );
    while( p != 0 )
    {
        if( isa< ExecutionSemanticsBlock >( p ) )
        {
            p = (Value*)( (ExecutionSemanticsBlock*)p )->scope().get();
        }
        else if( isa< Instruction >( p ) )
        {
            p = (Value*)( (Instruction*)p )->statement().get();
        }
        else if( isa< Statement >( p ) )
        {
            p = (Value*)( (Statement*)p )->scope().get();
        }
        else
        {
            break;
            assert( 0 );
        }

        cnt++;
        ind += "  ";
    }

    return ind;
}

void CasmIRDumpPass::visit_prolog( Specification& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( Specification& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Agent& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( Agent& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Builtin& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( Builtin& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Function& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( Function& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Derived& value, Context& )
{
}
void CasmIRDumpPass::visit_interlog( Derived& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( Derived& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Rule& value, Context& )
{
}
void CasmIRDumpPass::visit_interlog( Rule& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( Rule& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( ParallelBlock& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( ParallelBlock& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( SequentialBlock& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( SequentialBlock& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( TrivialStatement& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( TrivialStatement& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( BranchStatement& value, Context& )
{
}
void CasmIRDumpPass::visit_interlog( BranchStatement& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( BranchStatement& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( LocalInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( LocalInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( AssertInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( AssertInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( SelectInstruction& value, Context& )
{
    // i32 cnt = -1;
    // for( auto operand : value.operands() )
    // {
    //     cnt++;
    //     if( cnt == 0 or ( cnt % 2 ) == 1 )
    //     {
    //         if( isa< Instruction >( operand ) or isa< Constant >( operand ) )
    //         {
    //             fprintf( stderr, ", %s [%s]", operand->label(),
    //                 operand->type().description() );
    //         }
    //         else
    //         {
    //             fprintf( stderr, " : %s", operand->label() );
    //         }
    //     }
    //     else
    //     {
    //         assert( isa< ExecutionSemanticsBlock >( operand ) );

    //         fprintf( stderr, " : %s", operand->label() );
    //     }
    // }
}
void CasmIRDumpPass::visit_epilog( SelectInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( SkipInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( SkipInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( ForkInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( ForkInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( MergeInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( MergeInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( LocationInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( LocationInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( LookupInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( LookupInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( UpdateInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( UpdateInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( CallInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( CallInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( PrintInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( PrintInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( AddInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( AddInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( SubInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( SubInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( MulInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( MulInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( ModInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( ModInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( DivInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( DivInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( AndInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( AndInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( XorInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( XorInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( OrInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( OrInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( EquInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( EquInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( NeqInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( NeqInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( LthInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( LthInstruction& value, Context& )
{
}

//
// Constant
//

void CasmIRDumpPass::visit_prolog( AgentConstant& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( AgentConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( RuleReferenceConstant& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( RuleReferenceConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( BooleanConstant& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( BooleanConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( IntegerConstant& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( IntegerConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( BitConstant& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( BitConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( StringConstant& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( StringConstant& value, Context& )
{
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

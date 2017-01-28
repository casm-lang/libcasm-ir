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
    "ir-dump-debug", 0 );

u1 CasmIRDumpPass::run( libpass::PassResult& pr )
{
    Specification* value = (Specification*)pr.getResult< CasmIRDumpPass >();
    assert( value );

    value->iterate( Traversal::PREORDER, this );

    return true;
}

static const char* indention( Value& value )
{
    std::string ind = "";
    u8 cnt = 0;
    Value* p = ( &value );
    while( p != 0 )
    {
        if( isa< ExecutionSemanticsBlock >( p ) )
        {
            p = (Value*)( (ExecutionSemanticsBlock*)p )->getScope();
        }
        else if( isa< Instruction >( p ) )
        {
            p = (Value*)( (Instruction*)p )->getStatement();
        }
        else if( isa< Statement >( p ) )
        {
            p = (Value*)( (Statement*)p )->getScope();
        }
        else
        {
            break;
            assert( 0 );
        }

        cnt++;
        ind += "  ";
    }

    return libstdhl::Allocator::string( ind );
}

#define DUMP_PREFIX                                                            \
    fprintf( stderr, "%p: %s, %s%s ", &value, value.getLabel(),                \
        indention( value ), value.getName() )
#define DUMP_POSTFIX fprintf( stderr, "\n" );

#define DUMP_INSTR                                                             \
    for( auto v : value.getValues() )                                          \
    {                                                                          \
        fprintf( stderr, ", %s [%s]", v->getLabel(),                           \
            v->getType()->getDescription() );                                  \
    }

void CasmIRDumpPass::visit_prolog( Specification& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Specification& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Agent& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Agent& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Builtin& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Builtin& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Function& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Function& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Derived& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_interlog( Derived& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( Derived& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( Rule& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_interlog( Rule& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( Rule& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( ParallelBlock& value, Context& )
{
    DUMP_PREFIX;
    fprintf( stderr, " (%p, %p) ", value.getScope(), value.getParent() );
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( ParallelBlock& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( SequentialBlock& value, Context& )
{
    DUMP_PREFIX;
    fprintf( stderr, " (%p, %p) ", value.getScope(), value.getParent() );
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SequentialBlock& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( TrivialStatement& value, Context& )
{
    DUMP_PREFIX;
    fprintf( stderr, " (%p, %p) ", value.getScope(), value.getParent() );
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( TrivialStatement& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( BranchStatement& value, Context& )
{
    DUMP_PREFIX;
    fprintf( stderr, " (%p, %p) ", value.getScope(), value.getParent() );
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_interlog( BranchStatement& value, Context& )
{
}
void CasmIRDumpPass::visit_epilog( BranchStatement& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( LocalInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LocalInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( AssertInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AssertInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( SelectInstruction& value, Context& )
{
    DUMP_PREFIX;

    i32 cnt = -1;
    for( auto v : value.getValues() )
    {
        cnt++;
        if( cnt == 0 or ( cnt % 2 ) == 1 )
        {
            if( isa< Instruction >( v ) or isa< Constant >( v ) )
            {
                fprintf( stderr, ", %s [%s]", v->getLabel(),
                    v->getType()->getDescription() );
            }
            else
            {
                fprintf( stderr, " : %s", v->getLabel() );
            }
        }
        else
        {
            assert( isa< ExecutionSemanticsBlock >( v ) );

            fprintf( stderr, " : %s", v->getLabel() );
        }
    }

    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SelectInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( SkipInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SkipInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( ForkInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( ForkInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( MergeInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( MergeInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( LocationInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LocationInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( LookupInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LookupInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( UpdateInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( UpdateInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( CallInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( CallInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( PrintInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( PrintInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( AddInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AddInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_prolog( SubInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SubInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_prolog( MulInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( MulInstruction& value, Context& )
{
}
void CasmIRDumpPass::visit_prolog( ModInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( ModInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( DivInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( DivInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( AndInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AndInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( XorInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( XorInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( OrInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( OrInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( EquInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( EquInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( NeqInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( NeqInstruction& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( LthInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LthInstruction& value, Context& )
{
}

//
// Constant
//

void CasmIRDumpPass::visit_prolog( AgentConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AgentConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( RuleReferenceConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( RuleReferenceConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( BooleanConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( BooleanConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( IntegerConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( IntegerConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( BitConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( BitConstant& value, Context& )
{
}

void CasmIRDumpPass::visit_prolog( StringConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
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

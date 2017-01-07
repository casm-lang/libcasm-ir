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
#include "Specification.h"

using namespace libcasm_ir;

char CasmIRDumpPass::id = 0;

static libpass::PassRegistration< CasmIRDumpPass > PASS( "CASM IR Dumping Pass",
    "outputs from the CASM IR an ASCII representation for debugging",
    "ir-dump-debug", 0 );

bool CasmIRDumpPass::run( libpass::PassResult& pr )
{
    Specification* value = (Specification*)pr.getResult< CasmIRDumpPass >();
    assert( value );

    value->iterate( Traversal::PREORDER, this );

    return true;
}

static const char* indention( Value& value )
{
    string ind = "";
    u8 cnt = 0;
    Value* p = ( &value );
    while( p != 0 )
    {
        if( Value::isa< ExecutionSemanticsBlock >( p ) )
        {
            p = (Value*)( (ExecutionSemanticsBlock*)p )->getScope();
        }
        else if( Value::isa< Instruction >( p ) )
        {
            p = (Value*)( (Instruction*)p )->getStatement();
        }
        else if( Value::isa< Statement >( p ) )
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
    fprintf( stderr, "%-14s: %p, %s, %s%s ", __FUNCTION__, &value,             \
        value.getLabel(), indention( value ), value.getName() )
#define DUMP_POSTFIX fprintf( stderr, "\n" );

#define DUMP_INSTR                                                             \
    for( auto v : value.getValues() )                                          \
    {                                                                          \
        fprintf( stderr, ", %s [%s]", v->getLabel(),                           \
            v->getType()->getDescription() );                                  \
    }

void CasmIRDumpPass::visit_prolog( Specification& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Specification& value )
{
}

void CasmIRDumpPass::visit_prolog( Agent& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Agent& value )
{
}

void CasmIRDumpPass::visit_prolog( Builtin& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Builtin& value )
{
}

void CasmIRDumpPass::visit_prolog( Function& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Function& value )
{
}

void CasmIRDumpPass::visit_prolog( Derived& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_interlog( Derived& value )
{
}
void CasmIRDumpPass::visit_epilog( Derived& value )
{
}

void CasmIRDumpPass::visit_prolog( Rule& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_interlog( Rule& value )
{
}
void CasmIRDumpPass::visit_epilog( Rule& value )
{
}

void CasmIRDumpPass::visit_prolog( ParallelBlock& value )
{
    DUMP_PREFIX;
    fprintf( stderr, " (%p, %p) ", value.getScope(), value.getParent() );
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( ParallelBlock& value )
{
}

void CasmIRDumpPass::visit_prolog( SequentialBlock& value )
{
    DUMP_PREFIX;
    fprintf( stderr, " (%p, %p) ", value.getScope(), value.getParent() );
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SequentialBlock& value )
{
}

void CasmIRDumpPass::visit_prolog( TrivialStatement& value )
{
    DUMP_PREFIX;
    fprintf( stderr, " (%p, %p) ", value.getScope(), value.getParent() );
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( TrivialStatement& value )
{
}

void CasmIRDumpPass::visit_prolog( BranchStatement& value )
{
    DUMP_PREFIX;
    fprintf( stderr, " (%p, %p) ", value.getScope(), value.getParent() );
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_interlog( BranchStatement& value )
{
}
void CasmIRDumpPass::visit_epilog( BranchStatement& value )
{
}

void CasmIRDumpPass::visit_prolog( LocalInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LocalInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( AssertInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AssertInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( SelectInstruction& value )
{
    DUMP_PREFIX;

    i32 cnt = -1;
    for( auto v : value.getValues() )
    {
        cnt++;
        if( cnt == 0 or ( cnt % 2 ) == 1 )
        {
            if( Value::isa< Instruction >( v ) or Value::isa< Constant >( v ) )
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
            assert( Value::isa< ExecutionSemanticsBlock >( v ) );

            fprintf( stderr, " : %s", v->getLabel() );
        }
    }

    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SelectInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( SkipInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SkipInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( ForkInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( ForkInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( MergeInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( MergeInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( LocationInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LocationInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( LookupInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LookupInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( UpdateInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( UpdateInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( CallInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( CallInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( PrintInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( PrintInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( AddInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AddInstruction& value )
{
}
void CasmIRDumpPass::visit_prolog( SubInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SubInstruction& value )
{
}
void CasmIRDumpPass::visit_prolog( MulInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( MulInstruction& value )
{
}
void CasmIRDumpPass::visit_prolog( ModInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( ModInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( DivInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( DivInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( AndInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AndInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( XorInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( XorInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( OrInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( OrInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( EquInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( EquInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( NeqInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( NeqInstruction& value )
{
}

void CasmIRDumpPass::visit_prolog( LthInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LthInstruction& value )
{
}

//
// Constant
//

void CasmIRDumpPass::visit_prolog( AgentConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AgentConstant& value )
{
}

void CasmIRDumpPass::visit_prolog( RuleReferenceConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( RuleReferenceConstant& value )
{
}

void CasmIRDumpPass::visit_prolog( BooleanConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( BooleanConstant& value )
{
}

void CasmIRDumpPass::visit_prolog( IntegerConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( IntegerConstant& value )
{
}

void CasmIRDumpPass::visit_prolog( BitConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( BitConstant& value )
{
}

void CasmIRDumpPass::visit_prolog( StringConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( StringConstant& value )
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

//  
//  Copyright (c) 2015-2016 CASM Organization
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

static libpass::PassRegistration< CasmIRDumpPass > PASS
( "CASM IR Dumping Pass"
, "generates a ASCII representation of the CASM IR"
, 0 //"ast-dump"
, 'D'
);


bool CasmIRDumpPass::run( libpass::PassResult& pr )
{
    // CasmIRNode* node = (CasmIRNode*)pr.getResult< TypeCheckPass >();
    
    // CasmIRWalker< CasmIRDumpVisitor, bool > dump_walker( *this );
    
    // dump_walker.suppress_calls = true;
    // dump_walker.walk_specification( node );
    
    // // std::cout << this->get_dump() << std::endl;
    
    // std::ofstream dotfile( "./obj/out.dot" );
    // dotfile << this->get_dump() << "\n";
    // dotfile.close();
    
    
    Specification* value = (Specification*)pr.getResult< CasmIRDumpPass >();
    assert( value );
    
    value->iterate
    ( Traversal::PREORDER
    , this
    );
    
    return false;
}

static const char* indention( Value& value )
{
    string ind = "";
    u8 cnt = 0;
    Value* p = (&value);
    while( p != 0 )
    {
        if( Value::isa< ExecutionSemanticsBlock >( p ) )
        {
            p = (Value*)((ExecutionSemanticsBlock*)p)->getScope();
        }
        else if( Value::isa< Instruction >( p ) )
        {
            p = (Value*)((Instruction*)p)->getStatement();
        }
        else if( Value::isa< Statement >( p ) )
        {
            p = (Value*)((Statement*)p)->getScope();
        }
        else
        {
            break;
            assert(0);
        }
        
        cnt++;
        ind+="  ";
    }
    
    return libstdhl::Allocator::string( ind );
}



#define DUMP_PREFIX  printf( "%-14s: %p, %s, %s%s ", __FUNCTION__, &value, value.getLabel(), indention( value ), value.getName() )
#define DUMP_POSTFIX printf( "\n" );

#define DUMP_INSTR for( auto v : value.getValues() ) { printf( ", %s (%s)", v->getLabel(), v->getType()->getName() ); }

void CasmIRDumpPass::visit_prolog( Specification& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( Specification& value ) {}

void CasmIRDumpPass::visit_prolog( Agent& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( Agent& value ) {}

void CasmIRDumpPass::visit_prolog( Function& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( Function& value ) {}

void CasmIRDumpPass::visit_prolog( Rule& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_interlog( Rule& value ) {}
void CasmIRDumpPass::visit_epilog( Rule& value ) {}

void CasmIRDumpPass::visit_prolog( ParallelBlock& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( ParallelBlock& value ) {}

void CasmIRDumpPass::visit_prolog( SequentialBlock& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( SequentialBlock& value ) {}

void CasmIRDumpPass::visit_prolog( TrivialStatement& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( TrivialStatement& value ) {}

void CasmIRDumpPass::visit_prolog( BranchStatement& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_interlog( BranchStatement& value ) {}
void CasmIRDumpPass::visit_epilog( BranchStatement& value ) {}

void CasmIRDumpPass::visit_prolog( LocationInstruction& value ) { DUMP_PREFIX; DUMP_INSTR; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( LocationInstruction& value ) {}

void CasmIRDumpPass::visit_prolog( LookupInstruction& value ) { DUMP_PREFIX; DUMP_INSTR; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( LookupInstruction& value ) {}

void CasmIRDumpPass::visit_prolog( UpdateInstruction& value ) { DUMP_PREFIX; DUMP_INSTR; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( UpdateInstruction& value ) {}

void CasmIRDumpPass::visit_prolog( PrintInstruction& value ) { DUMP_PREFIX; DUMP_INSTR; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( PrintInstruction& value ) {}

void CasmIRDumpPass::visit_prolog( AddInstruction& value ) { DUMP_PREFIX; DUMP_INSTR; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( AddInstruction& value ) {}

void CasmIRDumpPass::visit_prolog( DivInstruction& value ) { DUMP_PREFIX; DUMP_INSTR; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( DivInstruction& value ) {}

void CasmIRDumpPass::visit_prolog( AndInstruction& value ) { DUMP_PREFIX; DUMP_INSTR; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( AndInstruction& value ) {}

void CasmIRDumpPass::visit_prolog( EquInstruction& value ) { DUMP_PREFIX; DUMP_INSTR; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( EquInstruction& value ) {}

void CasmIRDumpPass::visit_prolog( IntegerConstant& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( IntegerConstant& value ) {}

void CasmIRDumpPass::visit_prolog( StringConstant& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( StringConstant& value ) {}

void CasmIRDumpPass::visit_prolog( RulePointerConstant& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( RulePointerConstant& value ) {}

void CasmIRDumpPass::visit_prolog( AgentConstant& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void CasmIRDumpPass::visit_epilog( AgentConstant& value ) {}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

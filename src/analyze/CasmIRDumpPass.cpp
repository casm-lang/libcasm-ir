//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/casm-ir
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  WITH THE SOFTWARE.
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


#define DUMP_PREFIX  printf( "%-14s: %p, %s ", __FUNCTION__, &value, value.getName() )
#define DUMP_POSTFIX printf( "\n" );

void CasmIRDumpPass::visit_prolog( Specification& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Specification& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( Agent& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Agent& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( Function& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Function& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( Rule& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_interlog( Rule& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( Rule& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}


void CasmIRDumpPass::visit_prolog( ParallelBlock& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( ParallelBlock& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( SequentialBlock& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( SequentialBlock& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( TrivialStatement& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( TrivialStatement& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

		
void CasmIRDumpPass::visit_prolog( LocationInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LocationInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( LookupInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( LookupInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( UpdateInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( UpdateInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( AddInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AddInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( DivInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( DivInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( AndInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( AndInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void CasmIRDumpPass::visit_prolog( IntegerConstant& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void CasmIRDumpPass::visit_epilog( IntegerConstant& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

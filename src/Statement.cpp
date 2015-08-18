//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
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

#include "Statement.h"

#include "Constant.h"
#include "Instruction.h"

using namespace libcasm_ir;


Statement::Statement( const char* name, Type* type, ExecutionSemanticsBlock* scope, Value::ID id )
: Block( name, type, id )
, scope( scope )
{
	assert( scope );
	
	scope->add( this );
	
	printf( "[Statement] '%s' at %lu\n", name, scope->getPseudoState() );
}

ExecutionSemanticsBlock* Statement::getScope( void ) const
{
	return scope;
}

void Statement::add( Value* instruction )
{
	printf( "%s: %p\n", __FUNCTION__, instruction );
	assert( instruction );

	if( Value::isa< ConstantValue >( instruction ) )
	{
		printf( "%s: %p --> Constant, omitted\n", __FUNCTION__, instruction );
		return;
	}
	
	if( Value::isa< Instruction >( instruction ) )
	{
		printf( "%s: %p --> Instruction\n", __FUNCTION__, instruction );
	    static_cast< Instruction* >( instruction )->setStatement( this );
	}
	
	instructions.push_back( instruction );
	printf( "[Stmt] add: %p\n", instruction );	
}

void Statement::dump( void ) const
{
	for( auto instr : instructions )
	{
		static_cast< Value* >( instr )->dump();
	}
}





TrivialStatement::TrivialStatement( ExecutionSemanticsBlock* scope )
: Statement( "block", 0, scope, Value::TRIVIAL_STATEMENT )
{
}

void TrivialStatement::dump( void ) const
{
	printf( "[TrStm] %p @ %lu\n", this, scope->getPseudoState() );
	((Statement*)this)->dump();	
}





BranchStatement::BranchStatement( const char* name, Type* type, ExecutionSemanticsBlock* scope )
: Statement( name, type, scope, Value::BRANCH_STATEMENT )
{
}

void BranchStatement::add( Block* block )
{
	blocks.push_back( block );
}

void BranchStatement::dump( void ) const
{
	printf( "[BranchStatement] %p\n", this );
	
	((Statement*)this)->dump();
	
	// TODO: here the branches etc.
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

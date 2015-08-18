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

/**
   @file     Value.cpp
   @author   Philipp Paulweber
   @date     2015-02-20
*/

#include "Value.h"
#include "libcasm-ir.h"

using namespace libcasm_ir;


Value::Value( const char* name, Type* type, Value::ID id )
: name( name )
, type( type )
, id( id )
{
	SymbolTable& symbols = *getSymbols();
	symbols[ name ].insert( this );
	printf( "[Value] created '%s' @ %p of type %p\n", name, this, type );
}

Value::~Value()
{
	SymbolTable& symbols = *getSymbols();
	symbols[ name ].erase( this );
	printf( "[Value] deleted '%s' @ %p of type %p\n", name, this, type );
}

const char* Value::getName( void ) const
{
	return name;
}

Type* Value::getType( void ) const
{
	return type;
}


Value::ID Value::getValueID() const
{
	return id;
}


void Value::dump( void ) const
{
	switch( this->getValueID() )
	{
	case Value::RULE:
		return ((Rule*)this)->dump(); break;
	case Value::BLOCK:
		return ((Block*)this)->dump(); break;
	case Value::EXECUTION_SEMANTICS_BLOCK:
		return ((ExecutionSemanticsBlock*)this)->dump(); break;
	case Value::PARALLEL_BLOCK:
		return ((ParallelBlock*)this)->dump(); break;
	case Value::SEQUENTIAL_BLOCK:
		return ((SequentialBlock*)this)->dump(); break;
	case Value::STATEMENT:
		return ((Statement*)this)->dump(); break;
	case Value::TRIVIAL_STATEMENT:
		return ((TrivialStatement*)this)->dump(); break;
	case Value::BRANCH_STATEMENT:
		return ((BranchStatement*)this)->dump(); break;
	case Value::INSTRUCTION:
		return ((Instruction*)this)->dump(); break;
	case Value::UNARY_INSTRUCTION:
		return ((UnaryInstruction*)this)->dump(); break;
	case Value::BINARY_INSTRUCTION:
		return ((BinaryInstruction*)this)->dump(); break;
	case Value::LOOKUP_INSTRUCTION:
		return ((LookupInstruction*)this)->dump(); break;
	case Value::UPDATE_INSTRUCTION:
		return ((UpdateInstruction*)this)->dump(); break;
	case Value::LOCATION_INSTRUCTION:
		return ((LocationInstruction*)this)->dump(); break;
	case Value::CONSTANT:
		return ((ConstantValue*)this)->dump(); break;
	case Value::INTEGER_CONSTANT:
		return ((IntegerConstant*)this)->dump(); break;
	case Value::OPERATOR_INSTRUCTION:
		return ((OperatorInstruction*)this)->dump(); break;
	default:
		printf( "[Value] %p of %u\n", this, getValueID() );
	}
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

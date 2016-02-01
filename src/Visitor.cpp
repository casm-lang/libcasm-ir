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

#include "Visitor.h"
#include "Value.h"

using namespace libcasm_ir;

#define CASE_VALUE( VID, CLASS )									\
	case Value::ID::VID:												    \
	     if( stage == Stage::PROLOG )   visit_prolog(   *((CLASS*)value) ); \
	else if( stage == Stage::EPILOG )   visit_epilog(   *((CLASS*)value) ); \
	else assert( !"invalid visitor stage value!" );						    \
	break

#define CASE_VALUE_INTER( VID, CLASS )									\
	case Value::ID::VID:												    \
	     if( stage == Stage::PROLOG )   visit_prolog(   *((CLASS*)value) ); \
	else if( stage == Stage::INTERLOG ) visit_interlog( *((CLASS*)value) ); \
	else if( stage == Stage::EPILOG )   visit_epilog(   *((CLASS*)value) ); \
	else assert( !"invalid visitor stage value!" );						    \
	break

void Visitor::dispatch( Stage stage, Value* value )
{
	assert( value );
	
	switch( value->getValueID() )
	{
		CASE_VALUE( SPECIFICATION,        Specification );
		CASE_VALUE( AGENT,                Agent );
		
		CASE_VALUE( FUNCTION,             Function );
        CASE_VALUE_INTER( RULE,           Rule );
		
		CASE_VALUE( PARALLEL_BLOCK,       ParallelBlock );
		CASE_VALUE( SEQUENTIAL_BLOCK,     SequentialBlock );
	    CASE_VALUE( TRIVIAL_STATEMENT,    TrivialStatement );
		
		CASE_VALUE( LOCATION_INSTRUCTION, LocationInstruction );
		CASE_VALUE( LOOKUP_INSTRUCTION,   LookupInstruction );
		CASE_VALUE( UPDATE_INSTRUCTION,   UpdateInstruction );
		
		CASE_VALUE( ADD_INSTRUCTION,      AddInstruction );
		CASE_VALUE( DIV_INSTRUCTION,      DivInstruction );
		
		CASE_VALUE( AND_INSTRUCTION,      AndInstruction );
		
	    default:
			printf
			( "%s:%i: warning: unimplemented value name '%s' with id '%i' to dispatch\n"
			, __FILE__
			, __LINE__
			, value->getName()
			, value->getValueID()
			);
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

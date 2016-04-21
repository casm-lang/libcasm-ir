//  
//  Copyright (c) 2015-2016 CASM Organization
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
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
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
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

		CASE_VALUE( INTEGER_CONSTANT,     IntegerConstant );
		CASE_VALUE( RULE_POINTER_CONSTANT,RulePointerConstant );
		CASE_VALUE( AGENT_CONSTANT,       AgentConstant );
		
	    default:
		{
			fprintf
			( stderr
			, "%s:%i: error: unimplemented value name '%s' with id '%i' to dispatch\n"
			, __FILE__
			, __LINE__
			, value->getName()
			, value->getValueID()
			);
			assert(0);
			break;
		}
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

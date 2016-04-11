//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
//  
//  This software is licensed under the Open Software License v. 3.0 (OSL-3.0).
//  You either find the license text below or in the root directory of this 
//  project in the LICENSE.txt file. If you cannot find the LICENSE.txt file,
//  please refer to https://opensource.org/licenses/OSL-3.0.
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

		CASE_VALUE( INTEGER_CONSTANT,     IntegerConstant );
		
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

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

#include "Agent.h"

using namespace libcasm_ir;


Agent::Agent()
: User( ".agent", 0, Value::AGENT )
{			
    // (*Value::getSymbols())[ ".agent" ].insert( this );
}

// Agent::~Agent( void )
// {			
//     // (*Value::getSymbols())[ ".agent" ].erase( this );
// }

RulePointerConstant* Agent::getInitRulePointer( void ) const
{
    return rule_ptr_init;
}

void Agent::setInitRulePointer( RulePointerConstant* init )
{
    assert( init );	
    rule_ptr_init = init;
}

void Agent::dump( void ) const
{
    printf( "[Agent] " );
    debug();
    
    if( rule_ptr_init )
    {
        printf( "@%p\n", rule_ptr_init );
    }
    else
    {
        printf( "('rule_ptr_init' not set)\n" );
    }
}

bool Agent::classof( Value const* obj )
{
	return obj->getValueID() == Value::AGENT;
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

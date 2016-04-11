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

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

#include "Function.h"

using namespace libcasm_ir;


Function::Function( const char* name, Type* result )
: User( name, result, Value::FUNCTION )
, ident( 0 )
{
	ident = Identifier::create( result, name );
	(*Value::getSymbols())[ ".function" ].insert( this );
	(*Value::getSymbols())[ ".identifier" ].insert( ident );
}

Function::~Function( void )
{
	(*Value::getSymbols())[ ".function" ].erase( this );
	(*Value::getSymbols())[ ".identifier" ].erase( ident );
}

void Function::dump( void ) const
{
	printf( "[Function] " );
	debug();
}

bool Function::classof( Value const* obj )
{
	return obj->getValueID() == Value::FUNCTION;
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

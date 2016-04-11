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

#include "Derived.h"

using namespace libcasm_ir;


Derived::Derived( const char* name, Type* result )
: User( name, result, Value::DERIVED )
{			
    ident = Identifier::create( result, name );
	
	(*Value::getSymbols())[ ".derived" ].insert( this );
	(*Value::getSymbols())[ ".identifier" ].insert( this );
}

Derived::~Derived( void )
{
	(*Value::getSymbols())[ ".derived" ].erase( this );
	(*Value::getSymbols())[ ".identifier" ].erase( ident );
}

TrivialStatement* Derived::getContext( void ) const
{
	return context;
}

void Derived::setContext( TrivialStatement* scope )
{
	assert( scope );
	context = scope;
}

void Derived::addParameter( Value* value )
{
	assert( Value::isa< Identifier >( value ) and "parameter must be an identifier" );
    
	parameter.push_back( value );
}

const std::vector< Value* >& Derived::getParameters( void ) const
{
	return parameter;
}

void Derived::dump( void ) const
{
	printf( "[Derived] " );
    debug();
	
	if( context )
	{
		context->dump();
	}
	else
	{
		printf( "('context' not set)\n" );
	}
}

bool Derived::classof( Value const* obj )
{
	return obj->getValueID() == Value::DERIVED;
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

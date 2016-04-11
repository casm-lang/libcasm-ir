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

#include "Specification.h"
#include "libcasm-ir.h"

using namespace libcasm_ir;


Specification::Specification( const char* name )
: User( name, 0, Value::SPECIFICATION )
{			
	(*Value::getSymbols())[ ".specification" ].insert( this );
}

Specification::~Specification( void )
{			
	(*Value::getSymbols())[ ".specification" ].erase( this );
}

const std::vector< Value* >& Specification::getContent( void ) const
{
	return content;
}
		
void Specification::add( Value* value )
{
	assert( value );
	
	assert
	(  Value::isa< Rule >( value )
	or Value::isa< Derived >( value )
	or Value::isa< Function >( value )
	or Value::isa< Agent >( value )
	or Value::isa< ConstantValue >( value )
	);
	
	content.push_back( value );
}

void Specification::dump( void ) const
{
	printf( "[Specification ] " );
	debug();
	printf( "<<not implemented due to new dump pass>>\n" );
}

bool Specification::classof( Value const* obj )
{
	return obj->getValueID() == Value::SPECIFICATION;
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

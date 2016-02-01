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

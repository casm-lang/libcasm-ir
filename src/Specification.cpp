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

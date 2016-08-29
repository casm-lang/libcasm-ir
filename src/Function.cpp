//  
//  Copyright (c) 2015-2016 CASM Organization
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcasm-ir
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
//  along with libcasm-ir. If not, see <http://www.gnu.org/licenses/>.
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
    return obj->getValueID() == classid();
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

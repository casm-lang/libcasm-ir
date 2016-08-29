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

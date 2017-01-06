//
//  Copyright (c) 2015-2017 CASM Organization
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

#include "Specification.h"
#include "Agent.h"
#include "Builtin.h"
#include "Derived.h"
#include "Function.h"
#include "Rule.h"

using namespace libcasm_ir;

Specification::Specification( const char* name )
: Value( name, 0, Value::SPECIFICATION )
{
    getSymbols()[ ".specification" ].insert( this );
}

Specification::~Specification( void )
{
    getSymbols()[ ".specification" ].erase( this );
}

void Specification::add( Value* value )
{
    assert( value );

    if( Value::isa< Rule >( value ) )
    {
        content[ Rule::classid() ].emplace( value->getName(), value );
    }
    else if( Value::isa< Derived >( value ) )
    {
        content[ Derived::classid() ].emplace( value->getName(), value );
    }
    else if( Value::isa< Function >( value ) )
    {
        content[ Function::classid() ].emplace( value->getName(), value );
    }
    else if( Value::isa< Agent >( value ) )
    {
        content[ Agent::classid() ].emplace( value->getName(), value );
    }
    else if( Value::isa< Constant >( value ) )
    {
        content[ Constant::classid() ].emplace( value->getName(), value );
    }
    else if( Value::isa< Builtin >( value ) )
    {
        content[ Builtin::classid() ].emplace( value->getName(), value );
    }
    else
    {
        assert( !"unsupported Specification content Value found!" );
    }
}

void Specification::dump( void ) const
{
    printf( "[Specification ] " );
    debug();
    printf( "<<not implemented due to new dump pass>>\n" );
}

bool Specification::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

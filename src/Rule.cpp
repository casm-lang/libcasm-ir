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

#include "Rule.h"

using namespace libcasm_ir;

Rule::Rule( const char* name )
: User( name, 0, Value::RULE )
{
    ( *Value::getSymbols() )[ ".rule" ].insert( this );
}

Rule::~Rule( void )
{
    ( *Value::getSymbols() )[ ".rule" ].erase( this );
}

ParallelBlock* Rule::getContext( void ) const
{
    return context;
}

void Rule::setContext( ParallelBlock* scope )
{
    assert( scope );
    context = scope;
}

void Rule::dump( void ) const
{
    printf( "[Rule ] " );
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

bool Rule::classof( Value const* obj )
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

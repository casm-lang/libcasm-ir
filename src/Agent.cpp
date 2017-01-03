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

#include "Agent.h"

using namespace libcasm_ir;

Agent::Agent()
: User( ".agent", Type::getAgent(), Value::AGENT )
{
    // getSymbols()[ ".agent" ].insert( this );
}

// Agent::~Agent( void )
// {
//     // getSymbols()[ ".agent" ].erase( this );
// }

RuleReferenceConstant* Agent::getInitRuleReference( void ) const
{
    return rule_ptr_init;
}

void Agent::setInitRuleReference( RuleReferenceConstant* init )
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

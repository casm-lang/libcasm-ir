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
#include "Constant.h"

using namespace libcasm_ir;

Agent::Agent()
: User( "$", Type::Agent(), classid() )
, m_init_rule( 0 )
{
    static Agent* check = 0;

    assert(
        not check
        and " currently only a single execution agent behavior is "
            "supported! " );

    check = this;
}

Agent::~Agent( void )
{
}

RuleReferenceConstant* Agent::initRuleReference( void ) const
{
    assert( m_init_rule );
    return m_init_rule;
}

void Agent::setInitRuleReference( Value* init )
{
    assert( init and isa< RuleReferenceConstant >( init ) );
    m_init_rule = static_cast< RuleReferenceConstant* >( init );
}

Rule* Agent::initRule( void )
{
    RuleReferenceConstant* rule_ref = initRuleReference();
    assert( rule_ref->defined() );
    return rule_ref->value();
}

u1 Agent::classof( Value const* obj )
{
    return obj->id() == classid();
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

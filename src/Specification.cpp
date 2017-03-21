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

using namespace libcasm_ir;

Specification::Specification( const std::string& name )
: Value( name, libstdhl::get< VoidType >(), classid() )
{
}

void Specification::setAgent( const Agent::Ptr& agent )
{
    m_agent = agent;
}

Agent::Ptr Specification::agent( void ) const
{
    return m_agent;
}

void Specification::add( const Constant::Ptr& constant )
{
    m_constants.add( constant );
}
void Specification::add( const Builtin::Ptr& builtin )
{
    m_builtins.add( builtin );
}
void Specification::add( const Function::Ptr& function )
{
    m_functions.add( function );
}
void Specification::add( const Derived::Ptr& derived )
{
    m_deriveds.add( derived );
}
void Specification::add( const Rule::Ptr& rule )
{
    m_rules.add( rule );
}

Constants& Specification::constants( void )
{
    return m_constants;
}

Builtins& Specification::builtins( void )
{
    return m_builtins;
}

Functions& Specification::functions( void )
{
    return m_functions;
}

Deriveds& Specification::deriveds( void )
{
    return m_deriveds;
}

Rules& Specification::rules( void )
{
    return m_rules;
}

void Specification::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 Specification::classof( Value const* obj )
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

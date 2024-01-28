//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcasm-ir/graphs/contributors>
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
//  Additional permission under GNU GPL version 3 section 7
//
//  libcasm-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcasm-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcasm-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcasm-ir give you permission to link libcasm-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcasm-ir. If you modify libcasm-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "Specification.h"

using namespace libcasm_ir;

static const auto VOID = libstdhl::Memory::get< VoidType >();

Specification::Specification( const std::string& name )
: Value( VOID, classid() )
, m_name( name )
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

void Specification::add( const Type::Ptr& type )
{
    m_types.add( type );
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

Types& Specification::types( void )
{
    return m_types;
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

std::string Specification::name( void ) const
{
    return m_name;
}

std::size_t Specification::hash( void ) const
{
    return libstdhl::Hash::combine( classid(), std::hash< std::string >()( name() ) );
}

u1 Specification::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const Specification& >( rhs );
    return ( this->name() == other.name() );
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

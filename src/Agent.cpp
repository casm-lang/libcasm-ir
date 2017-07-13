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
#include "Enumeration.h"

using namespace libcasm_ir;

Agent::Agent( const Type::Ptr& type )
: Value( type, classid() )
{
}

Agent::Agent( const std::vector< std::string >& agents )
: Agent( libstdhl::make< EnumerationType >(
      libstdhl::make< Enumeration >( "Agent", agents ) ) )
{
}

std::string Agent::name( void ) const
{
    return "agent";
}

std::size_t Agent::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

u1 Agent::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    return true;
}

void Agent::accept( Visitor& visitor )
{
    visitor.visit( *this );
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

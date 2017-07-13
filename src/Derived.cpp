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

#include "Derived.h"

#include "Constant.h"
#include "Statement.h"

using namespace libcasm_ir;

Derived::Derived( const std::string& name, const Type::Ptr& result )
: User( result, classid() )
, m_name( name )
{
}

void Derived::setContext( const Statement::Ptr& context )
{
    m_context = context;
}

Statement::Ptr Derived::context( void ) const
{
    return m_context;
}

std::string Derived::name( void ) const
{
    return m_name;
}

std::size_t Derived::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

u1 Derived::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const Derived& >( rhs );
    return ( this->name() == other.name() );
}

void Derived::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 Derived::classof( Value const* obj )
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

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

#include "Range.h"

#include "Constant.h"

using namespace libcasm_ir;

Range::Range( const Value::Ptr& from, const Value::Ptr& to )
: Value( "", from->ptr_type(), classid() )
, m_from( from )
, m_to( to )
{
    if( from->type() != to->type() )
    {
        throw std::domain_error(
            "unable to create a range from different types '" + from->name()
            + "' and '"
            + to->name()
            + "'" );
    }
}

Range::Range( const Constant& from, const Constant& to )
: Range( libstdhl::make_unique< Constant >( from ),
      libstdhl::make_unique< Constant >( to ) )
{
}

Value::Ptr Range::from( void ) const
{
    return m_from;
}

Value::Ptr Range::to( void ) const
{
    return m_to;
}

std::string Range::name( void ) const
{
    return "[" + m_from->name() + ".." + m_to->name() + "]";
}

std::size_t Range::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

u1 Range::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const Range& >( rhs );
    return ( *this->from() == *other.from() )
           and ( *this->to() == *other.to() );
}

void Range::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 Range::classof( Value const* obj )
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

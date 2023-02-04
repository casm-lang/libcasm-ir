//
//  Copyright (C) 2015-2023 CASM Organization <https://casm-lang.org>
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

#include "Enumeration.h"

#include "Agent.h"

using namespace libcasm_ir;

Enumeration::Enumeration( const std::string& name, const std::vector< std::string >& values )
: Value( libstdhl::Memory::get< VoidType >(), classid() )
, m_name( name )
{
    for( const auto& v : values )
    {
        add( v );
    }
}

Enumeration::Enumeration( const std::string& name )
: Enumeration( name, {} )
{
}

void Enumeration::add( const std::string& value )
{
    const auto pos = m_values.size();

    m_values.emplace_back( value );

    auto result = m_value2uid.emplace( value, pos );
    if( not result.second )
    {
        throw std::domain_error(
            "enumeration '" + name() + "' already has a value '" + value + "'" );
    }
}

const std::vector< std::string >& Enumeration::elements( void ) const
{
    return m_values;
}

u64 Enumeration::encode( const std::string& value ) const
{
    auto result = m_value2uid.find( value );
    if( result != m_value2uid.end() )
    {
        return result->second;
    }

    throw std::domain_error(
        "invalid value '" + value + "' to encode for enumeration '" + name() + "'!" );
}

std::string Enumeration::decode( const u64 value ) const
{
    if( value >= m_values.size() )
    {
        throw std::domain_error(
            "invalid value '" + std::to_string( value ) + "' to decode for enumeration '" + name() +
            "'" );
    }

    return m_values[ value ];
}

std::string Enumeration::name( void ) const
{
    return m_name;
}

std::size_t Enumeration::hash( void ) const
{
    return libstdhl::Hash::combine( classid(), std::hash< std::string >()( name() ) );
}

u1 Enumeration::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const Enumeration& >( rhs );
    return ( this->name() == other.name() );
}

void Enumeration::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 Enumeration::classof( Value const* obj )
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

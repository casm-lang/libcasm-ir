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

#include "Rule.h"

#include "Block.h"
#include "Constant.h"

using namespace libcasm_ir;

Rule::Rule( const std::string& name, const Type::Ptr& type )
: User( type, classid() )
, m_name( name )
, m_context( 0 )
{
}

void Rule::setContext( const ParallelBlock::Ptr& context )
{
    if( not context )
    {
        throw std::domain_error(
            "adding a null pointer context is not allowed" );
    }
    if( context->rule() )
    {
        throw std::domain_error( "block '" + context->dump()
                                 + "' is already bound to rule '"
                                 + context->rule()->dump()
                                 + "'" );
    }

    const auto self = ptr_this< Rule >();

    context->setRule( self );

    m_context = context;
}

ParallelBlock::Ptr Rule::context( void ) const
{
    return m_context;
}

std::string Rule::name( void ) const
{
    return m_name;
}

std::size_t Rule::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

u1 Rule::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const Rule& >( rhs );
    return ( this->name() == other.name() );
}

void Rule::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 Rule::classof( Value const* obj )
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

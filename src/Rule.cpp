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

Rule::Rule( const char* name, Type* result )
: User( libstdhl::Allocator::string( "@" + std::string( name ) ),
      result,
      classid() )
, m_ident( 0 )
, m_context( 0 )
{
    m_ident = Identifier::create( result, this->name() );
}

Rule::~Rule( void )
{
}

ParallelBlock* Rule::context( void ) const
{
    return m_context;
}

void Rule::setContext( ParallelBlock* scope )
{
    assert( scope );
    m_context = scope;
}

void Rule::addParameter( Value* value )
{
    assert( isa< Identifier >( value ) and "parameter must be an identifier" );

    m_parameter.push_back( value );
}

const std::vector< Value* >& Rule::parameters( void ) const
{
    return m_parameter;
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

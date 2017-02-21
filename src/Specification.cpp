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
#include "Agent.h"
#include "Builtin.h"
#include "Constant.h"
#include "Derived.h"
#include "Function.h"
#include "Rule.h"

using namespace libcasm_ir;

Specification::Specification( const std::string& name )
: Value( name, 0, classid() )
{
}

void Specification::add( const Value::Ptr& value )
{
    assert( value );

    if( isa< Rule >( value ) )
    {
        m_content[ Rule::classid() ].emplace( value->name(), value );
    }
    else if( isa< Derived >( value ) )
    {
        m_content[ Derived::classid() ].emplace( value->name(), value );
    }
    else if( isa< Function >( value ) )
    {
        m_content[ Function::classid() ].emplace( value->name(), value );
    }
    else if( isa< Agent >( value ) )
    {
        m_content[ Agent::classid() ].emplace( value->name(), value );
    }
    else if( isa< Constant >( value ) )
    {
        m_content[ Constant::classid() ].emplace( value->name(), value );
    }
    else if( isa< Builtin >( value ) )
    {
        Builtin& obj = static_cast< Builtin& >( *value );

        m_content[ Builtin::classid() ].emplace( obj.description(), value );
    }
    else
    {
        assert( !"unsupported Specification content Value found!" );
    }
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

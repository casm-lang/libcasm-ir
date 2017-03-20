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

#include "User.h"

#include "Agent.h"
#include "Builtin.h"
#include "Constant.h"
#include "Derived.h"
#include "Function.h"
#include "Instruction.h"
#include "Rule.h"

using namespace libcasm_ir;

User::User( const std::string& name, const Type::Ptr& type, Value::ID id )
: Value( name, type, id )
, m_uses()
{
}

Uses User::uses() const
{
    return m_uses;
}

void User::setUse( User& user )
{
    m_uses.add( libstdhl::make< Use >( *this, user ) );
}

void User::removeUse( const User& user )
{
    fprintf(
        stderr, "removeUse: %s in %s\n", user.name().c_str(), name().c_str() );

    m_uses.remove( std::remove_if(
        m_uses.begin(), m_uses.end(), [&user]( const Use::Ptr& element ) {
            return element->use() == user;
        } ) );
}

void User::replaceAllUsesWith( const Value::Ptr& value )
{
    for( auto u : uses() )
    {
        fprintf( stderr, "%s -> %s\n", u->def().name().c_str(),
            u->use().name().c_str() );

        if( auto instr = cast< Instruction >( u->use() ) )
        {
            instr->replace( u->def(), value );
        }
    }
}

u1 User::classof( Value const* obj )
{
    return obj->id() == classid() or Agent::classof( obj )
           or Rule::classof( obj ) or Derived::classof( obj )
           or Function::classof( obj ) or Builtin::classof( obj )
           or Instruction::classof( obj );
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

//
//  Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
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

#include "User.h"

#include "Agent.h"
#include "Builtin.h"
#include "Constant.h"
#include "Derived.h"
#include "Function.h"
#include "Instruction.h"
#include "Rule.h"

using namespace libcasm_ir;

const std::vector< Use::Ptr >& User::uses( void ) const
{
    return m_uses;
}

void User::setUse( User& user )
{
    m_uses.emplace_back( libstdhl::Memory::make< Use >( *this, user ) );
}

void User::removeUse( const User& user )
{
    fprintf( stderr, "removeUse: %s in %s\n", user.name().c_str(), name().c_str() );

    std::remove_if( m_uses.begin(), m_uses.end(), [&user]( const Use::Ptr& element ) {
        return element->use() == user;
    } );
}

void User::replaceAllUsesWith( const Value::Ptr& value )
{
    for( auto u : uses() )
    {
        fprintf( stderr, "%s -> %s\n", u->def().name().c_str(), u->use().name().c_str() );

        if( auto instr = cast< Instruction >( u->use() ) )
        {
            instr->replace( u->def(), value );
        }
    }
}

u1 User::classof( Value const* obj )
{
    return obj->id() == classid() or Agent::classof( obj ) or Rule::classof( obj ) or
           Derived::classof( obj ) or Function::classof( obj ) or Builtin::classof( obj ) or
           Instruction::classof( obj );
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

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

#ifndef _LIB_CASMIR_SPECIFICATION_H_
#define _LIB_CASMIR_SPECIFICATION_H_

#include "Value.h"

#include "Agent.h"
#include "Builtin.h"
#include "Constant.h"
#include "Derived.h"
#include "Function.h"
#include "Instruction.h"
#include "Rule.h"
#include "Statement.h"

namespace libcasm_ir
{
    class Specification final : public Value
    {
      public:
        using Ptr = std::shared_ptr< Specification >;

        Specification( const std::string& name );

        ~Specification( void ) = default;

        void setAgent( const Agent::Ptr& agent );
        Agent::Ptr agent( void ) const;

        void add( const Constant::Ptr& constant );
        void add( const Builtin::Ptr& builtin );
        void add( const Function::Ptr& function );
        void add( const Derived::Ptr& derived );
        void add( const Rule::Ptr& rule );

        Constants& constants( void );
        Builtins& builtins( void );
        Functions& functions( void );
        Deriveds& deriveds( void );
        Rules& rules( void );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::SPECIFICATION;
        }

        static u1 classof( Value const* obj );

      private:
        Agent::Ptr m_agent;

        Constants m_constants;
        Builtins m_builtins;
        Functions m_functions;
        Deriveds m_deriveds;
        Rules m_rules;
    };
}

#endif // _LIB_CASMIR_SPECIFICATION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

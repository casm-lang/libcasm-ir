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

#ifndef _LIB_CASMIR_USER_H_
#define _LIB_CASMIR_USER_H_

#include "Value.h"

namespace libcasm_ir
{
    class User;

    class Use : public CasmIR
    {
      public:
        using Ptr = std::shared_ptr< Use >;

        Use( Value& def, User& use )
        : m_def( def )
        , m_use( use )
        {
        }

        Value& def( void ) const
        {
            return m_def;
        }

        User& use( void ) const
        {
            return m_use;
        }

      private:
        Value& m_def;
        User& m_use;
    };

    class User : public Value
    {
      public:
        inline User( const Type::Ptr& type, Value::ID id = classid() )
        : Value( type, id )
        , m_uses()
        {
        }

        const std::vector< Use::Ptr >& uses( void ) const;

        void setUse( User& user );

        void removeUse( const User& user );

        void replaceAllUsesWith( const Value::Ptr& value );

        static inline Value::ID classid( void )
        {
            return Value::USER;
        }

        static u1 classof( Value const* obj );

      private:
        std::vector< Use::Ptr > m_uses;
    };
}

#endif // _LIB_CASMIR_USER_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

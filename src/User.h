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

#ifndef _LIBCASM_IR_USER_H_
#define _LIBCASM_IR_USER_H_

#include <libcasm-ir/Value>

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

#endif  // _LIBCASM_IR_USER_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

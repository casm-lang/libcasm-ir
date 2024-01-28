//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
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

#ifndef _LIBCASM_IR_RANGE_H_
#define _LIBCASM_IR_RANGE_H_

#include <libcasm-ir/Value>

namespace libcasm_ir
{
    class Constant;

    class Range final : public Value
    {
      public:
        using Ptr = std::shared_ptr< Range >;

        Range( const RangeType::Ptr& type );

        Range( const Value::Ptr& from, const Value::Ptr& to );

        Range( const Constant& from, const Constant& to );

        ~Range( void ) = default;

        Value::Ptr from( void ) const;

        Value::Ptr to( void ) const;

        std::string name( void ) const override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        void accept( Visitor& visitor ) override;

        static inline Value::ID classid( void )
        {
            return Value::RANGE;
        }

        static u1 classof( Value const* obj );

      private:
        Value::Ptr m_from;
        Value::Ptr m_to;
    };
}

#endif  // _LIBCASM_IR_RANGE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

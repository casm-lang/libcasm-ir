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

#include "../../../main.h"

using namespace libcasm_ir;

static const auto id = Value::ID::AS_BOOLEAN_BUILTIN;

#define TEST_( NAME, SIZE, FROM, TO )                                   \
    TEST( libcasm_ir__builtin_as_boolean_binary, SIZE##NAME )           \
    {                                                                   \
        const auto arg = BinaryConstant FROM;                           \
        const auto type = libstdhl::Memory::get< RelationType >(        \
            libstdhl::Memory::get< BooleanType >(),                     \
            Types( { libstdhl::Memory::get< BinaryType >( SIZE ) } ) ); \
                                                                        \
        Constant res;                                                   \
        Operation::execute( id, *type, res, arg );              \
        EXPECT_TRUE( res == BooleanConstant( TO ) );                    \
    }

TEST_( undef_at_undef, 1, ( 1 ), );
TEST_( false_at_zero, 1, ( 1, 0 ), false );
TEST_( true__at_one, 1, ( 1, 1 ), true );

TEST_( undef_at_undef, 8, ( 8 ), );
TEST_( false_at_zero, 8, ( 8, 0 ), false );
TEST_( true__at_one, 8, ( 8, 1 ), true );
TEST_( true__at_large, 8, ( 8, 0xee ), true );

TEST_( undef_at_undef, 23, ( 23 ), );
TEST_( false_at_zero, 23, ( 23, 0 ), false );
TEST_( true__at_one, 23, ( 23, 1 ), true );
TEST_( true__at_large, 23, ( 23, 0xfeed ), true );

TEST_( undef_at_undef, 48, ( 48 ), );
TEST_( false_at_zero, 48, ( 48, 0 ), false );
TEST_( true__at_one, 48, ( 48, 1 ), true );
TEST_( true__at_large, 48, ( 48, 0xfeedbeef ), true );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

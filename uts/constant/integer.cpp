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

#include "uts/main.h"

using namespace libcasm_ir;

static void libcasm_ir__constant_integer_test( i64 i )
{
    auto v = libstdhl::make< IntegerConstant >( i );
    ASSERT_TRUE( v != nullptr );

    EXPECT_STREQ( v->name().c_str(), std::to_string( i ).c_str() );
    EXPECT_STREQ( v->name().c_str(), std::to_string( v->value() ).c_str() );

    EXPECT_EQ( v->defined(), true );
    EXPECT_EQ( v->symbolic(), false );

    auto w = libstdhl::make< IntegerConstant >( i );
    ASSERT_TRUE( w != nullptr );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *w == *w );

    auto a = libstdhl::get< IntegerConstant >( i );
    auto b = libstdhl::get< IntegerConstant >( i );
    ASSERT_TRUE( a != nullptr );
    ASSERT_TRUE( b != nullptr );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );
}

TEST( libcasm_ir__constant_integer, create_range )
{
    for( i64 i = -128; i < 128; i++ )
    {
        libcasm_ir__constant_integer_test( i );
    }
}

TEST( libcasm_ir__constant_integer, create_random )
{
    for( u32 c = 0; c < 256; c++ )
    {
        i64 i = libstdhl::Random::uniform< i64 >();

        libcasm_ir__constant_integer_test( i );
    }
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

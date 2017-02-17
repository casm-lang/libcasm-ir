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

#include "gtest/gtest.h"

#include "libcasm-ir.h"

#include "libstdhl.h"

using namespace libcasm_ir;

static void test_constant_integer( Type::IntegerTy i )
{
    auto v = libstdhl::make< IntegerConstant >( i );

    EXPECT_STREQ( v->name(), std::to_string( i ).c_str() );
    EXPECT_STREQ( v->name(), std::to_string( v->value() ).c_str() );
    EXPECT_EQ( v->defined(), true );

    auto w = libstdhl::make< IntegerConstant >( i );

    ASSERT_TRUE( v != w );
    ASSERT_TRUE( *w == *w );

    auto a = libstdhl::get< IntegerConstant >( i );
    auto b = libstdhl::get< IntegerConstant >( i );

    ASSERT_TRUE( a == b );
    ASSERT_TRUE( *a == *b );
}

TEST( libcasm_ir__Constant_Integer, create_range )
{
    for( Type::IntegerTy i = -128; i < 128; i++ )
    {
        test_constant_integer( i );
    }
}

TEST( libcasm_ir__Constant_Integer, create_random )
{
    for( u32 c = 0; c < 256; c++ )
    {
        Type::IntegerTy i = libstdhl::Random::uniform< Type::IntegerTy >();

        test_constant_integer( i );
    }
}

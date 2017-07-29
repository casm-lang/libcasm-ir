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

static void libcasm_ir__constant_string_test( const std::string& s )
{
    auto v = libstdhl::make< StringConstant >( s );
    ASSERT_TRUE( v != nullptr );

    EXPECT_STREQ( v->name().c_str(), s.c_str() );

    EXPECT_EQ( v->defined(), true );
    EXPECT_EQ( v->symbolic(), false );

    auto w = libstdhl::make< StringConstant >( s );
    ASSERT_TRUE( w != nullptr );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *w == *w );

    auto a = libstdhl::get< StringConstant >( s );
    auto b = libstdhl::get< StringConstant >( s );
    ASSERT_TRUE( a != nullptr );
    ASSERT_TRUE( b != nullptr );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );
}

TEST( libcasm_ir__constant_string, copy_move_test )
{
    StringConstant x( "asdf" );
    StringConstant y = x;
    StringConstant z = std::move( x );
}

TEST( libcasm_ir__constant_string, create_hello_world_on_stack )
{
    StringConstant v( "hello world!" );

    EXPECT_STREQ( v.name().c_str(), "hello world!" );
}

TEST( libcasm_ir__constant_string, create )
{
    libcasm_ir__constant_string_test( "" );
}

TEST( libcasm_ir__constant_string, create_a )
{
    libcasm_ir__constant_string_test( "a" );
}

TEST( libcasm_ir__constant_string, create_az )
{
    libcasm_ir__constant_string_test( "az" );
}

TEST( libcasm_ir__constant_string, create_foo )
{
    libcasm_ir__constant_string_test( "foo" );
}

TEST( libcasm_ir__constant_string, create_dot_bar )
{
    libcasm_ir__constant_string_test( ".bar" );
}

TEST( libcasm_ir__constant_string, create_foobarqux )
{
    libcasm_ir__constant_string_test( "foobarqux" );
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

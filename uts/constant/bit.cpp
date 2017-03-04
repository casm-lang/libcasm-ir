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

static void libcasm_ir__constant_bit_test( u16 bitsize, u64 c )
{
    auto v = libstdhl::make< BitConstant >( bitsize, c );
    ASSERT_TRUE( v != nullptr );

    EXPECT_STREQ( v->name().c_str(), std::to_string( c ).c_str() );
    EXPECT_STREQ( v->name().c_str(), std::to_string( v->value() ).c_str() );
    if( strcmp( v->name().c_str(), std::to_string( v->value() ).c_str() ) )
    {
        printf( "%u, %lu\n", bitsize, c );
    }

    EXPECT_EQ( v->defined(), true );
    EXPECT_EQ( v->symbolic(), false );

    auto w = libstdhl::make< BitConstant >( bitsize, c );
    ASSERT_TRUE( w != nullptr );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *v == *w );

    auto a = libstdhl::get< BitConstant >( bitsize, c );
    auto b = libstdhl::get< BitConstant >( bitsize, c );

    ASSERT_TRUE( a != nullptr );
    ASSERT_TRUE( b != nullptr );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );
}

TEST( libcasm_ir__constant_bit, create_range_from_1_to_size_max )
{
    for( u16 c = 1; c <= BitType::SizeMax; c++ )
    {
        libcasm_ir__constant_bit_test( c, c );
    }
}

TEST( libcasm_ir__constant_bit, create_random )
{
    for( u64 c = 1; c <= BitType::SizeMax; c++ )
    {
        for( u64 i = 0; i < 100; i++ )
        {
            u64 value = libstdhl::Random::uniform< u64 >() % ( (u64)1 << c );

            libcasm_ir__constant_bit_test( c, value );
        }
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

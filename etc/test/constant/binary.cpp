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

#include "../main.h"

using namespace libcasm_ir;

static void libcasm_ir__constant_binary_test( u16 bitsize, u64 c )
{
    auto v = libstdhl::Memory::make< BinaryConstant >( bitsize, c );
    ASSERT_TRUE( v != nullptr );

    EXPECT_STREQ( v->name().c_str(), std::to_string( c ).c_str() );

    EXPECT_EQ( v->defined(), true );
    EXPECT_EQ( v->symbolic(), false );

    auto w = libstdhl::Memory::make< BinaryConstant >( bitsize, c );
    ASSERT_TRUE( w != nullptr );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *v == *w );

    auto a = libstdhl::Memory::get< BinaryConstant >( bitsize, c );
    auto b = libstdhl::Memory::get< BinaryConstant >( bitsize, c );

    ASSERT_TRUE( a != nullptr );
    ASSERT_TRUE( b != nullptr );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );
}

TEST( libcasm_ir__constant_binary, create_range_from_1_to_size_max )
{
    for( u16 c = 1; c <= BinaryType::SizeMax; c++ )
    {
        libcasm_ir__constant_binary_test( c, c );
    }
}

TEST( libcasm_ir__constant_binary, create_random )
{
    for( u64 c = 1; c <= BinaryType::SizeMax; c++ )
    {
        for( u64 i = 0; i < 100; i++ )
        {
            u64 value = libstdhl::Random::uniform< u64 >() % ( (u64)1 << c );

            libcasm_ir__constant_binary_test( c, value );
        }
    }
}

TEST( libcasm_ir__constant_binary, create_from_string )
{
    libstdhl::Memory::get< BinaryConstant >( "011101011", libstdhl::Type::Radix::BINARY );
    libstdhl::Memory::get< BinaryConstant >( "0111'0'1011", libstdhl::Type::Radix::BINARY );

    libstdhl::Memory::get< BinaryConstant >( "1234", libstdhl::Type::Radix::HEXADECIMAL );
    libstdhl::Memory::get< BinaryConstant >( "12'34", libstdhl::Type::Radix::HEXADECIMAL );

    libstdhl::Memory::get< BinaryConstant >( "F", libstdhl::Type::Radix::RADIX64 );
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

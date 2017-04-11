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

TEST( libcasm_ir__type_ranged_integer, make_and_get )
{
    const auto m = libstdhl::get< IntegerConstant >( -4 );
    const auto n = libstdhl::get< IntegerConstant >( 13 );

    const auto r = libstdhl::get< RangeType >( m, n );

    auto v = libstdhl::make< IntegerType >( r );
    ASSERT_TRUE( v != nullptr );

    EXPECT_STREQ( v->name().c_str(), "i[-4..13]" );
    EXPECT_STREQ( v->description().c_str(), "Integer'[-4..13]" );

    auto w = libstdhl::make< IntegerType >( r );
    ASSERT_TRUE( w != nullptr );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *w == *w );

    auto a = libstdhl::get< IntegerType >( r );
    auto b = libstdhl::get< IntegerType >( r );
    ASSERT_TRUE( a != nullptr );
    ASSERT_TRUE( b != nullptr );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );
}

TEST( libcasm_ir__type_ranged_integer, invalid_range )
{
    const auto m = libstdhl::get< IntegerConstant >( -4 );
    const auto n = libstdhl::get< IntegerConstant >( 13 );

    const auto r = libstdhl::get< RangeType >( n, m );

    EXPECT_THROW( { libstdhl::make< IntegerType >( r ); }, std::domain_error );
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

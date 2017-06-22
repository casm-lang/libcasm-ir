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

#include "src/Exception.h"

using namespace libcasm_ir;

TEST( libcasm_ir__type_ranged_integer, make_and_get )
{
    const auto m = libstdhl::get< IntegerConstant >( -4 );
    const auto n = libstdhl::get< IntegerConstant >( 13 );

    const auto R = libstdhl::make< Range >( m, n );

    const auto r = libstdhl::get< RangeType >( R );

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

    v->foreach( [](
        const Constant& constant ) { std::cerr << constant.name() << "\n"; } );

    std::cerr << v->choose().name() << "\n";
}

TEST( libcasm_ir__type_ranged_integer, valid_check )
{
    const auto a = libstdhl::get< IntegerConstant >( -4 );
    const auto b = libstdhl::get< IntegerConstant >( 13 );
    const auto R = libstdhl::make< Range >( a, b );
    const auto r = libstdhl::make< RangeType >( R );

    const auto x0 = libstdhl::get< IntegerConstant >( -4321 );
    const auto x1 = libstdhl::get< IntegerConstant >( -5 );
    const auto x2 = libstdhl::get< IntegerConstant >( -3 );
    const auto x3 = libstdhl::get< IntegerConstant >( 0 );
    const auto x4 = libstdhl::get< IntegerConstant >( 12 );
    const auto x5 = libstdhl::get< IntegerConstant >( 14 );
    const auto x6 = libstdhl::get< IntegerConstant >( 4321 );

    EXPECT_THROW( r->validate( *x0 );, ValidationException );
    EXPECT_THROW( r->validate( *x1 );, ValidationException );
    r->validate( *a );
    r->validate( *x2 );
    r->validate( *x3 );
    r->validate( *x4 );
    r->validate( *b );
    EXPECT_THROW( r->validate( *x5 );, ValidationException );
    EXPECT_THROW( r->validate( *x6 );, ValidationException );
}

TEST( DISABLED_libcasm_ir__type_ranged_integer, invalid_range )
{
    const auto m = libstdhl::get< IntegerConstant >( -4 );
    const auto n = libstdhl::get< IntegerConstant >( 13 );

    const auto R = libstdhl::make< Range >( m, n );

    const auto r = libstdhl::get< RangeType >( R );

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

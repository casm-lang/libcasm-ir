//
//  Copyright (C) 2015-2021 CASM Organization <https://casm-lang.org>
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

#include "../main.h"

using namespace libcasm_ir;

TEST( libcasm_ir__type_ranged_integer, make_and_get )
{
    const auto m = libstdhl::Memory::make< IntegerConstant >( -4 );
    const auto n = libstdhl::Memory::make< IntegerConstant >( 13 );

    const auto R = libstdhl::Memory::make< Range >( m, n );

    const auto r = libstdhl::Memory::get< RangeType >( R );

    auto v = libstdhl::Memory::make< IntegerType >( r );
    ASSERT_TRUE( v != nullptr );

    EXPECT_STREQ( v->name().c_str(), "i[-4..13]" );
    EXPECT_STREQ( v->description().c_str(), "Integer'[-4..13]" );

    auto w = libstdhl::Memory::make< IntegerType >( r );
    ASSERT_TRUE( w != nullptr );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *v == *w );

    auto a = libstdhl::Memory::get< IntegerType >( r );
    auto b = libstdhl::Memory::get< IntegerType >( r );
    ASSERT_TRUE( a != nullptr );
    ASSERT_TRUE( b != nullptr );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );

    v->foreach( []( const Constant& constant ) { std::cerr << constant.name() << "\n"; } );

    std::cerr << v->choose().name() << "\n";
}

TEST( libcasm_ir__type_ranged_integer, valid_check )
{
    const auto a = libstdhl::Memory::make< IntegerConstant >( -4 );
    const auto b = libstdhl::Memory::make< IntegerConstant >( 13 );
    const auto R = libstdhl::Memory::make< Range >( a, b );
    const auto r = libstdhl::Memory::make< RangeType >( R );

    const auto x0 = libstdhl::Memory::make< IntegerConstant >( -4321 );
    const auto x1 = libstdhl::Memory::make< IntegerConstant >( -5 );
    const auto x2 = libstdhl::Memory::make< IntegerConstant >( -3 );
    const auto x3 = libstdhl::Memory::make< IntegerConstant >( 0 );
    const auto x4 = libstdhl::Memory::make< IntegerConstant >( 12 );
    const auto x5 = libstdhl::Memory::make< IntegerConstant >( 14 );
    const auto x6 = libstdhl::Memory::make< IntegerConstant >( 4321 );

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
    const auto m = libstdhl::Memory::get< IntegerConstant >( -4 );
    const auto n = libstdhl::Memory::get< IntegerConstant >( 13 );

    const auto R = libstdhl::Memory::make< Range >( m, n );

    const auto r = libstdhl::Memory::get< RangeType >( R );

    EXPECT_THROW( { libstdhl::Memory::make< IntegerType >( r ); }, std::domain_error );
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

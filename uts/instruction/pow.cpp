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

#include "libcasm-ir.h"
#include "gtest/gtest.h"

using namespace libcasm_ir;

TEST( libcasm_ir__instruction_pow, make_and_get )
{
    {
        auto a = libstdhl::make< IntegerConstant >( 5 );
        auto i = PowInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
    {
        auto a = libstdhl::get< IntegerConstant >( 5 );
        auto i = PowInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
    {
        auto c = IntegerConstant( 5 );
        auto a = libstdhl::wrap< Constant >( c );
        auto i = PowInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
}

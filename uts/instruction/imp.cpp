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

TEST( libcasm_ir__instruction_imp, make_and_get )
{
    {
        auto a = libstdhl::make< BooleanConstant >( true );
        auto i = ImpInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
    {
        auto a = libstdhl::get< BooleanConstant >( true );
        auto i = ImpInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
    {
        auto c = BooleanConstant( true );
        auto a = libstdhl::wrap< Constant >( c );
        auto i = ImpInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
}

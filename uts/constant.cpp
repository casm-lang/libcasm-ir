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

TEST( libcasm_ir__cpp_constant, undef_ctor )
{
    IntegerConstant a;

    EXPECT_EQ( a.defined(), false );
}

TEST( libcasm_ir__cpp_constant, trivial_ctor )
{
    IntegerConstant a( 1234 );

    EXPECT_EQ( a.value().value(), 1234 );
    EXPECT_EQ( a.defined(), true );
}

TEST( libcasm_ir__cpp_constant, trivial_ctor_copy )
{
    IntegerConstant a( 1234 );
    IntegerConstant b( a );

    EXPECT_EQ( b.value().value(), 1234 );
    EXPECT_EQ( b.defined(), true );
}

TEST( libcasm_ir__cpp_constant, trivial_oper_copy )
{
    IntegerConstant a( 1234 );
    IntegerConstant b = a;

    EXPECT_EQ( b.value().value(), 1234 );
    EXPECT_EQ( b.defined(), true );
}

TEST( libcasm_ir__cpp_constant, trivial_ctor_move )
{
    IntegerConstant a( 1234 );
    IntegerConstant b( std::move( a ) );

    EXPECT_EQ( b.value().value(), 1234 );
    EXPECT_EQ( b.defined(), true );

    EXPECT_EQ( a.value().value(), 0 );
    EXPECT_EQ( a.defined(), false );
}

TEST( libcasm_ir__cpp_constant, trivial_oper_move )
{
    IntegerConstant a( 1234 );
    IntegerConstant b = std::move( a );

    EXPECT_EQ( b.value().value(), 1234 );
    EXPECT_EQ( b.defined(), true );

    EXPECT_EQ( a.value().value(), 0 );
    EXPECT_EQ( a.defined(), false );
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

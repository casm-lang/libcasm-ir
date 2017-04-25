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

#define LIB_CASMIR_UTS_CONSTANT_UNDEF( TYPE, ARGS )                            \
    TEST( libcasm_ir__constant_undef, TYPE##Type )                             \
    {                                                                          \
        const auto t = libstdhl::get< TYPE##Type >();                          \
        const auto u = Constant::undef( t );                                   \
                                                                               \
        EXPECT_TRUE( u == TYPE##Constant( ARGS ) );                            \
    }

LIB_CASMIR_UTS_CONSTANT_UNDEF( Integer, );
LIB_CASMIR_UTS_CONSTANT_UNDEF( Boolean, );
LIB_CASMIR_UTS_CONSTANT_UNDEF( Floating, );
LIB_CASMIR_UTS_CONSTANT_UNDEF( String, );

// TEST( libcasm_ir__constant_undef, IntegerType )
// {
//     const auto t = libstdhl::get< IntegerType >();
//     const auto u = Constant::undef( t );

//     EXPECT_TRUE( u == IntegerConstant() );
// }

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

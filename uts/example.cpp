//
//  Copyright (c) 2015-2016 CASM Organization
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

TEST( libcasm_ir, example )
{
    EXPECT_EXIT( libcasm_ir::AddInstruction i( 0, 0 ),
        ::testing::KilledBySignal( SIGABRT ), "" );
}

TEST( libcasm_ir, example2 )
{
    libcasm_ir::IntegerConstant* a = libcasm_ir::IntegerConstant::create( 5 );
    libcasm_ir::AddInstruction i( a, a );

    // printf( "--> %s\n", i.getType() );
}

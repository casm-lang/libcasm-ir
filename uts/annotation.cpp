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

TEST( libcasm_ir, example )
{
    EXPECT_THROW( { AddInstruction i( 0, 0 ); }, std::domain_error );
}

TEST( libcasm_ir, example2 )
{
    auto a = IntegerConstant( 5 );
    auto i = AddInstruction(
        libstdhl::wrap< Value >( a ), libstdhl::wrap< Value >( a ) );

    ASSERT_EQ( i.type().id(), a.type().id() );
}

TEST( libcasm_ir, example3 )
{
    Annotation::ResultTypes< AddInstruction >();
    Annotation::ResultTypes< SubInstruction >();
    Annotation::ResultTypes< DivInstruction >();
}

TEST( libcasm_ir, example4 )
{
    Annotation::ResultTypes< AsIntegerBuiltin >();
    Annotation::ResultTypes< AsEnumerationBuiltin >();
}

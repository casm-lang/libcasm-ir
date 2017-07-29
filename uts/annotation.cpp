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

TEST( libcasm_ir_Annotation, instructions )
{
    Annotation::find< AddInstruction >().resultTypes();
    Annotation::find< SubInstruction >().resultTypes();
    Annotation::find< MulInstruction >().resultTypes();
}

TEST( libcasm_ir_Annotation, builtin_as )
{
    Annotation::find< AsIntegerBuiltin >().resultTypes();
    Annotation::find< AsEnumerationBuiltin >().resultTypes();
}

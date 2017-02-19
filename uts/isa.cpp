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

#define ISA( INSTR, LHS, RHS )                                                 \
    TEST( libcasm_ir, isa_##INSTR )                                            \
    {                                                                          \
        auto a = LHS;                                                          \
        auto b = RHS;                                                          \
        auto i = INSTR(                                                        \
            libstdhl::wrap< Value >( a ), libstdhl::wrap< Value >( b ) );      \
        ASSERT_EQ( isa< INSTR >( i ), true );                                  \
    }

ISA( AddInstruction, IntegerConstant( 5 ), IntegerConstant( 2 ) )
ISA( SubInstruction, IntegerConstant( 5 ), IntegerConstant( 2 ) )
ISA( MulInstruction, IntegerConstant( 5 ), IntegerConstant( 2 ) )
ISA( ModInstruction, IntegerConstant( 5 ), IntegerConstant( 2 ) )
ISA( DivInstruction, IntegerConstant( 5 ), IntegerConstant( 2 ) )

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

#define ISA( T, ... )                                                          \
    TEST( libcasm_ir, isa_##T )                                                \
    {                                                                          \
        static T v( __VA_ARGS__ );                                             \
        ASSERT_EQ( isa< T >( v ), true );                                      \
    }

ISA( AddInstruction, Constant::getInteger( 5 ), Constant::getInteger( 2 ) )
ISA( SubInstruction, Constant::getInteger( 5 ), Constant::getInteger( 2 ) )
ISA( MulInstruction, Constant::getInteger( 5 ), Constant::getInteger( 2 ) )
ISA( ModInstruction, Constant::getInteger( 5 ), Constant::getInteger( 2 ) )
ISA( DivInstruction, Constant::getInteger( 5 ), Constant::getInteger( 2 ) )

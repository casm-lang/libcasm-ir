//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcasm-ir/graphs/contributors>
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

TEST( libcasm_ir__instruction_imp, make_and_get )
{
    {
        auto a = libstdhl::Memory::make< BooleanConstant >( true );
        auto i = ImpInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
    {
        auto a = libstdhl::Memory::get< BooleanConstant >( true );
        auto i = ImpInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
    {
        auto c = BooleanConstant( true );
        auto a = libstdhl::Memory::wrap< Constant >( c );
        auto i = ImpInstruction( a, a );

        EXPECT_STREQ( i.type().name().c_str(), a->type().name().c_str() );
        EXPECT_TRUE( i.type() == a->type() );
    }
}

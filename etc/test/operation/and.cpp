//
//  Copyright (C) 2015-2021 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
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

// | AND   | undef | false | true  | sym   |
// |-------+-------+-------+-------+-------|
// | undef | undef | false | undef | sym'  |
// | false | false | false | false | false |
// | true  | undef | false | true  | sym'  |
// | sym   | sym'  | false | sym'  | sym'  |

static const auto id = Value::ID::AND_INSTRUCTION;

static const auto type = libstdhl::Memory::get< RelationType >(
    libstdhl::Memory::get< BooleanType >(),
    Types( { libstdhl::Memory::get< BooleanType >(), libstdhl::Memory::get< BooleanType >() } ) );

#define CALC_( LHS, RHS )                    \
    const auto lhs = BooleanConstant( LHS ); \
    const auto rhs = BooleanConstant( RHS ); \
    Constant res;                            \
    Operation::execute( id, *type, res, lhs, rhs );

#define TEST_( NAME, RES, LHS, RHS )                                                             \
    TEST( libcasm_ir__instruction_and_boolean_boolean, NAME )                                    \
    {                                                                                            \
        CALC_( LHS, RHS );                                                                       \
        EXPECT_TRUE( res == BooleanConstant( RES ) );                                            \
        EXPECT_STREQ( res.description().c_str(), BooleanConstant( RES ).description().c_str() ); \
    }

// BENCHMARK( // TODO: PPA: FIXME: move this to the benchmarks!!!
//     libcasm_ir__instruction_and_boolean_boolean, one_word_no_wrap, 10, 10 )
// {
//     CALC_( true, false );
// }

TEST_( undef__at__undef__undef, , , );
TEST_( false__at__false__undef, false, false, );
TEST_( undef__at__true___undef, , true, );

TEST_( false__at__undef__false, false, , false );
TEST_( false__at__false__false, false, false, false );
TEST_( false__at__true___false, false, true, false );

TEST_( undef__at__undef__true_, , , true );
TEST_( false__at__false__true_, false, false, true );
TEST_( true___at__true___true_, true, true, true );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

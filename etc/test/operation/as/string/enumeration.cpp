//
//  Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
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

#include "../../../main.h"

using namespace libcasm_ir;

static const auto definition = libstdhl::Memory::make< Enumeration >(
    "ENUM", std::initializer_list< std::string >{ "A", "B", "C" } );

static const auto id = Value::ID::AS_STRING_BUILTIN;

static const auto type = libstdhl::Memory::get< RelationType >(
    libstdhl::Memory::get< StringType >(),
    Types( { libstdhl::Memory::make< EnumerationType >( definition ) } ) );

#define TEST_( NAME, TO, FROM )                                                \
    TEST( libcasm_ir__builtin_as_string_enumeration, NAME )                    \
    {                                                                          \
        const auto arg = EnumerationConstant FROM;                             \
        Constant res;                                                          \
        Operation::execute( id, *type, res, arg );                     \
        EXPECT_STREQ( res.description().c_str(),                               \
            StringConstant( TO ).description().c_str() );                      \
    }

TEST_( undef_at_undef, , ( definition ) );
TEST_( str_A_at_enumA, "A", ( definition, "A" ) );
TEST_( str_B_at_enumB, "B", ( definition, "B" ) );
TEST_( str_C_at_enumC, "C", ( definition, "C" ) );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

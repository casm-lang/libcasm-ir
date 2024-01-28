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

#include "main.h"

using namespace libcasm_ir;

TEST( libcasm_ir__Type, id_to_token_string )
{
    for( u64 c = 0; c < (u64)libcasm_ir::Type::Kind::_SIZE_; c++ )
    {
        libcasm_ir::Type::Kind id = (libcasm_ir::Type::Kind)c;

        switch( id )
        {
            // synthetic
            case libcasm_ir::Type::Kind::VOID:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Void" );
                break;
            }
            case libcasm_ir::Type::Kind::LABEL:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Label" );
                break;
            }
            case libcasm_ir::Type::Kind::LOCATION:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Location" );
                break;
            }
            case libcasm_ir::Type::Kind::RELATION:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Relation" );
                break;
            }
            // primitive
            case libcasm_ir::Type::Kind::BOOLEAN:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Boolean" );
                break;
            }
            case libcasm_ir::Type::Kind::INTEGER:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Integer" );
                break;
            }
            case libcasm_ir::Type::Kind::BINARY:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Binary" );
                break;
            }
            case libcasm_ir::Type::Kind::STRING:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "String" );
                break;
            }
            case libcasm_ir::Type::Kind::DECIMAL:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Decimal" );
                break;
            }
            case libcasm_ir::Type::Kind::RATIONAL:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Rational" );
                break;
            }
            // composed
            case libcasm_ir::Type::Kind::ENUMERATION:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Enumeration" );
                break;
            }
            case libcasm_ir::Type::Kind::RANGE:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Range" );
                break;
            }
            case libcasm_ir::Type::Kind::TUPLE:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Tuple" );
                break;
            }
            case libcasm_ir::Type::Kind::RECORD:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Record" );
                break;
            }
            case libcasm_ir::Type::Kind::LIST:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "List" );
                break;
            }
            case libcasm_ir::Type::Kind::OBJECT:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Object" );
                break;
            }
            // reference
            case libcasm_ir::Type::Kind::RULE_REFERENCE:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "RuleRef" );
                break;
            }
            case libcasm_ir::Type::Kind::FUNCTION_REFERENCE:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "FuncRef" );
                break;
            }
            // abstraction
            case libcasm_ir::Type::Kind::FILE:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "File" );
                break;
            }
            case libcasm_ir::Type::Kind::PORT:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "Port" );
                break;
            }

            case libcasm_ir::Type::Kind::_SIZE_:
            {
                EXPECT_STREQ( libcasm_ir::Type::token( id ).c_str(), "_SIZE_" );
                break;
            }
        }
    }
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

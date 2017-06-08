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

TEST( libcasm_ir__Type, id_to_token_string )
{
    for( u64 c = 0; c < Type::_TOP_; c++ )
    {
        Type::ID id = (Type::ID)c;

        switch( id )
        {
            case Type::_BOTTOM_:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "_BOTTOM_" );
                break;
            }
            // synthetic
            case Type::VOID:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Void" );
                break;
            }
            case Type::LABEL:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Label" );
                break;
            }
            case Type::LOCATION:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Location" );
                break;
            }
            case Type::RELATION:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Relation" );
                break;
            }
            // primitive
            case Type::BOOLEAN:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Boolean" );
                break;
            }
            case Type::INTEGER:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Integer" );
                break;
            }
            case Type::BIT:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Bit" );
                break;
            }
            case Type::STRING:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "String" );
                break;
            }
            case Type::FLOATING:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Floating" );
                break;
            }
            case Type::RATIONAL:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Rational" );
                break;
            }
            // composed
            case Type::ENUMERATION:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Enumeration" );
                break;
            }
            case Type::RANGE:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Range" );
                break;
            }
            case Type::TUPLE:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "Tuple" );
                break;
            }
            case Type::LIST:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "List" );
                break;
            }
            // reference
            case Type::RULE_REFERENCE:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "RuleRef" );
                break;
            }
            case Type::FUNCTION_REFERENCE:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "FuncRef" );
                break;
            }
            // abstract
            case Type::FILE:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "File" );
                break;
            }
            case Type::_TOP_:
            {
                EXPECT_STREQ( Type::token( id ).c_str(), "_TOP_" );
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

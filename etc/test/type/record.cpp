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

#include "../main.h"

using namespace libcasm_ir;

TEST( libcasm_ir__type_record, make_and_get )
{
    auto i = libstdhl::Memory::make< IntegerType >();
    auto t = Types( { i, i, i } );

    auto v = libstdhl::Memory::make< RecordType >( t, std::vector< std::string >{ "a", "b", "c" } );
    ASSERT_TRUE( v != nullptr );

    EXPECT_STREQ( v->name().c_str(), "r<a:i,b:i,c:i>" );
    EXPECT_STREQ( v->description().c_str(), "( a : Integer, b : Integer, c : Integer )" );

    auto w = libstdhl::Memory::make< RecordType >( t, std::vector< std::string >{ "a", "b", "c" } );
    ASSERT_TRUE( w != nullptr );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *v == *w );

    auto a = libstdhl::Memory::get< RecordType >( t, std::vector< std::string >{ "a", "b", "c" } );
    auto b = libstdhl::Memory::get< RecordType >( t, std::vector< std::string >{ "a", "b", "c" } );
    ASSERT_TRUE( a != nullptr );
    ASSERT_TRUE( b != nullptr );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );

    v->foreach( []( const Constant& constant ) { std::cerr << constant.name() << "\n"; } );

    std::cerr << v->choose().name() << "\n";
}

TEST( libcasm_ir__type_record, contains )
{
    auto i = libstdhl::Memory::make< IntegerType >();
    auto b = libstdhl::Memory::make< BooleanType >();

    // record type as base type
    auto types = Types( { i, i, i } );
    auto names = std::vector< std::string >{ "a", "b", "c" };
    const auto baseType = RecordType( types, names );

    // contains in-order
    const auto inOrderType = RecordType( types, names );
    EXPECT_TRUE( baseType.contains( inOrderType ) );

    // contains out-of-order
    names = std::vector< std::string >{ "c", "a", "b" };
    const auto outOfOrderType = RecordType( types, names );
    EXPECT_TRUE( baseType.contains( outOfOrderType ) );

    // contains partial
    types = Types( { i } );
    names = std::vector< std::string >{ "b" };
    const auto partialType = RecordType( types, names );
    EXPECT_TRUE( baseType.contains( partialType ) );

    // does not contain this record type, because
    // record element types do not match size and
    // record element names do not match
    types = Types( { i, i } );
    names = std::vector< std::string >{ "d", "e" };
    const auto notPartialType = RecordType( types, names );
    EXPECT_FALSE( baseType.contains( notPartialType ) );

    // does not contain this record type, because
    // record element types match in kind and size but
    // record element names do not match
    types = Types( { i, i, i } );
    names = std::vector< std::string >{ "a", "z", "c" };
    const auto differentNamesType = RecordType( types, names );
    EXPECT_FALSE( baseType.contains( differentNamesType ) );

    // does not contain this record type, because
    // record element types do not match in kind, but
    // they match in record element names
    types = Types( { i, b, i } );
    names = std::vector< std::string >{ "a", "b", "c" };
    const auto differentTypesType = RecordType( types, names );
    EXPECT_FALSE( baseType.contains( differentTypesType ) );

    // does not contain this record type, because
    // record element types do not match in kind and
    // they do not match in record element names at all
    types = Types( { i, i, b } );
    names = std::vector< std::string >{ "f", "b", "h" };
    const auto differentTypesAndNamesType = RecordType( types, names );
    EXPECT_FALSE( baseType.contains( differentTypesAndNamesType ) );

    // does not contain this record type, because
    // this record type elements are greater then the record
    // element type/name size from the base record type
    types = Types( { i, i, i, b } );
    names = std::vector< std::string >{ "a", "b", "c", "d" };
    const auto biggerElementSizeType = RecordType( types, names );
    EXPECT_FALSE( baseType.contains( biggerElementSizeType ) );
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

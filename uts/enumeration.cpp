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

TEST( libcasm_ir__enumeration, example )
{
    const std::vector< std::string > elements = { "foo", "bar", "baz" };
    const auto e = libstdhl::make< Enumeration >( "example", elements );

    ASSERT_TRUE( e != nullptr );

    for( std::size_t c = 0; c < elements.size(); c++ )
    {
        EXPECT_EQ( e->encode( elements[ c ] ), c );

        EXPECT_STREQ( e->decode( c ).c_str(), elements[ c ].c_str() );
    }

    EXPECT_THROW( { e->encode( "enum" ); }, std::domain_error );

    EXPECT_THROW( { e->decode( 123 ); }, std::domain_error );
}

TEST( libcasm_ir__enumeration, invalid_same_values_are_not_allowed )
{
    const std::vector< std::string > elements
        = { "foo", "bar", "baz", "bar", "foo" };

    EXPECT_THROW(
        {
            const auto e = libstdhl::make< Enumeration >( "example", elements );
        },
        std::domain_error );
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

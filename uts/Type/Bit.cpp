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

#include "gtest/gtest.h"

#include "libcasm-ir.h"

using namespace libcasm_ir;

TEST( libcasm_ir__Type_Bit, make_valid )
{
    std::unordered_map< std::string, BitType::Ptr > cache;

    for( u16 c = 1; c <= BitType::SizeMax; c++ )
    {
        auto v = make< BitType >( c );
        EXPECT_TRUE( v != nullptr );

        auto r = cache.find( v->getName() );
        EXPECT_TRUE( r == cache.end() );
        cache[ v->getName() ] = v;

        EXPECT_EQ( v->getSize(), c );

        EXPECT_EQ( v->getID(), Type::BIT );
    }
}

TEST( libcasm_ir__Type_Bit, get_valid )
{
    // std::unordered_map< std::string, BitType::Ptr > cache;

    // for( u16 c = 1; c <= BitType::SizeMax; c++ )
    // {
    //     auto v = make< BitType >( c );
    //     EXPECT_TRUE( v != nullptr );

    //     auto r = cache.find( v->getName() );
    //     EXPECT_TRUE( r == cache.end() );
    //     cache[ v->getName() ] = v;

    //     EXPECT_EQ( v->getSize(), c );
    // }
}

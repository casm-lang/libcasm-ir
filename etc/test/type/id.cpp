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

TEST( libcasm_ir__type_id, sequence )
{
    auto v = libstdhl::Memory::get< VoidType >();
    auto i = libstdhl::Memory::get< IntegerType >();
    auto s = libstdhl::Memory::get< StringType >();
    auto b = libstdhl::Memory::get< BooleanType >();

    auto vid = v->id();
    auto iid = i->id();
    auto sid = s->id();
    auto bid = b->id();

    std::cout << std::to_string( vid ) << "\n";
    std::cout << std::to_string( iid ) << "\n";
    std::cout << std::to_string( sid ) << "\n";
    std::cout << std::to_string( bid ) << "\n";

    EXPECT_EQ( v.get(), libcasm_ir::Type::fromID( vid ).get() );
    EXPECT_EQ( i.get(), libcasm_ir::Type::fromID( iid ).get() );
    EXPECT_EQ( s.get(), libcasm_ir::Type::fromID( sid ).get() );
    EXPECT_EQ( b.get(), libcasm_ir::Type::fromID( bid ).get() );
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

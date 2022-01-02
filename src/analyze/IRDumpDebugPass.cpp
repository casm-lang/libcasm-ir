//
//  Copyright (C) 2015-2022 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//                <https://github.com/casm-lang/libcasm-ir/graphs/contributors>
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

#include "IRDumpDebugPass.h"

#include <libcasm-ir/Specification>

#include <libcasm-ir/analyze/ConsistencyCheckPass>

#include <libpass/PassLogger>
#include <libpass/PassRegistry>
#include <libpass/PassResult>
#include <libpass/PassUsage>

using namespace libcasm_ir;

char IRDumpDebugPass::id = 0;

static libpass::PassRegistration< IRDumpDebugPass > PASS(
    "IRDumpDebugPass",
    "outputs from the CASM IR an ASCII representation for debugging",
    "ir-dump-debug",
    0 );

static inline std::string indention( Value& value );

//
//
// IRDumpDebugPass
//

void IRDumpDebugPass::usage( libpass::PassUsage& pu )
{
    pu.require< ConsistencyCheckPass >();
}

u1 IRDumpDebugPass::run( libpass::PassResult& pr )
{
    libpass::PassLogger log( &id, stream() );

    const auto& data = pr.input< ConsistencyCheckPass >();
    const auto& specification = data->specification();

    try
    {
        specification->iterate( Traversal::PREORDER, [this, &log]( Value& value ) {
            log.info( "%p: %s%s", &value, indention( value ).c_str(), value.dump().c_str() );
        } );
    }
    catch( ... )
    {
        log.error( "unsuccessful dump of specification" );
        return false;
    }

    return true;
}

static inline std::string indention( Value& value )
{
    std::string ind = "";
    u8 cnt = 0;
    Value* p = ( &value );
    while( p != 0 )
    {
        if( isa< ExecutionSemanticsBlock >( p ) )
        {
            p = (Value*)( (ExecutionSemanticsBlock*)p )->scope().get();
        }
        else if( isa< Instruction >( p ) )
        {
            p = (Value*)( (Instruction*)p )->statement().get();
        }
        else if( isa< Statement >( p ) )
        {
            p = (Value*)( (Statement*)p )->scope().get();
        }
        else
        {
            break;
            assert( 0 );
        }

        cnt++;
        ind += "  ";
    }

    return ind;
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

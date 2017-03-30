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

#include "IRDumpDebugPass.h"

using namespace libcasm_ir;

char IRDumpDebugPass::id = 0;

static libpass::PassRegistration< IRDumpDebugPass > PASS( "IRDumpDebugPass",
    "outputs from the CASM IR an ASCII representation for debugging",
    "ir-dump-debug", 0 );

void IRDumpDebugPass::usage( libpass::PassUsage& pu )
{
    pu.require< ConsistencyCheckPass >();
}

u1 IRDumpDebugPass::run( libpass::PassResult& pr )
{
    libpass::PassLogger log( &id, stream() );

    auto data = pr.result< ConsistencyCheckPass >();
    assert( data );

    try
    {
        data->specification()->iterate(
            Traversal::PREORDER, [this, &log]( Value& value ) {
                log.info( "%p: %s%s", &value, this->indention( value ).c_str(),
                    value.dump().c_str() );
            } );
    }
    catch( ... )
    {
        log.error( "unsuccessful dump of specification" );
        return false;
    }

    return true;
}

std::string IRDumpDebugPass::indention( Value& value ) const
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

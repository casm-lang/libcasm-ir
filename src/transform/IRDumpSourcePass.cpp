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

#include "IRDumpSourcePass.h"

#include "libcasm-ir.h"

using namespace libcasm_ir;

char IRDumpSourcePass::id = 0;

static libpass::PassRegistration< IRDumpSourcePass > PASS(
    "CASM IR to Source Code Pass",
    "translates the CASM IR to the ASCII source code representation", "ir-dump",
    0 );

static FILE* stream = stdout;

static const char* indention( Value& value )
{
    if( isa< Instruction >( value ) )
    {
        return "  ";
    }
    else
    {
        return "";
    }
}

u1 IRDumpSourcePass::run( libpass::PassResult& pr )
{
    auto data = pr.result< ConsistencyCheckPass >();
    assert( data );

    data->specification()->iterate( Traversal::PREORDER, []( Value& value,
                                                             Context& ) {
        if( isa< Constant >( value ) )
        {
            static u1 first = true;

            if( first )
            {
                first = false;
                fprintf( stream, "\n" );
            }

            fprintf( stream, "%s = %s %s\n", value.label().c_str(),
                value.type().name().c_str(), value.name().c_str() );
        }
        else if( isa< Function >( value ) )
        {
            static u1 first = true;

            if( first )
            {
                first = false;
                fprintf( stream, "\n" );
            }

            fprintf( stream, "%s = %s\n", value.name().c_str(),
                value.type().name().c_str() );
        }
        else if( isa< Builtin >( value ) )
        {
            static u1 first = true;

            if( first )
            {
                first = false;
                fprintf( stream, "\n" );
            }

            fprintf( stream, "%s = %s %s\n", value.label().c_str(),
                value.type().name().c_str(), value.name().c_str() );
        }
        else if( isa< Derived >( value ) )
        {
            fprintf( stream,
                "\n"
                "%s %s = \n"
                "[\n",
                value.name().c_str(), value.type().name().c_str() );
        }
        else if( isa< Rule >( value ) )
        {
            fprintf( stream,
                "\n"
                "%s %s = \n"
                "{\n",
                value.name().c_str(), value.type().name().c_str() );
        }
        else if( isa< Statement >( value ) )
        {
            // Statement& stmt = static_cast< Statement& >( value );

            const char* nline = "\n";
            const char* label = value.label().c_str();
            std::string scope = "";

            fprintf( stream, "%s%s%s: %s\n", nline, indention( value ),
                &label[ 1 ], scope.c_str() );
        }
        else if( auto instr = cast< Instruction >( value ) )
        {
            if( isa< ForkInstruction >( instr ) )
            {
                fprintf( stream,
                    "%s%s %s %s %s\n",
                    indention( *instr ),
                    instr->name().c_str(),
                    instr->statement()->scope()->name().c_str(),
                    instr->statement()->scope()->type().name().c_str(),
                    instr->statement()->scope()->label().c_str() );
            }
            else if( isa< MergeInstruction >( instr ) )
            {
                fprintf( stream, "%s%s %s %s %s\n", indention( *instr ),
                    instr->statement()->scope()->name().c_str(),
                    instr->name().c_str(),
                    instr->statement()->scope()->type().name().c_str(),
                    instr->statement()->scope()->label().c_str() );
            }
            else
            {
                std::string tmp = "";
                u1 first = true;

                for( auto operand : instr->operands() )
                {
                    if( not first )
                    {
                        tmp += ", ";
                    }
                    else
                    {
                        first = false;
                    }

                    tmp += operand->type().name();
                    tmp += " ";
                    tmp += operand->label();
                }

                std::string uses = "{";
                for( auto u : instr->uses() )
                {
                    uses += u->use().label();
                    uses += " : ";
                    uses += u->use().name();
                    uses += ", ";
                }
                uses += "}";

                fprintf( stream, "%s%s = %s %s                 ;; uses = %s\n",
                    indention( *instr ), instr->label().c_str(),
                    instr->name().c_str(), tmp.c_str(), uses.c_str() );
            }

            auto scope = instr->statement()->scope();

            if( not scope )
            {
                // end of derived!
                fprintf( stream, "]\n" );
            }
            else
            {
                if( scope->scope() == 0 ) // and scope->exitBlock() == stmt )
                {
                    // reached end of rule, this blk is the top level
                    // exec.sem.blk
                    fprintf( stream, "}\n" );
                }
            }
        }
    } );

    return true;
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
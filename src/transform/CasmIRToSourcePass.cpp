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

#include "CasmIRToSourcePass.h"

#include "libcasm-ir.h"

using namespace libcasm_ir;

char CasmIRToSourcePass::id = 0;

static libpass::PassRegistration< CasmIRToSourcePass > PASS(
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

bool CasmIRToSourcePass::run( libpass::PassResult& pr )
{
    Specification* value = (Specification*)pr.getResult< CasmIRDumpPass >();
    assert( value );

    value->iterate( Traversal::PREORDER, []( Value& value, Context& ) {
        if( isa< Constant >( value ) )
        {
            static u1 first = true;

            if( first )
            {
                first = false;
                fprintf( stream, "\n" );
            }

            fprintf( stream, "%s = %s %s\n", value.getLabel(),
                value.getType()->getName(), value.getName() );
        }
        else if( isa< Agent >( value ) )
        {
            static u1 first = true;

            if( first )
            {
                first = false;
                fprintf( stream, "\n" );
            }

            Agent& val = static_cast< Agent& >( value );

            fprintf( stream, "%s = init %s %s\n", value.getLabel(),
                val.getInitRule()->getType()->getName(),
                val.getInitRule()->getName() );
        }
        else if( isa< Function >( value ) )
        {
            static u1 first = true;

            if( first )
            {
                first = false;
                fprintf( stream, "\n" );
            }

            fprintf( stream, "%s = %s\n", value.getName(),
                value.getType()->getName() );
        }
        else if( isa< Builtin >( value ) )
        {
            static u1 first = true;

            if( first )
            {
                first = false;
                fprintf( stream, "\n" );
            }

            fprintf( stream, "%s = %s %s\n", value.getLabel(),
                value.getType()->getName(), value.getName() );
        }
        else if( isa< Derived >( value ) )
        {
            fprintf( stream,
                "\n"
                "%s %s = \n"
                "[\n",
                value.getName(), value.getType()->getName() );
        }
        else if( isa< Rule >( value ) )
        {
            fprintf( stream,
                "\n"
                "%s %s = \n"
                "{\n",
                value.getName(), value.getType()->getName() );
        }
        else if( isa< Statement >( value ) )
        {
            Statement& stmt = static_cast< Statement& >( value );

            const char* nline = "\n";
            const char* label = value.getLabel();
            std::string scope = "";

            if( not stmt.getScope() )
            {
                nline = "";
            }
            else
            {
                if( stmt.getScope()->getEntryBlock() == &stmt )
                {
                    label = stmt.getScope()->getLabel();
                }

                if( stmt.getScope()->getParent() )
                {
                    scope += stmt.getScope()->getParent()->getLabel();
                }
                else
                {
                    if( stmt.getScope()->getEntryBlock() == &stmt )
                    {
                        nline = "";
                        scope = "entry";
                    }
                    else if( stmt.getScope()->getExitBlock() == &stmt )
                    {
                        scope = "exit";
                    }
                    else
                    {
                        stmt.getScope()->getLabel();
                    }
                }
            }

            fprintf( stream, "%s%s%s: %s\n", nline, indention( value ),
                &label[ 1 ], scope.c_str() );
        }
        else if( isa< Instruction >( value ) )
        {
            Instruction& instr = static_cast< Instruction& >( value );

            if( isa< ForkInstruction >( instr ) )
            {
                fprintf( stream,
                    "%s%s %s %s %s\n",
                    indention( instr ),
                    instr.getName(),
                    instr.getStatement()->getScope()->getName(),
                    instr.getStatement()->getScope()->getType()->getName(),
                    instr.getStatement()->getScope()->getLabel() );
            }
            else if( isa< MergeInstruction >( instr ) )
            {
                fprintf( stream, "%s%s %s %s %s\n", indention( instr ),
                    instr.getStatement()->getScope()->getName(),
                    instr.getName(),
                    instr.getStatement()->getScope()->getType()->getName(),
                    instr.getStatement()->getScope()->getLabel() );
            }
            else
            {
                std::string tmp = "";
                u1 first = true;

                for( auto v : instr.getValues() )
                {
                    if( not first )
                    {
                        tmp += ", ";
                    }
                    else
                    {
                        first = false;
                    }

                    tmp += v->getType()->getName();
                    tmp += " ";
                    tmp += v->getLabel();
                }

                fprintf( stream, "%s%s = %s %s\n", indention( instr ),
                    instr.getLabel(), instr.getName(), tmp.c_str() );
            }

            const Statement* stmt = instr.getStatement();
            assert( stmt );
            ExecutionSemanticsBlock* scope = stmt->getScope();

            if( not scope )
            {
                // end of derived!
                fprintf( stream, "]\n" );
            }
            else
            {
                if( scope->getScope() == 0 and scope->getExitBlock() == stmt )
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

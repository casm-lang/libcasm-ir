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
#include "Specification.h"

using namespace libcasm_ir;

char CasmIRToSourcePass::id = 0;

static libpass::PassRegistration< CasmIRToSourcePass > PASS(
    "CASM IR to Source Code Pass",
    "translates the CASM IR to the ASCII source code representation", "ir-dump",
    0 );

static FILE* stream = stdout;

static const char* indention( Value& value )
{
    string ind = "";
    Value* p = ( &value );
    while( p != 0 )
    {
        if( Value::isa< ExecutionSemanticsBlock >( p ) )
        {
            p = (Value*)( (ExecutionSemanticsBlock*)p )->getScope();

            if( p == 0 )
            {
                continue;
            }
        }
        else if( Value::isa< Instruction >( p ) )
        {
            u1 skip = Value::isa< ForkInstruction >( p )
                      or Value::isa< MergeInstruction >( p );

            p = (Value*)( (Instruction*)p )->getStatement();

            if( skip )
            {
                continue;
            }
        }
        else if( Value::isa< Statement >( p ) )
        {
            p = (Value*)( (Statement*)p )->getScope();
            continue;
        }
        else
        {
            assert( 0 );
        }

        ind += "  ";
    }

    return libstdhl::Allocator::string( ind );
}

template < class T >
static void constant(
    FILE* stream, ConstantOf< T >& value, const char* value_str )
{
}

bool CasmIRToSourcePass::run( libpass::PassResult& pr )
{
    Specification* value = (Specification*)pr.getResult< CasmIRDumpPass >();
    assert( value );

    value->iterate( Traversal::PREORDER, []( Value* value_ptr ) {
        assert( value_ptr );
        Value& value = *value_ptr;

        if( Value::isa< Constant >( value ) )
        {
            fprintf( stream, "@%s = %s %s\n", value.getLabel(),
                value.getType()->getName(), value.getName() );
        }
        else if( Value::isa< Agent >( value ) )
        {
            // Agent& val = static_cast< Agent& >( value );

            fprintf(
                stream, "@%s = init ?TODO? ;; agent\n", value.getLabel() ); //,
            // val.getInitRule()->getName() ); // getType()->getName() );
        }
        else if( Value::isa< Function >( value ) )
        {
            fprintf( stream, "@%s = %s ;; function '%s'\n", value.getLabel(),
                value.getType()->getName(), value.getName() );
        }
        else if( Value::isa< Builtin >( value ) )
        {
            fprintf( stream, "@%s = %s %s\n", value.getLabel(),
                value.getType()->getName(), value.getName() );
        }
        else if( Value::isa< Rule >( value ) )
        {
            fprintf( stream, "@%s %s = \n", value.getName(),
                value.getType()->getName() );
        }
        else if( Value::isa< Statement >( value ) )
        {
            Statement& val = static_cast< Statement& >( value );

            if( val.getInstructions().size() == 1
                and ( Value::isa< ForkInstruction >(
                          val.getInstructions()[ 0 ] )
                        or Value::isa< MergeInstruction >(
                               val.getInstructions()[ 0 ] ) ) )
            {
                return;
            }

            fprintf( stream, "%s$%s: ;; %s\n", indention( val ),
                value.getLabel(), value.getName() );
        }
        else if( Value::isa< Instruction >( value ) )
        {
            Instruction& val = static_cast< Instruction& >( value );

            if( Value::isa< ForkInstruction >( val ) )
            {
                fprintf( stream, "%s%s\n", indention( val ),
                    val.getStatement()->getScope()->isParallel() ? "{" : "{|" );
            }
            else if( Value::isa< MergeInstruction >( val ) )
            {
                fprintf( stream, "%s%s\n", indention( val ),
                    val.getStatement()->getScope()->isParallel() ? "}" : "|}" );
            }
            else
            {
                std::string tmp = "";
                u1 first = true;

                for( auto v : val.getValues() )
                {
                    if( not first )
                    {
                        tmp += ", ";
                    }
                    else
                    {
                        first = false;
                    }

                    if( Value::isa< Instruction >( v ) )
                    {
                        tmp += "%";
                    }
                    else
                    {
                        tmp += "@";
                    }
                    tmp += v->getLabel();
                }

                fprintf( stream, "%s%%%s = %s %s\n", indention( val ),
                    val.getLabel(), &val.getName()[ 1 ], tmp.c_str() );
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

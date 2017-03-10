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

static const char* name = 0;

u1 IRDumpSourcePass::run( libpass::PassResult& pr )
{
    name = libpass::PassRegistry::passInfo< IRDumpSourcePass >().name();

    libstdhl::Log::info( "%s: starting", name );

    auto data = pr.result< ConsistencyCheckPass >();
    assert( data );

    try
    {
        m_first.clear();

        data->specification()->iterate( Traversal::PREORDER, this );
    }
    catch( ... )
    {
        libstdhl::Log::error( "unsuccessful dump of specification" );
        return false;
    }

    libstdhl::Log::info( "%s: stopping", name );

    return true;
}

std::string IRDumpSourcePass::indention( Value& value ) const
{
#define INDENT "  "

    if( isa< Statement >( value ) )
    {
        return INDENT;
    }
    else if( isa< Instruction >( value ) )
    {
        return INDENT INDENT;
    }
    else
    {
        return "";
    }
}

//
// Specification
//

void IRDumpSourcePass::visit_prolog( Specification& value, Context& )
{
}
void IRDumpSourcePass::visit_epilog( Specification& value, Context& )
{
}

//
// Agent
//

void IRDumpSourcePass::visit_prolog( Agent& value, Context& )
{
}
void IRDumpSourcePass::visit_epilog( Agent& value, Context& )
{
}

//
// Builtin
//

void IRDumpSourcePass::visit_prolog( Builtin& value, Context& )
{
    auto result = m_first.find( value.id() );
    if( result == m_first.end() )
    {
        m_first.insert( value.id() );
        fprintf( stream, "\n" );
    }

    fprintf( stream, "%s = %s %s\n", value.label().c_str(),
        value.type().name().c_str(), value.name().c_str() );
}
void IRDumpSourcePass::visit_epilog( Builtin& value, Context& )
{
}

//
// Function
//

void IRDumpSourcePass::visit_prolog( Function& value, Context& )
{
    auto result = m_first.find( value.id() );
    if( result == m_first.end() )
    {
        m_first.insert( value.id() );
        fprintf( stream, "\n" );
    }

    fprintf( stream, "%s = %s\n", value.name().c_str(),
        value.type().name().c_str() );
}
void IRDumpSourcePass::visit_epilog( Function& value, Context& )
{
}

//
// Derived
//

void IRDumpSourcePass::visit_prolog( Derived& value, Context& )
{
    fprintf( stream,
        "\n"
        "%s %s = \n",
        value.name().c_str(), value.type().name().c_str() );
}
void IRDumpSourcePass::visit_interlog( Derived& value, Context& )
{
    fprintf( stream, "[\n" );
}
void IRDumpSourcePass::visit_epilog( Derived& value, Context& )
{
    fprintf( stream, "]\n" );
}

//
// Rule
//

void IRDumpSourcePass::visit_prolog( Rule& value, Context& )
{
    fprintf( stream,
        "\n"
        "%s %s = \n",
        value.name().c_str(), value.type().name().c_str() );
}
void IRDumpSourcePass::visit_interlog( Rule& value, Context& )
{
    fprintf( stream, "{\n" );
}
void IRDumpSourcePass::visit_epilog( Rule& value, Context& )
{
    fprintf( stream, "}\n" );
}

//
// ParallelBlock
//

void IRDumpSourcePass::visit_prolog( ParallelBlock& value, Context& )
{
}
void IRDumpSourcePass::visit_epilog( ParallelBlock& value, Context& )
{
}

//
// SequentialBlock
//

void IRDumpSourcePass::visit_prolog( SequentialBlock& value, Context& )
{
}
void IRDumpSourcePass::visit_epilog( SequentialBlock& value, Context& )
{
}

//
// TrivialStatement
//

void IRDumpSourcePass::visit_prolog( TrivialStatement& value, Context& )
{
    const char* nline = "\n";
    const char* label = value.label().c_str();
    const char* scope = value.scope()->label().c_str();

    if( value.scope()->entry().get() == &value )
    {
        label = value.scope()->label().c_str();

        if( not value.scope()->scope() )
        {
            scope = "entry";
            nline = "";
        }
        else
        {
            scope = value.scope()->scope()->label().c_str();
        }
    }
    else if( value.scope()->exit().get() == &value )
    {
        label = value.scope()->label().c_str();

        if( not value.scope()->scope() )
        {
            scope = "exit";
        }
        else
        {
            scope = value.scope()->scope()->label().c_str();
        }
    }

    fprintf( stream, "%s%s%s: %s\n", nline, indention( value ).c_str(),
        &label[ 1 ], scope );
}
void IRDumpSourcePass::visit_epilog( TrivialStatement& value, Context& )
{
}

//
// BranchStatement
//

void IRDumpSourcePass::visit_prolog( BranchStatement& value, Context& )
{
}
void IRDumpSourcePass::visit_interlog( BranchStatement& value, Context& )
{
}
void IRDumpSourcePass::visit_epilog( BranchStatement& value, Context& )
{
}

//
// Instruction
//

void IRDumpSourcePass::visit( Instruction& value ) const
{
    if( isa< ForkInstruction >( value ) or isa< MergeInstruction >( value ) )
    {
        fprintf( stream,
            "%s%s %s %s %s\n",
            indention( value ).c_str(),
            value.name().c_str(),
            value.statement()->scope()->name().c_str(),
            value.statement()->scope()->type().name().c_str(),
            value.statement()->scope()->label().c_str() );
    }
    else
    {
        std::string tmp = "";
        u1 first = true;

        for( auto operand : value.operands() )
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
        for( auto u : value.uses() )
        {
            uses += u->use().label();
            uses += " : ";
            uses += u->use().name();
            uses += ", ";
        }
        uses += "}";

        fprintf( stream, "%s%s = %s %s                 ;; uses = %s\n",
            indention( value ).c_str(), value.label().c_str(),
            value.name().c_str(), tmp.c_str(), uses.c_str() );
    }
}

//
// Constant
//

void IRDumpSourcePass::visit( Constant& value ) const
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

#define IR_DUMP_SOURCE_VISIT( CLASS )                                          \
    void IRDumpSourcePass::visit_prolog( CLASS& value, Context& )              \
    {                                                                          \
        visit( value );                                                        \
    }                                                                          \
    void IRDumpSourcePass::visit_epilog( CLASS& value, Context& )              \
    {                                                                          \
    }

IR_DUMP_SOURCE_VISIT( ForkInstruction );
IR_DUMP_SOURCE_VISIT( MergeInstruction );
IR_DUMP_SOURCE_VISIT( LocalInstruction );
IR_DUMP_SOURCE_VISIT( AssertInstruction );
IR_DUMP_SOURCE_VISIT( SelectInstruction );
IR_DUMP_SOURCE_VISIT( SkipInstruction );
IR_DUMP_SOURCE_VISIT( LocationInstruction );
IR_DUMP_SOURCE_VISIT( LookupInstruction );
IR_DUMP_SOURCE_VISIT( UpdateInstruction );
IR_DUMP_SOURCE_VISIT( CallInstruction );
IR_DUMP_SOURCE_VISIT( PrintInstruction );

IR_DUMP_SOURCE_VISIT( AddInstruction );
IR_DUMP_SOURCE_VISIT( SubInstruction );
IR_DUMP_SOURCE_VISIT( MulInstruction );
IR_DUMP_SOURCE_VISIT( ModInstruction );
IR_DUMP_SOURCE_VISIT( DivInstruction );
IR_DUMP_SOURCE_VISIT( AndInstruction );
IR_DUMP_SOURCE_VISIT( XorInstruction );
IR_DUMP_SOURCE_VISIT( OrInstruction );
IR_DUMP_SOURCE_VISIT( EquInstruction );
IR_DUMP_SOURCE_VISIT( NeqInstruction );
IR_DUMP_SOURCE_VISIT( LthInstruction );

IR_DUMP_SOURCE_VISIT( AgentConstant );
IR_DUMP_SOURCE_VISIT( RuleReferenceConstant );
IR_DUMP_SOURCE_VISIT( BooleanConstant );
IR_DUMP_SOURCE_VISIT( IntegerConstant );
IR_DUMP_SOURCE_VISIT( BitConstant );
IR_DUMP_SOURCE_VISIT( StringConstant );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

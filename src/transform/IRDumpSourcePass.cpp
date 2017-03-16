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

        data->specification()->accept( *this );
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

void IRDumpSourcePass::dump( Statement& value ) const
{
    const char* nline = "\n";
    const char* label = &value.label().c_str()[ 1 ];
    const char* scope = value.scope()->label().c_str();

    if( value.scope()->entry().get() == &value )
    {
        label = &value.scope()->label().c_str()[ 1 ];

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
        scope = value.scope()->label().c_str();

        if( not value.scope()->scope() )
        {
            label = "exit";
        }
        else
        {
            label = &value.scope()->scope()->label().c_str()[ 1 ];
        }
    }

    fprintf( stream, "%s%s%s: %s\n", nline, indention( value ).c_str(), label,
        scope );
}

void IRDumpSourcePass::dump( Instruction& value ) const
{
    if( isa< ForkInstruction >( value ) or isa< MergeInstruction >( value ) )
    {
        fprintf( stream,
            "%s%s %s\n",
            indention( value ).c_str(),
            value.name().c_str(),
            value.statement()->scope()->name().c_str() );
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

void IRDumpSourcePass::dump( Constant& value ) const
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

//
// RecursiveVisitor General
//

void IRDumpSourcePass::visit( Specification& value )
{
    RecursiveVisitor::visit( value );
}
void IRDumpSourcePass::visit( Agent& value )
{
    auto result = m_first.find( value.id() );
    if( result == m_first.end() )
    {
        m_first.insert( value.id() );
        fprintf( stream, "\n" );
    }

    fprintf(
        stream, "%s = %s", value.name().c_str(), value.type().name().c_str() );

    u1 first = true;

    for( auto element : value.elements() )
    {
        fprintf( stream, "%s %s", first ? "" : ",", element.c_str() );
        first = false;
    }

    fprintf( stream, "\n" );
}
void IRDumpSourcePass::visit( Function& value )
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
void IRDumpSourcePass::visit( Derived& value )
{
    fprintf( stream,
        "\n"
        "%s %s = \n"
        "[\n",
        value.name().c_str(), value.type().name().c_str() );

    RecursiveVisitor::visit( value );

    fprintf( stream, "]\n" );
}
void IRDumpSourcePass::visit( Rule& value )
{
    fprintf( stream,
        "\n"
        "%s %s = \n"
        "{\n",
        value.name().c_str(), value.type().name().c_str() );

    RecursiveVisitor::visit( value );

    fprintf( stream, "}\n" );
}
void IRDumpSourcePass::visit( Builtin& value )
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

void IRDumpSourcePass::visit( Enumeration& value )
{
    fprintf( stream, ";; %s\n", value.dump().c_str() );
}

void IRDumpSourcePass::visit( TrivialStatement& value )
{
    dump( value );
    RecursiveVisitor::visit( value );
}
void IRDumpSourcePass::visit( BranchStatement& value )
{
    dump( value );
    RecursiveVisitor::visit( value );
}

//
// RecursiveVisitor Instructions
//

void IRDumpSourcePass::visit( SkipInstruction& value )
{
    dump( value );
}

void IRDumpSourcePass::visit( ForkInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( MergeInstruction& value )
{
    dump( value );
}

void IRDumpSourcePass::visit( LookupInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( UpdateInstruction& value )
{
    dump( value );
}

void IRDumpSourcePass::visit( LocalInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( LocationInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( CallInstruction& value )
{
    dump( value );
}

void IRDumpSourcePass::visit( AssertInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( SelectInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( SymbolicInstruction& value )
{
    dump( value );
}

void IRDumpSourcePass::visit( AddInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( SubInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( MulInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( ModInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( DivInstruction& value )
{
    dump( value );
}

void IRDumpSourcePass::visit( AndInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( XorInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( OrInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( NotInstruction& value )
{
    dump( value );
}

void IRDumpSourcePass::visit( EquInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( NeqInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( LthInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( LeqInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( GthInstruction& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( GeqInstruction& value )
{
    dump( value );
}

//
// RecursiveVisitor Constants
//

void IRDumpSourcePass::visit( VoidConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( RuleReferenceConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( BooleanConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( IntegerConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( BitConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( StringConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( FloatingConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( RationalConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( EnumerationConstant& value )
{
    dump( value );
}
void IRDumpSourcePass::visit( AgentConstant& value )
{
    dump( value );
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

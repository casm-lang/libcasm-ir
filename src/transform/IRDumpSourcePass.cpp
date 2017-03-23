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

static const char* name = 0;

u1 IRDumpSourcePass::run( libpass::PassResult& pr )
{
    name = libpass::PassRegistry::passInfo< IRDumpSourcePass >().name();

    libstdhl::Log::info( "%s: starting", name );

    try
    {
        const auto data = pr.result< ConsistencyCheckPass >();

        IRDumpSourceVisitor visitor{ std::cout };

        data->specification()->accept( visitor );
    }
    catch( ... )
    {
        libstdhl::Log::error( "unsuccessful dump of specification" );
        return false;
    }

    libstdhl::Log::info( "%s: stopping", name );

    return true;
}

IRDumpSourceVisitor::IRDumpSourceVisitor( std::ostream& stream )
: m_stream( stream )
{
}

std::string IRDumpSourceVisitor::indention( Value& value ) const
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

void IRDumpSourceVisitor::dump( Statement& value ) const
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

    m_stream << nline << indention( value ) << label << ": " << scope << "\n";
}

void IRDumpSourceVisitor::dump( Instruction& value ) const
{
    if( isa< ForkInstruction >( value ) or isa< MergeInstruction >( value ) )
    {
        m_stream << indention( value ) << value.name() << " "
                 << value.statement()->scope()->name() << "\n";
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

        m_stream << indention( value ) << value.label() << " = " << value.name()
                 << " " << tmp << "                 ;; uses = " << uses << "\n";
    }
}

void IRDumpSourceVisitor::dump( Constant& value ) const
{
    static u1 first = true;

    if( first )
    {
        first = false;

        m_stream << "\n";
    }

    m_stream << value.label() << " = " << value.type().name() << " "
             << value.name() << "\n";
}

//
// General
//

void IRDumpSourceVisitor::visit( Specification& value )
{
    m_stream << ";; " << value.name() << "\n";

    RecursiveVisitor::visit( value );

    m_stream << "\n";
}
void IRDumpSourceVisitor::visit( Agent& value )
{
    auto result = m_first.find( value.id() );
    if( result == m_first.end() )
    {
        m_first.insert( value.id() );
        m_stream << "\n";
    }

    m_stream << value.name() << " = " << value.type().name();

    u1 first = true;

    for( auto element : value.elements() )
    {
        m_stream << ( first ? "" : "," ) << " " << element;
        first = false;
    }

    m_stream << "\n";
}
void IRDumpSourceVisitor::visit( Function& value )
{
    auto result = m_first.find( value.id() );
    if( result == m_first.end() )
    {
        m_first.insert( value.id() );
        m_stream << "\n";
    }

    m_stream << value.name() << " = " << value.type().name() << "\n";
}
void IRDumpSourceVisitor::visit( Derived& value )
{
    m_stream << "\n"
             << value.name() << " " << value.type().name() << " =\n"
             << "[\n";

    RecursiveVisitor::visit( value );

    m_stream << "]\n";
}
void IRDumpSourceVisitor::visit( Rule& value )
{
    m_stream << "\n"
             << value.name() << " " << value.type().name() << " =\n"
             << "{\n";

    RecursiveVisitor::visit( value );

    m_stream << "}\n";
}
void IRDumpSourceVisitor::visit( Builtin& value )
{
    auto result = m_first.find( value.id() );
    if( result == m_first.end() )
    {
        m_first.insert( value.id() );
        m_stream << "\n";
    }

    m_stream << value.label() << " = " << value.type().name() << " "
             << value.name() << "\n";
}

void IRDumpSourceVisitor::visit( Enumeration& value )
{
    m_stream << ";; " << value.dump() << "\n";
}

void IRDumpSourceVisitor::visit( TrivialStatement& value )
{
    dump( value );
    RecursiveVisitor::visit( value );
}
void IRDumpSourceVisitor::visit( BranchStatement& value )
{
    dump( value );
    RecursiveVisitor::visit( value );
}

//
// Instructions
//

void IRDumpSourceVisitor::visit( SkipInstruction& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( ForkInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( MergeInstruction& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( LookupInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( UpdateInstruction& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( LocalInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( LocationInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( CallInstruction& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( AssertInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( SelectInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( SymbolicInstruction& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( InvInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( AddInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( SubInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( MulInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( ModInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( DivInstruction& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( AndInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( XorInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( OrInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( NotInstruction& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( EquInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( NeqInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( LthInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( LeqInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( GthInstruction& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( GeqInstruction& value )
{
    dump( value );
}

//
// Constants
//

void IRDumpSourceVisitor::visit( VoidConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( RuleReferenceConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( BooleanConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( IntegerConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( BitConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( StringConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( FloatingConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( RationalConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( EnumerationConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( AgentConstant& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( Identifier& value )
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

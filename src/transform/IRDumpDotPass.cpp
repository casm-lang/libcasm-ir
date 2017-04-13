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

#include "IRDumpDotPass.h"

using namespace libcasm_ir;

char IRDumpDotPass::id = 0;

static libpass::PassRegistration< IRDumpDotPass > PASS( "IRDumpDotPass",
    "translates the CASM IR to the ASCII source code representation",
    "ir-dump-dot", 0 );

void IRDumpDotPass::usage( libpass::PassUsage& pu )
{
    pu.require< ConsistencyCheckPass >();
}

u1 IRDumpDotPass::run( libpass::PassResult& pr )
{
    libpass::PassLogger log(
        libpass::PassRegistry::passInfo< IRDumpDotPass >(), stream() );

    log.debug( "starting" );

    try
    {
        const auto data = pr.result< ConsistencyCheckPass >();

        std::ofstream dotfile( "./obj/out.ir.dot" );

        IRDumpDotVisitor visitor{ dotfile };

        data->specification()->accept( visitor );
    }
    catch( ... )
    {
        log.error( "unsuccessful dump of specification" );
        return false;
    }

    log.debug( "stopping" );

    libstdhl::Log::StringFormatter f;
    libstdhl::Log::OutputStreamSink c( std::cerr, f );
    stream().flush( c );

    return true;
}

static inline std::string indention( Value& value );

IRDumpDotVisitor::IRDumpDotVisitor( std::ostream& stream )
: m_stream( stream )
{
}

//
// General
//

void IRDumpDotVisitor::visit( Specification& value )
{
    m_stream << "digraph \"" << value.name() << "\"\n"
             << "{\n"
                "  graph [\n"
                "    rankdir = \"TB\"\n"
                // "    compound=true\n"
                "  ];\n"
                // "  \n"
                // "  node [\n"
                // "    fontsize = \"16\"\n"
                // "    shape = \"ellipse\"\n"
                // "  ];\n"
                // "  \n"
                // "  edge [\n"
                // "  ];\n"
                "  \n";

    m_stream << "  Agents -> \"" << value.agent().get() << "\";\n";

    for( auto v : value.constants() )
    {
        m_stream << "  Constants -> \"" << v.get() << "\";\n";
    }

    for( auto v : value.builtins() )
    {
        m_stream << "  Builtins -> \"" << v.get() << "\";\n";
    }

    for( auto v : value.functions() )
    {
        m_stream << "  Functions -> \"" << v.get() << "\";\n";
    }

    for( auto v : value.deriveds() )
    {
        m_stream << "  Deriveds -> \"" << v.get() << "\";\n";
    }

    for( auto v : value.rules() )
    {
        m_stream << "  Rules -> \"" << v.get() << "_B\";\n";
    }

    RecursiveVisitor::visit( value );

    m_stream << "}\n";
}
void IRDumpDotVisitor::visit( Agent& value )
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  \"" << &value << "\""
             << "  [shape=plaintext, label=<\n"
             << "<TABLE"
             << " BORDER=\"0\""
             << " CELLBORDER=\"1\""
             << " CELLSPACING=\"0\""
             << " CELLPADDING=\"3\""
             << ">\n";

    m_stream << "<TR>"
             << "<TD PORT=\"" << value.name() << "\">"
             << "<B>" << value.name() << "</B>"
             << "</TD>"
             << "</TR>\n";

    // TODO: PPA: dump the current agent domain type and there values!

    m_stream << "</TABLE>>];\n";
}
void IRDumpDotVisitor::visit( Function& value )
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  \"" << &value << "\" [label=\"" << value.dump() << "\"];\n";
}
void IRDumpDotVisitor::visit( Derived& value )
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  \"" << &value << "\" [label=\"" << value.dump() << "\"];\n";

    RecursiveVisitor::visit( value );
}
void IRDumpDotVisitor::visit( Rule& value )
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  subgraph \"" << &value << "\" { \n"
             << "    label=\"" << value.name() << "\"\n"
             << "    color=blue;\n";

    // begin (B) and end (E) connection points of the sub-graph

    m_stream << "  \"" << &value << "_B\" [label=\"B: " << value.dump()
             << "\"]\n"; // TODO: , style=invis

    m_stream << "  \"" << &value << "_E\"   [label=\"E: " << value.dump()
             << "\"]\n";

    m_stream << "  \"" << &value << "_B\" -> \"" << value.context().get()
             << "_B\"\n";

    RecursiveVisitor::visit( value );

    m_stream << "  \"" << value.context().get() << "_E\" -> \"" << &value
             << "_E\"\n";

    m_stream << "  }\n";
}
void IRDumpDotVisitor::visit( Builtin& value )
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  \"" << &value << "\" [label=\"" << value.dump() << "\"];\n";
}

void IRDumpDotVisitor::visit( Enumeration& value )
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  \"" << &value << "\" [label=\"" << value.dump() << "\"];\n";
}

void IRDumpDotVisitor::visit( ParallelBlock& value )
{
    dump( value );

    RecursiveVisitor::visit( value );

    m_stream << "  }\n";
}
void IRDumpDotVisitor::visit( SequentialBlock& value )
{
    dump( value );

    RecursiveVisitor::visit( value );

    m_stream << "  }\n";
}

void IRDumpDotVisitor::visit( TrivialStatement& value )
{
    dump( value );

    RecursiveVisitor::visit( value );

    m_stream << "  \"" << value.instructions().back().get() << "\" -> \""
             << &value << "_E\"\n";

    m_stream << "  }\n";
}
void IRDumpDotVisitor::visit( BranchStatement& value )
{
    dump( value );

    RecursiveVisitor::visit( value );

    m_stream << "  }\n";
}

//
// Instructions
//

void IRDumpDotVisitor::visit( SkipInstruction& value )
{
    dump( value );
}

void IRDumpDotVisitor::visit( ForkInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( MergeInstruction& value )
{
    dump( value );
}

void IRDumpDotVisitor::visit( LookupInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( UpdateInstruction& value )
{
    dump( value );
}

void IRDumpDotVisitor::visit( LocalInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( LocationInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( CallInstruction& value )
{
    dump( value );
}

void IRDumpDotVisitor::visit( AssertInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( SelectInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( SymbolicInstruction& value )
{
    dump( value );
}

void IRDumpDotVisitor::visit( InvInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( AddInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( SubInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( MulInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( ModInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( DivInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( PowInstruction& value )
{
    dump( value );
}

void IRDumpDotVisitor::visit( AndInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( XorInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( OrInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( ImpInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( NotInstruction& value )
{
    dump( value );
}

void IRDumpDotVisitor::visit( EquInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( NeqInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( LthInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( LeqInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( GthInstruction& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( GeqInstruction& value )
{
    dump( value );
}

//
// Constants
//

void IRDumpDotVisitor::visit( VoidConstant& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( RuleReferenceConstant& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( BooleanConstant& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( IntegerConstant& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( BitConstant& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( StringConstant& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( FloatingConstant& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( RationalConstant& value )
{
    dump( value );
}
void IRDumpDotVisitor::visit( EnumerationConstant& value )
{
    dump( value );
}

void IRDumpDotVisitor::visit( Identifier& value )
{
    dump( value );
}

void IRDumpDotVisitor::dump( ExecutionSemanticsBlock& value ) const
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  subgraph \"" << &value << "\" {\n"
             << "    label=\"" << value.dump() << "\"\n";

    // begin (B) and end (E) connection points of the sub-graph

    m_stream << "  \"" << &value << "_B\" [label=\"B: " << value.dump()
             << "\"]\n"; // TODO: , style=invis

    m_stream << "  \"" << &value << "_E\"   [label=\"E: " << value.dump()
             << "\"]\n";

    m_stream << "  \"" << &value << "_B\" -> \"" << &value
             << "_E\" [style=dashed, color=gray];\n";

    // up references

    if( value.scope() )
    {
        m_stream << "  \"" << &value << "_B\" -> \"" << value.scope().get()
                 << "_B\" [style=dashed, color=green];\n";
    }

    if( value.parent() )
    {
        m_stream << "  \"" << &value << "_B\" -> \"" << value.parent().get()
                 << "_B\" [style=dashed, color=blue];\n";
    }

    // inner connection

    if( value.entry() )
    {
        m_stream << "  \"" << &value << "_B\" -> \"" << value.entry().get()
                 << "_B\"\n";
    }

    const Block* connection_point = &value;

    if( value.entry() )
    {
        connection_point = value.entry().get();
    }

    for( auto block : value.blocks() )
    {
        m_stream << "  \"" << connection_point << "_E\" -> \"" << block.get()
                 << "_B\"\n";

        if( value.parallel() )
        {
            if( value.exit() )
            {
                m_stream << "  \"" << block.get() << "_E\" -> \""
                         << value.exit().get() << "_B\"\n";
            }
        }
        else
        {
            connection_point = block.get();
        }
    }

    if( value.exit() )
    {
        if( not value.parallel() )
        {
            m_stream << "  \"" << value.blocks().back().get() << "_E\" -> \""
                     << value.exit().get() << "_B\"\n";
        }

        m_stream << "  \"" << value.exit().get() << "_E\" -> \"" << &value
                 << "_E\"\n";
    }
}

void IRDumpDotVisitor::dump( Statement& value ) const
{
    const char* label = &value.label().c_str()[ 1 ];
    const char* scope = value.scope()->label().c_str();

    if( value.scope()->entry().get() == &value )
    {
        label = &value.scope()->label().c_str()[ 1 ];

        if( not value.scope()->scope() )
        {
            scope = "entry";
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

    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  subgraph \"" << &value << "\" {\n"
             << "    label=\"" << label << ": " << scope << "\"\n";

    // begin (B) and end (E) connection points of the sub-graph

    m_stream << "  \"" << &value << "_B\" [label=\"B: " << value.dump()
             << "\"]\n"; // TODO: , style=invis

    m_stream << "  \"" << &value << "_E\"   [label=\"E: " << value.dump()
             << "\"]\n";

    if( value.scope() )
    {
        m_stream << "  \"" << &value << "_B\" -> \"" << value.scope().get()
                 << "_B\" [style=dashed, color=green];\n";
    }

    if( value.parent() )
    {
        m_stream << "  \"" << &value << "_B\" -> \"" << value.parent().get()
                 << "_B\" [style=dashed, color=blue];\n";
    }

    m_stream << "  \"" << &value << "_B\" -> \""
             << value.instructions().front().get() << "\"\n";

    for( auto instr : value.instructions() )
    {
        const auto next = instr->next();
        if( next )
        {
            m_stream << "  \"" << instr.get() << "\" ->  \"" << next.get()
                     << "\";\n";
        }
    }
}

void IRDumpDotVisitor::dump( Instruction& value ) const
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  \"" << &value << "\" [shape=box, color=red, label=\""
             << value.dump() << "\"];\n";

    if( isa< ForkInstruction >( value ) or isa< MergeInstruction >( value ) )
    {
        m_stream << "  #" << indention( value ) << value.name() << " "
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

        m_stream << "  #" << indention( value ) << value.label() << " = "
                 << value.name() << " " << tmp
                 << "                 ;; uses = " << uses << "\n";
    }
}

void IRDumpDotVisitor::dump( Constant& value ) const
{
    m_stream << "  # " << value.dump() << "\n";

    m_stream << "  \"" << &value << "\" [label=\"" << value.dump() << "\"];\n";
}

static inline std::string indention( Value& value )
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
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

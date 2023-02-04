//
//  Copyright (C) 2015-2023 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcasm-ir/graphs/contributors>
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

#include "IRDumpSourcePass.h"

#include <libcasm-ir/Specification>
#include <libcasm-ir/analyze/ConsistencyCheckPass>

#include <libpass/PassLogger>
#include <libpass/PassRegistry>
#include <libpass/PassResult>
#include <libpass/PassUsage>

using namespace libcasm_ir;

char IRDumpSourcePass::id = 0;

static libpass::PassRegistration< IRDumpSourcePass > PASS(
    "IRDumpSourcePass",
    "translates the CASM IR to the ASCII source code representation",
    "ir-dump",
    0 );

void IRDumpSourcePass::usage( libpass::PassUsage& pu )
{
    pu.require< ConsistencyCheckPass >();
}

u1 IRDumpSourcePass::run( libpass::PassResult& pr )
{
    libpass::PassLogger log( &id, stream() );

    const auto& data = pr.input< ConsistencyCheckPass >();
    const auto& specification = data->specification();

    IRDumpSourceVisitor visitor{ std::cout };
    data->specification()->accept( visitor );

    return true;
}

static inline std::string indention( Value& value );

IRDumpSourceVisitor::IRDumpSourceVisitor( std::ostream& stream )
: m_stream( stream )
{
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

    m_stream << value.label() << " = " << value.type().name() << " " << value.name() << "\n";
}

void IRDumpSourceVisitor::visit( Enumeration& value )
{
    m_stream << ";; " << value.dump() << "\n";
}

void IRDumpSourceVisitor::visit( Range& value )
{
    m_stream << ";; " << value.dump() << "\n";
}

void IRDumpSourceVisitor::visit( List& value )
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

void IRDumpSourceVisitor::visit( SelectInstruction& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( SelfInstruction& value )
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
void IRDumpSourceVisitor::visit( PowInstruction& value )
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
void IRDumpSourceVisitor::visit( ImpInstruction& value )
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
void IRDumpSourceVisitor::visit( FunctionReferenceConstant& value )
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
void IRDumpSourceVisitor::visit( BinaryConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( StringConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( DecimalConstant& value )
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
void IRDumpSourceVisitor::visit( RangeConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( TupleConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( ListConstant& value )
{
    dump( value );
}
void IRDumpSourceVisitor::visit( DomainConstant& value )
{
    dump( value );
}

void IRDumpSourceVisitor::visit( Identifier& value )
{
    dump( value );
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
        m_stream << indention( value ) << value.name() << " " << value.statement()->scope()->name()
                 << "\n";
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
            uses += ", ";
        }
        uses += "}";

        m_stream << indention( value );
        if( not value.type().isVoid() )
        {
            m_stream << value.label() << " = ";
        }

        m_stream << value.name() << " " << tmp << "    ;; uses = " << uses << "\n";
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

    m_stream << value.label() << " = " << value.type().name() << " " << value.name() << "\n";
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

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

#include "ConsistencyCheckPass.h"

using namespace libcasm_ir;

char ConsistencyCheckPass::id = 0;

static libpass::PassRegistration< ConsistencyCheckPass > PASS(
    "CASM IR Consistency Check",
    "checks the constructed IR in-memory representation", "ir-check", 0 );

static const char* name = 0;

u1 ConsistencyCheckPass::run( libpass::PassResult& pr )
{
    name = libpass::PassRegistry::passInfo< ConsistencyCheckPass >().name();

    libstdhl::Log::info( "%s: starting", name );

    auto data = pr.result< ConsistencyCheckPass >();
    assert( data );

    try
    {
#ifndef NDEBUG
        // check 'is-a' relation and 'type of value' only in 'debug' builds
        data->specification()->accept( *this );
#endif
        data->specification()->iterate( Traversal::PREORDER, [this]( Value&
                                                                     value ) {
            if( value.label().empty() )
            {
                libstdhl::Log::error( "value '%p' %s: has no label", &value,
                    value.dump().c_str() );
            }

            if( const auto v = cast< Specification >( value ) )
            {
                if( not( v->rules().size() > 0 ) )
                {
                    libstdhl::Log::error( "specification '%p' %s: has no rules",
                        v, v->dump().c_str() );
                }
                if( not v->agent() )
                {
                    libstdhl::Log::error( "specification '%p' %s: has no agent",
                        v, v->dump().c_str() );
                }
            }
            else if( const auto v = cast< Rule >( value ) )
            {
                if( not v->context() )
                {
                    libstdhl::Log::error(
                        "rule '%p' %s: has no context", v, v->dump().c_str() );
                }

                if( const auto p = cast< ParallelBlock >( v->context() ) )
                {
                    if( *p->rule() != value )
                    {
                        libstdhl::Log::error(
                            "rule '%p' %s: context does not point to this rule",
                            v,
                            v->dump().c_str() );
                    }
                }
                else
                {
                    libstdhl::Log::error(
                        "rule '%p' %s: does not start with a parallel block",
                        v,
                        v->dump().c_str() );
                }
            }
            else if( const auto v = cast< ExecutionSemanticsBlock >( value ) )
            {
                u1 block_is_context_of_rule = false;

                if( const auto p = cast< ParallelBlock >( value ) )
                {
                    if( p->rule() )
                    {
                        block_is_context_of_rule = true;
                    }
                }

                if( not v->parent() and not block_is_context_of_rule )
                {
                    libstdhl::Log::error(
                        "eblk '%p' %s: has no parent", v, v->dump().c_str() );
                }

                if( not v->scope() and not block_is_context_of_rule )
                {
                    libstdhl::Log::error(
                        "eblk '%p' %s: has no scope", v, v->dump().c_str() );
                }

                if( v->blocks().size() < 1 )
                {
                    libstdhl::Log::error(
                        "eblk '%p' %s: shall contain at least 1 block", v,
                        v->dump().c_str() );
                }

                if( ( not v->entry() ) and ( not v->exit() )
                    and ( v->blocks().size() != 1 ) )
                {
                    libstdhl::Log::error(
                        "eblk '%p' %s: if empty entry and exit section, inner "
                        "blocks size shall be 1",
                        v, v->dump().c_str() );
                }

                if( ( ( not v->entry() ) and ( v->exit() ) )
                    or ( ( v->entry() ) and ( not v->exit() ) ) )
                {
                    libstdhl::Log::error(
                        "eblk '%p' %s: empty entry or empty exit found", v,
                        v->dump().c_str() );
                }
            }
            else if( const auto v = cast< Statement >( value ) )
            {
                if( not v->parent() )
                {
                    libstdhl::Log::error(
                        "stmt '%p' %s: has no parent", v, v->dump().c_str() );
                }

                if( not v->scope() )
                {
                    libstdhl::Log::error(
                        "stmt '%p' %s: has no scope", v, v->dump().c_str() );
                }

                if( v->instructions().size() < 1 )
                {
                    libstdhl::Log::error(
                        "stmt '%p' %s: shall contain at least 1 instruction", v,
                        v->dump().c_str() );
                }
            }
            else if( const auto v = cast< Instruction >( value ) )
            {
                if( not v->statement() )
                {
                    libstdhl::Log::error( "inst '%p' %s: has no statement",
                        v,
                        v->dump().c_str() );
                }
            }
        } );
    }
    catch( ... )
    {
        libstdhl::Log::error( "unsuccessful dump of specification" );
        return false;
    }

    pr.setResult< ConsistencyCheckPass >( data );

    libstdhl::Log::info( "%s: stopping", name );

    return true;
}

template < typename T >
void ConsistencyCheckPass::verify( Value& value )
{
    if( not isa< T >( value ) )
    {
        libstdhl::Log::error(
            "inconsistent class value '%u' found", value.id() );
    }
    if( not value.ptr_type() )
    {
        libstdhl::Log::error( "value with no type found" );
    }
}

//
// RecursiveVisitor General
//

void ConsistencyCheckPass::visit( Specification& value )
{
    verify< Specification >( value );

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckPass::visit( Agent& value )
{
    verify< Agent >( value );
}
void ConsistencyCheckPass::visit( Function& value )
{
    verify< Function >( value );
}
void ConsistencyCheckPass::visit( Derived& value )
{
    verify< Derived >( value );

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckPass::visit( Rule& value )
{
    verify< Rule >( value );

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckPass::visit( Builtin& value )
{
    verify< Builtin >( value );
}

void ConsistencyCheckPass::visit( Enumeration& value )
{
    verify< Enumeration >( value );
}

void ConsistencyCheckPass::visit( ParallelBlock& value )
{
    verify< ParallelBlock >( value );

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckPass::visit( SequentialBlock& value )
{
    verify< SequentialBlock >( value );

    RecursiveVisitor::visit( value );
}

void ConsistencyCheckPass::visit( TrivialStatement& value )
{
    verify< TrivialStatement >( value );

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckPass::visit( BranchStatement& value )
{
    verify< BranchStatement >( value );

    RecursiveVisitor::visit( value );
}

//
// RecursiveVisitor Instructions
//

void ConsistencyCheckPass::visit( SkipInstruction& value )
{
    verify< SkipInstruction >( value );
}

void ConsistencyCheckPass::visit( ForkInstruction& value )
{
    verify< ForkInstruction >( value );
}
void ConsistencyCheckPass::visit( MergeInstruction& value )
{
    verify< MergeInstruction >( value );
}

void ConsistencyCheckPass::visit( LookupInstruction& value )
{
    verify< LookupInstruction >( value );
}
void ConsistencyCheckPass::visit( UpdateInstruction& value )
{
    verify< UpdateInstruction >( value );
}

void ConsistencyCheckPass::visit( LocalInstruction& value )
{
    verify< LocalInstruction >( value );
}
void ConsistencyCheckPass::visit( LocationInstruction& value )
{
    verify< LocationInstruction >( value );
}
void ConsistencyCheckPass::visit( CallInstruction& value )
{
    verify< CallInstruction >( value );
}

void ConsistencyCheckPass::visit( AssertInstruction& value )
{
    verify< AssertInstruction >( value );
}
void ConsistencyCheckPass::visit( SelectInstruction& value )
{
    verify< SelectInstruction >( value );
}
void ConsistencyCheckPass::visit( SymbolicInstruction& value )
{
    verify< SymbolicInstruction >( value );
}

void ConsistencyCheckPass::visit( AddInstruction& value )
{
    verify< AddInstruction >( value );
}
void ConsistencyCheckPass::visit( SubInstruction& value )
{
    verify< SubInstruction >( value );
}
void ConsistencyCheckPass::visit( MulInstruction& value )
{
    verify< MulInstruction >( value );
}
void ConsistencyCheckPass::visit( ModInstruction& value )
{
    verify< ModInstruction >( value );
}
void ConsistencyCheckPass::visit( DivInstruction& value )
{
    verify< DivInstruction >( value );
}

void ConsistencyCheckPass::visit( AndInstruction& value )
{
    verify< AndInstruction >( value );
}
void ConsistencyCheckPass::visit( XorInstruction& value )
{
    verify< XorInstruction >( value );
}
void ConsistencyCheckPass::visit( OrInstruction& value )
{
    verify< OrInstruction >( value );
}
void ConsistencyCheckPass::visit( NotInstruction& value )
{
    verify< NotInstruction >( value );
}

void ConsistencyCheckPass::visit( EquInstruction& value )
{
    verify< EquInstruction >( value );
}
void ConsistencyCheckPass::visit( NeqInstruction& value )
{
    verify< NeqInstruction >( value );
}
void ConsistencyCheckPass::visit( LthInstruction& value )
{
    verify< LthInstruction >( value );
}
void ConsistencyCheckPass::visit( LeqInstruction& value )
{
    verify< LeqInstruction >( value );
}
void ConsistencyCheckPass::visit( GthInstruction& value )
{
    verify< GthInstruction >( value );
}
void ConsistencyCheckPass::visit( GeqInstruction& value )
{
    verify< GeqInstruction >( value );
}

//
// RecursiveVisitor Constants
//

void ConsistencyCheckPass::visit( VoidConstant& value )
{
    verify< VoidConstant >( value );
}
void ConsistencyCheckPass::visit( RuleReferenceConstant& value )
{
    verify< RuleReferenceConstant >( value );
}
void ConsistencyCheckPass::visit( BooleanConstant& value )
{
    verify< BooleanConstant >( value );
}
void ConsistencyCheckPass::visit( IntegerConstant& value )
{
    verify< IntegerConstant >( value );
}
void ConsistencyCheckPass::visit( BitConstant& value )
{
    verify< BitConstant >( value );
}
void ConsistencyCheckPass::visit( StringConstant& value )
{
    verify< StringConstant >( value );
}
void ConsistencyCheckPass::visit( FloatingConstant& value )
{
    verify< FloatingConstant >( value );
}
void ConsistencyCheckPass::visit( RationalConstant& value )
{
    verify< RationalConstant >( value );
}
void ConsistencyCheckPass::visit( EnumerationConstant& value )
{
    verify< EnumerationConstant >( value );
}
void ConsistencyCheckPass::visit( AgentConstant& value )
{
    verify< AgentConstant >( value );
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

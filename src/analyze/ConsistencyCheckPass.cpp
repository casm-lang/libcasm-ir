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

#include "libcasm-ir.h"

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
        data->specification()->iterate( Traversal::PREORDER, this );
#endif

        data->specification()->iterate(
            Traversal::PREORDER, [this]( Value& value, Context& ) {
                if( const auto v = isa< Statement >( value ) )
                {
                }
                else if( const auto v = cast< Instruction >( value ) )
                {
                }
            } );
    }
    catch( ... )
    {
        libstdhl::Log::error( "unsuccessful dump of specification" );
        return false;
    }

    return true;
}

#define CONSISTENCY_CHECK_CASE2( CLASS )                                       \
    void ConsistencyCheckPass::visit_prolog( CLASS& value, Context& )          \
    {                                                                          \
        libstdhl::Log::info( "%s: @%s", name, #CLASS );                        \
                                                                               \
        if( not isa< CLASS >( value ) )                                        \
        {                                                                      \
            libstdhl::Log::error( #CLASS );                                    \
        }                                                                      \
    }                                                                          \
    void ConsistencyCheckPass::visit_epilog( CLASS& value, Context& )          \
    {                                                                          \
        if( not cast< CLASS >( value ) )                                       \
        {                                                                      \
            libstdhl::Log::error( #CLASS );                                    \
        }                                                                      \
    }

#define CONSISTENCY_CHECK_CASE3( CLASS )                                       \
    CONSISTENCY_CHECK_CASE2( CLASS );                                          \
    void ConsistencyCheckPass::visit_interlog( CLASS& value, Context& )        \
    {                                                                          \
        if( not isa< CLASS >( value ) )                                        \
        {                                                                      \
            libstdhl::Log::error( #CLASS );                                    \
        }                                                                      \
    }

CONSISTENCY_CHECK_CASE2( Specification );

CONSISTENCY_CHECK_CASE2( Agent );
CONSISTENCY_CHECK_CASE2( Builtin );
CONSISTENCY_CHECK_CASE2( Function );
CONSISTENCY_CHECK_CASE3( Derived );
CONSISTENCY_CHECK_CASE3( Rule );

CONSISTENCY_CHECK_CASE2( ParallelBlock );
CONSISTENCY_CHECK_CASE2( SequentialBlock );

CONSISTENCY_CHECK_CASE2( TrivialStatement );
CONSISTENCY_CHECK_CASE3( BranchStatement );

CONSISTENCY_CHECK_CASE2( ForkInstruction );
CONSISTENCY_CHECK_CASE2( MergeInstruction );
CONSISTENCY_CHECK_CASE2( LocalInstruction );
CONSISTENCY_CHECK_CASE2( AssertInstruction );
CONSISTENCY_CHECK_CASE2( SelectInstruction );
CONSISTENCY_CHECK_CASE2( SkipInstruction );
CONSISTENCY_CHECK_CASE2( LocationInstruction );
CONSISTENCY_CHECK_CASE2( LookupInstruction );
CONSISTENCY_CHECK_CASE2( UpdateInstruction );
CONSISTENCY_CHECK_CASE2( CallInstruction );
CONSISTENCY_CHECK_CASE2( PrintInstruction );

CONSISTENCY_CHECK_CASE2( AddInstruction );
CONSISTENCY_CHECK_CASE2( SubInstruction );
CONSISTENCY_CHECK_CASE2( MulInstruction );
CONSISTENCY_CHECK_CASE2( ModInstruction );
CONSISTENCY_CHECK_CASE2( DivInstruction );
CONSISTENCY_CHECK_CASE2( AndInstruction );
CONSISTENCY_CHECK_CASE2( XorInstruction );
CONSISTENCY_CHECK_CASE2( OrInstruction );
CONSISTENCY_CHECK_CASE2( EquInstruction );
CONSISTENCY_CHECK_CASE2( NeqInstruction );
CONSISTENCY_CHECK_CASE2( LthInstruction );

CONSISTENCY_CHECK_CASE2( AgentConstant );
CONSISTENCY_CHECK_CASE2( RuleReferenceConstant );
CONSISTENCY_CHECK_CASE2( BooleanConstant );
CONSISTENCY_CHECK_CASE2( IntegerConstant );
CONSISTENCY_CHECK_CASE2( BitConstant );
CONSISTENCY_CHECK_CASE2( StringConstant );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

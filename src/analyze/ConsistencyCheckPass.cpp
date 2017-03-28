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

u1 ConsistencyCheckPass::run( libpass::PassResult& pr )
{
    libpass::PassLogger log( &id, stream() );

    log.debug( "starting" );

    auto data = pr.result< ConsistencyCheckPass >();
    assert( data );

    try
    {
        ConsistencyCheckVisitor visitor;
        data->specification()->accept( visitor );
    }
    catch( ... )
    {
        log.error( "inconsistent specification" );
        return false;
    }

    pr.setResult< ConsistencyCheckPass >( data );

    log.debug( "stopping" );

    return true;
}

template < typename T >
void ConsistencyCheckVisitor::verify( Value& value )
{
    if( not value.ptr_type() )
    {
        libstdhl::Log::error( "value with no type found" );
    }

    if( not isa< T >( value ) )
    {
        libstdhl::Log::error(
            "inconsistent class value '%u' found", value.id() );

        // PPA: if the 'is-a' relation does not work, we cannot rely on this
        //      function to check the following IR properties, so we have to
        //      abort and return this function!
        return;
    }

    if( value.label().empty() )
    {
        libstdhl::Log::error(
            "value '%p' %s: has no label", &value, value.dump().c_str() );
    }

    if( const auto v = cast< Instruction >( value ) )
    {
        if( not v->statement() )
        {
            libstdhl::Log::error(
                "inst '%p' %s: has no statement", v, v->dump().c_str() );
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
}

//
// General
//

void ConsistencyCheckVisitor::visit( Specification& value )
{
    verify< Specification >( value );

    if( not( value.rules().size() > 0 ) )
    {
        libstdhl::Log::error( "specification '%p' %s: has no rules", &value,
            value.dump().c_str() );
    }

    if( not value.agent() )
    {
        libstdhl::Log::error( "specification '%p' %s: has no agent", &value,
            value.dump().c_str() );
    }

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckVisitor::visit( Agent& value )
{
    verify< Agent >( value );
}
void ConsistencyCheckVisitor::visit( Function& value )
{
    verify< Function >( value );
}
void ConsistencyCheckVisitor::visit( Derived& value )
{
    verify< Derived >( value );

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckVisitor::visit( Rule& value )
{
    verify< Rule >( value );

    if( not value.context() )
    {
        libstdhl::Log::error(
            "rule '%p' %s: has no context", &value, value.dump().c_str() );
    }

    if( const auto p = cast< ParallelBlock >( value.context() ) )
    {
        if( *p->rule() != value )
        {
            libstdhl::Log::error(
                "rule '%p' %s: context does not point to this rule",
                &value,
                value.dump().c_str() );
        }
    }
    else
    {
        libstdhl::Log::error(
            "rule '%p' %s: does not start with a parallel block",
            &value,
            value.dump().c_str() );
    }

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckVisitor::visit( Builtin& value )
{
    verify< Builtin >( value );
}

void ConsistencyCheckVisitor::visit( Enumeration& value )
{
    verify< Enumeration >( value );
}

void ConsistencyCheckVisitor::visit( ParallelBlock& value )
{
    verify< ParallelBlock >( value );

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckVisitor::visit( SequentialBlock& value )
{
    verify< SequentialBlock >( value );

    RecursiveVisitor::visit( value );
}

void ConsistencyCheckVisitor::visit( TrivialStatement& value )
{
    verify< TrivialStatement >( value );

    RecursiveVisitor::visit( value );
}
void ConsistencyCheckVisitor::visit( BranchStatement& value )
{
    verify< BranchStatement >( value );

    u1 found = false;
    for( auto instr : value.instructions() )
    {
        if( isa< SelectInstruction >( instr ) )
        {
            if( not found )
            {
                found = true;
            }
            else
            {
                libstdhl::Log::error(
                    "invalid 'BranchStatement' found, only one select "
                    "instruction is allowed, invalid '%s'",
                    instr->dump().c_str() );
            }
        }
    }

    RecursiveVisitor::visit( value );
}

//
// Instructions
//

void ConsistencyCheckVisitor::visit( SkipInstruction& value )
{
    verify< SkipInstruction >( value );
}

void ConsistencyCheckVisitor::visit( ForkInstruction& value )
{
    verify< ForkInstruction >( value );
}
void ConsistencyCheckVisitor::visit( MergeInstruction& value )
{
    verify< MergeInstruction >( value );
}

void ConsistencyCheckVisitor::visit( LookupInstruction& value )
{
    verify< LookupInstruction >( value );
}
void ConsistencyCheckVisitor::visit( UpdateInstruction& value )
{
    verify< UpdateInstruction >( value );
}

void ConsistencyCheckVisitor::visit( LocalInstruction& value )
{
    verify< LocalInstruction >( value );
}
void ConsistencyCheckVisitor::visit( LocationInstruction& value )
{
    verify< LocationInstruction >( value );
}
void ConsistencyCheckVisitor::visit( CallInstruction& value )
{
    verify< CallInstruction >( value );
}

void ConsistencyCheckVisitor::visit( AssertInstruction& value )
{
    verify< AssertInstruction >( value );
}
void ConsistencyCheckVisitor::visit( SelectInstruction& value )
{
    verify< SelectInstruction >( value );

    const auto size = value.operands().size();

    if( size < 3 or ( size % 2 ) != 1 )
    {
        libstdhl::Log::error(
            "select statement has invalid operand size of '%u'", size );
    }

    if( not isa< BranchStatement >( value.statement() ) )
    {
        libstdhl::Log::error(
            "select statement is only allowed to reside in 'BranchStatement' "
            "blocks" );
    }
}
void ConsistencyCheckVisitor::visit( SymbolicInstruction& value )
{
    verify< SymbolicInstruction >( value );
}

void ConsistencyCheckVisitor::visit( InvInstruction& value )
{
    verify< InvInstruction >( value );
}
void ConsistencyCheckVisitor::visit( AddInstruction& value )
{
    verify< AddInstruction >( value );
}
void ConsistencyCheckVisitor::visit( SubInstruction& value )
{
    verify< SubInstruction >( value );
}
void ConsistencyCheckVisitor::visit( MulInstruction& value )
{
    verify< MulInstruction >( value );
}
void ConsistencyCheckVisitor::visit( ModInstruction& value )
{
    verify< ModInstruction >( value );
}
void ConsistencyCheckVisitor::visit( DivInstruction& value )
{
    verify< DivInstruction >( value );
}

void ConsistencyCheckVisitor::visit( AndInstruction& value )
{
    verify< AndInstruction >( value );
}
void ConsistencyCheckVisitor::visit( XorInstruction& value )
{
    verify< XorInstruction >( value );
}
void ConsistencyCheckVisitor::visit( OrInstruction& value )
{
    verify< OrInstruction >( value );
}
void ConsistencyCheckVisitor::visit( NotInstruction& value )
{
    verify< NotInstruction >( value );
}

void ConsistencyCheckVisitor::visit( EquInstruction& value )
{
    verify< EquInstruction >( value );
}
void ConsistencyCheckVisitor::visit( NeqInstruction& value )
{
    verify< NeqInstruction >( value );
}
void ConsistencyCheckVisitor::visit( LthInstruction& value )
{
    verify< LthInstruction >( value );
}
void ConsistencyCheckVisitor::visit( LeqInstruction& value )
{
    verify< LeqInstruction >( value );
}
void ConsistencyCheckVisitor::visit( GthInstruction& value )
{
    verify< GthInstruction >( value );
}
void ConsistencyCheckVisitor::visit( GeqInstruction& value )
{
    verify< GeqInstruction >( value );
}

//
// Constants
//

void ConsistencyCheckVisitor::visit( VoidConstant& value )
{
    verify< VoidConstant >( value );
}
void ConsistencyCheckVisitor::visit( RuleReferenceConstant& value )
{
    verify< RuleReferenceConstant >( value );
}
void ConsistencyCheckVisitor::visit( BooleanConstant& value )
{
    verify< BooleanConstant >( value );
}
void ConsistencyCheckVisitor::visit( IntegerConstant& value )
{
    verify< IntegerConstant >( value );
}
void ConsistencyCheckVisitor::visit( BitConstant& value )
{
    verify< BitConstant >( value );
}
void ConsistencyCheckVisitor::visit( StringConstant& value )
{
    verify< StringConstant >( value );
}
void ConsistencyCheckVisitor::visit( FloatingConstant& value )
{
    verify< FloatingConstant >( value );
}
void ConsistencyCheckVisitor::visit( RationalConstant& value )
{
    verify< RationalConstant >( value );
}
void ConsistencyCheckVisitor::visit( EnumerationConstant& value )
{
    verify< EnumerationConstant >( value );
}
void ConsistencyCheckVisitor::visit( AgentConstant& value )
{
    verify< AgentConstant >( value );
}
void ConsistencyCheckVisitor::visit( Identifier& value )
{
    verify< Identifier >( value );
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

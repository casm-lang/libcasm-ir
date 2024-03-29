//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
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

#include "Visitor.h"

#include "Specification.h"

using namespace libcasm_ir;

//
// RecursiveVisitor
//

void RecursiveVisitor::visit( Specification& value )
{
    value.agent()->accept( *this );
    value.constants().accept( *this );
    value.builtins().accept( *this );
    value.functions().accept( *this );
    value.deriveds().accept( *this );
    value.rules().accept( *this );
}
void RecursiveVisitor::visit( Agent& value )
{
}
void RecursiveVisitor::visit( Function& value )
{
}
void RecursiveVisitor::visit( Derived& value )
{
    value.context()->accept( *this );
}
void RecursiveVisitor::visit( Rule& value )
{
    value.context()->accept( *this );
}
void RecursiveVisitor::visit( Builtin& value )
{
}

void RecursiveVisitor::visit( Enumeration& value )
{
}

void RecursiveVisitor::visit( Range& value )
{
}

void RecursiveVisitor::visit( List& value )
{
}

void RecursiveVisitor::visit( ParallelBlock& value )
{
    value.entry()->accept( *this );
    value.blocks().accept( *this );
    value.exit()->accept( *this );
}
void RecursiveVisitor::visit( SequentialBlock& value )
{
    value.entry()->accept( *this );
    value.blocks().accept( *this );
    value.exit()->accept( *this );
}

void RecursiveVisitor::visit( TrivialStatement& value )
{
    value.instructions().accept( *this );
}
void RecursiveVisitor::visit( BranchStatement& value )
{
    value.instructions().accept( *this );
    value.blocks().accept( *this );
}

//
// TraversalVisitor
//

TraversalVisitor::TraversalVisitor(
    const Traversal order, std::function< void( Value& ) > callback )
: m_order( order )
, m_callback( callback )
{
}

Traversal TraversalVisitor::order( void ) const
{
    return m_order;
}

std::function< void( Value& ) > TraversalVisitor::callback( void ) const
{
    return m_callback;
}

void TraversalVisitor::visit( Specification& value )
{
    if( order() == PREORDER )
    {
        callback()( value );
    }

    value.agent()->accept( *this );
    value.constants().accept( *this );
    value.builtins().accept( *this );
    value.functions().accept( *this );
    value.deriveds().accept( *this );
    value.rules().accept( *this );

    if( order() == POSTORDER )
    {
        callback()( value );
    }
}
void TraversalVisitor::visit( Agent& value )
{
    callback()( value );
}
void TraversalVisitor::visit( Function& value )
{
    callback()( value );
}
void TraversalVisitor::visit( Derived& value )
{
    if( order() == PREORDER )
    {
        callback()( value );
    }

    value.context()->accept( *this );

    if( order() == POSTORDER )
    {
        callback()( value );
    }
}
void TraversalVisitor::visit( Rule& value )
{
    if( order() == PREORDER )
    {
        callback()( value );
    }

    value.context()->accept( *this );

    if( order() == POSTORDER )
    {
        callback()( value );
    }
}
void TraversalVisitor::visit( Builtin& value )
{
    callback()( value );
}

void TraversalVisitor::visit( Enumeration& value )
{
    callback()( value );
}

void TraversalVisitor::visit( Range& value )
{
    callback()( value );
}

void TraversalVisitor::visit( List& value )
{
    callback()( value );
}

void TraversalVisitor::visit( ParallelBlock& value )
{
    if( order() == PREORDER )
    {
        callback()( value );
    }

    value.entry()->accept( *this );
    value.blocks().accept( *this );
    value.exit()->accept( *this );

    if( order() == POSTORDER )
    {
        callback()( value );
    }
}
void TraversalVisitor::visit( SequentialBlock& value )
{
    if( order() == PREORDER )
    {
        callback()( value );
    }

    value.entry()->accept( *this );
    value.blocks().accept( *this );
    value.exit()->accept( *this );

    if( order() == POSTORDER )
    {
        callback()( value );
    }
}

void TraversalVisitor::visit( TrivialStatement& value )
{
    if( order() == PREORDER )
    {
        callback()( value );
    }

    value.instructions().accept( *this );

    if( order() == POSTORDER )
    {
        callback()( value );
    }
}
void TraversalVisitor::visit( BranchStatement& value )
{
    if( order() == PREORDER )
    {
        callback()( value );
    }

    value.instructions().accept( *this );
    value.blocks().accept( *this );

    if( order() == POSTORDER )
    {
        callback()( value );
    }
}

void TraversalVisitor::visit( SkipInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( ForkInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( MergeInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( LookupInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( UpdateInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( LocalInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( LocationInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( CallInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( SelectInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( SelfInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( InvInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( AddInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( SubInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( MulInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( ModInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( DivInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( PowInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( AndInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( XorInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( OrInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( ImpInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( NotInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( EquInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( NeqInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( LthInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( LeqInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( GthInstruction& value )
{
    callback()( value );
}
void TraversalVisitor::visit( GeqInstruction& value )
{
    callback()( value );
}

void TraversalVisitor::visit( VoidConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( RuleReferenceConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( FunctionReferenceConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( BooleanConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( IntegerConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( BinaryConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( StringConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( DecimalConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( RationalConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( EnumerationConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( RangeConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( TupleConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( ListConstant& value )
{
    callback()( value );
}
void TraversalVisitor::visit( DomainConstant& value )
{
    callback()( value );
}

void TraversalVisitor::visit( Identifier& value )
{
    callback()( value );
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

//
//  Copyright (C) 2015-2021 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
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

#ifndef _LIBCASM_IR_VISITOR_H_
#define _LIBCASM_IR_VISITOR_H_

#include <libcasm-ir/CasmIR>

#include <functional>

namespace libcasm_ir
{
    class Specification;

    class Agent;
    class Function;
    class Derived;
    class Rule;
    class Builtin;

    class Enumeration;
    class Range;
    class Tuple;
    class List;
    class Structure;
    class Feature;

    class ParallelBlock;
    class SequentialBlock;

    class TrivialStatement;
    class BranchStatement;

    class SkipInstruction;

    class ForkInstruction;
    class MergeInstruction;

    class LookupInstruction;
    class UpdateInstruction;

    class LocalInstruction;
    class LocationInstruction;

    class SelectInstruction;

    class CallInstruction;

    class InvInstruction;
    class AddInstruction;
    class SubInstruction;
    class MulInstruction;
    class ModInstruction;
    class DivInstruction;
    class PowInstruction;

    class AndInstruction;
    class XorInstruction;
    class OrInstruction;
    class ImpInstruction;
    class NotInstruction;

    class EquInstruction;
    class NeqInstruction;
    class LthInstruction;
    class LeqInstruction;
    class GthInstruction;
    class GeqInstruction;

    class VoidConstant;
    class RuleReferenceConstant;
    class FunctionReferenceConstant;
    class BooleanConstant;
    class IntegerConstant;
    class BinaryConstant;
    class StringConstant;
    class RationalConstant;
    class DecimalConstant;
    class EnumerationConstant;
    class RangeConstant;
    class TupleConstant;
    class ListConstant;
    class DomainConstant;

    class Identifier;

    enum Traversal : u8
    {
        PREORDER = 0,
        POSTORDER
    };

    class Visitor : public CasmIR
    {
      public:
        virtual ~Visitor( void ) = default;

        //
        // General
        //

        virtual void visit( Specification& value ) = 0;
        virtual void visit( Agent& value ) = 0;
        virtual void visit( Function& value ) = 0;
        virtual void visit( Derived& value ) = 0;
        virtual void visit( Rule& value ) = 0;
        virtual void visit( Builtin& value ) = 0;

        virtual void visit( Enumeration& value ) = 0;
        virtual void visit( Range& value ) = 0;
        virtual void visit( Tuple& value ) = 0;
        virtual void visit( List& value ) = 0;
        virtual void visit( Structure& value ) = 0;
        virtual void visit( Feature& value ) = 0;

        virtual void visit( ParallelBlock& value ) = 0;
        virtual void visit( SequentialBlock& value ) = 0;

        virtual void visit( TrivialStatement& value ) = 0;
        virtual void visit( BranchStatement& value ) = 0;

        //
        // Instructions
        //

        virtual void visit( SkipInstruction& value ) = 0;

        virtual void visit( ForkInstruction& value ) = 0;
        virtual void visit( MergeInstruction& value ) = 0;

        virtual void visit( LookupInstruction& value ) = 0;
        virtual void visit( UpdateInstruction& value ) = 0;

        virtual void visit( LocalInstruction& value ) = 0;
        virtual void visit( LocationInstruction& value ) = 0;
        virtual void visit( CallInstruction& value ) = 0;

        virtual void visit( SelectInstruction& value ) = 0;

        virtual void visit( InvInstruction& value ) = 0;
        virtual void visit( AddInstruction& value ) = 0;
        virtual void visit( SubInstruction& value ) = 0;
        virtual void visit( MulInstruction& value ) = 0;
        virtual void visit( ModInstruction& value ) = 0;
        virtual void visit( DivInstruction& value ) = 0;
        virtual void visit( PowInstruction& value ) = 0;

        virtual void visit( AndInstruction& value ) = 0;
        virtual void visit( XorInstruction& value ) = 0;
        virtual void visit( OrInstruction& value ) = 0;
        virtual void visit( ImpInstruction& value ) = 0;
        virtual void visit( NotInstruction& value ) = 0;

        virtual void visit( EquInstruction& value ) = 0;
        virtual void visit( NeqInstruction& value ) = 0;
        virtual void visit( LthInstruction& value ) = 0;
        virtual void visit( LeqInstruction& value ) = 0;
        virtual void visit( GthInstruction& value ) = 0;
        virtual void visit( GeqInstruction& value ) = 0;

        //
        // Constants
        //

        virtual void visit( VoidConstant& value ) = 0;
        virtual void visit( RuleReferenceConstant& value ) = 0;
        virtual void visit( FunctionReferenceConstant& value ) = 0;
        virtual void visit( BooleanConstant& value ) = 0;
        virtual void visit( IntegerConstant& value ) = 0;
        virtual void visit( BinaryConstant& value ) = 0;
        virtual void visit( StringConstant& value ) = 0;
        virtual void visit( DecimalConstant& value ) = 0;
        virtual void visit( RationalConstant& value ) = 0;
        virtual void visit( EnumerationConstant& value ) = 0;
        virtual void visit( RangeConstant& value ) = 0;
        virtual void visit( TupleConstant& value ) = 0;
        virtual void visit( ListConstant& value ) = 0;
        virtual void visit( DomainConstant& value ) = 0;

        virtual void visit( Identifier& value ) = 0;
    };

    class RecursiveVisitor : public Visitor
    {
      public:
        //
        // General
        //

        void visit( Specification& value ) override;
        void visit( Agent& value ) override;
        void visit( Function& value ) override;
        void visit( Derived& value ) override;
        void visit( Rule& value ) override;
        void visit( Builtin& value ) override;

        void visit( Enumeration& value ) override;
        void visit( Range& value ) override;
        void visit( Tuple& value ) override;
        void visit( List& value ) override;
        void visit( Structure& value ) override;
        void visit( Feature& value ) override;

        void visit( ParallelBlock& value ) override;
        void visit( SequentialBlock& value ) override;

        void visit( TrivialStatement& value ) override;
        void visit( BranchStatement& value ) override;

        //
        // Instructions
        //

        virtual void visit( SkipInstruction& value ) override = 0;

        virtual void visit( ForkInstruction& value ) override = 0;
        virtual void visit( MergeInstruction& value ) override = 0;

        virtual void visit( LookupInstruction& value ) override = 0;
        virtual void visit( UpdateInstruction& value ) override = 0;

        virtual void visit( LocalInstruction& value ) override = 0;
        virtual void visit( LocationInstruction& value ) override = 0;
        virtual void visit( CallInstruction& value ) override = 0;

        virtual void visit( SelectInstruction& value ) override = 0;

        virtual void visit( InvInstruction& value ) override = 0;
        virtual void visit( AddInstruction& value ) override = 0;
        virtual void visit( SubInstruction& value ) override = 0;
        virtual void visit( MulInstruction& value ) override = 0;
        virtual void visit( ModInstruction& value ) override = 0;
        virtual void visit( DivInstruction& value ) override = 0;
        virtual void visit( PowInstruction& value ) override = 0;

        virtual void visit( AndInstruction& value ) override = 0;
        virtual void visit( XorInstruction& value ) override = 0;
        virtual void visit( OrInstruction& value ) override = 0;
        virtual void visit( ImpInstruction& value ) override = 0;
        virtual void visit( NotInstruction& value ) override = 0;

        virtual void visit( EquInstruction& value ) override = 0;
        virtual void visit( NeqInstruction& value ) override = 0;
        virtual void visit( LthInstruction& value ) override = 0;
        virtual void visit( LeqInstruction& value ) override = 0;
        virtual void visit( GthInstruction& value ) override = 0;
        virtual void visit( GeqInstruction& value ) override = 0;

        //
        // Constants
        //

        virtual void visit( VoidConstant& value ) override = 0;
        virtual void visit( RuleReferenceConstant& value ) override = 0;
        virtual void visit( FunctionReferenceConstant& value ) override = 0;
        virtual void visit( BooleanConstant& value ) override = 0;
        virtual void visit( IntegerConstant& value ) override = 0;
        virtual void visit( BinaryConstant& value ) override = 0;
        virtual void visit( StringConstant& value ) override = 0;
        virtual void visit( DecimalConstant& value ) override = 0;
        virtual void visit( RationalConstant& value ) override = 0;
        virtual void visit( EnumerationConstant& value ) override = 0;
        virtual void visit( RangeConstant& value ) override = 0;
        virtual void visit( TupleConstant& value ) override = 0;
        virtual void visit( ListConstant& value ) override = 0;
        virtual void visit( DomainConstant& value ) override = 0;

        virtual void visit( Identifier& value ) override = 0;
    };

    class TraversalVisitor : public Visitor
    {
      public:
        TraversalVisitor( const Traversal order, std::function< void( Value& ) > callback );

        Traversal order( void ) const;

        std::function< void( Value& ) > callback( void ) const;

      private:
        Traversal m_order;

        std::function< void( Value& ) > m_callback;

      public:
        //
        // General
        //

        void visit( Specification& value ) override;
        void visit( Agent& value ) override;
        void visit( Function& value ) override;
        void visit( Derived& value ) override;
        void visit( Rule& value ) override;
        void visit( Builtin& value ) override;

        void visit( Enumeration& value ) override;
        void visit( Range& value ) override;
        void visit( Tuple& value ) override;
        void visit( List& value ) override;
        void visit( Structure& value ) override;
        void visit( Feature& value ) override;

        void visit( ParallelBlock& value ) override;
        void visit( SequentialBlock& value ) override;

        void visit( TrivialStatement& value ) override;
        void visit( BranchStatement& value ) override;

        //
        // Instructions
        //

        void visit( SkipInstruction& value ) override;

        void visit( ForkInstruction& value ) override;
        void visit( MergeInstruction& value ) override;

        void visit( LookupInstruction& value ) override;
        void visit( UpdateInstruction& value ) override;

        void visit( LocalInstruction& value ) override;
        void visit( LocationInstruction& value ) override;
        void visit( CallInstruction& value ) override;

        void visit( SelectInstruction& value ) override;

        void visit( InvInstruction& value ) override;
        void visit( AddInstruction& value ) override;
        void visit( SubInstruction& value ) override;
        void visit( MulInstruction& value ) override;
        void visit( ModInstruction& value ) override;
        void visit( DivInstruction& value ) override;
        void visit( PowInstruction& value ) override;

        void visit( AndInstruction& value ) override;
        void visit( XorInstruction& value ) override;
        void visit( OrInstruction& value ) override;
        void visit( ImpInstruction& value ) override;
        void visit( NotInstruction& value ) override;

        void visit( EquInstruction& value ) override;
        void visit( NeqInstruction& value ) override;
        void visit( LthInstruction& value ) override;
        void visit( LeqInstruction& value ) override;
        void visit( GthInstruction& value ) override;
        void visit( GeqInstruction& value ) override;

        //
        // Constants
        //

        void visit( VoidConstant& value ) override;
        void visit( RuleReferenceConstant& value ) override;
        void visit( FunctionReferenceConstant& value ) override;
        void visit( BooleanConstant& value ) override;
        void visit( IntegerConstant& value ) override;
        void visit( BinaryConstant& value ) override;
        void visit( StringConstant& value ) override;
        void visit( DecimalConstant& value ) override;
        void visit( RationalConstant& value ) override;
        void visit( EnumerationConstant& value ) override;
        void visit( RangeConstant& value ) override;
        void visit( TupleConstant& value ) override;
        void visit( ListConstant& value ) override;
        void visit( DomainConstant& value ) override;

        void visit( Identifier& value ) override;
    };
}

#endif  // _LIBCASM_IR_VISITOR_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

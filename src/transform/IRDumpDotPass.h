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

#ifndef _LIB_CASMIR_IR_DUMP_DOT_PASS_H_
#define _LIB_CASMIR_IR_DUMP_DOT_PASS_H_

#include "../analyze/ConsistencyCheckPass.h"

#include "../Specification.h"

/**
   @brief    TODO

   TODO
*/

namespace libcasm_ir
{
    class Value;
    class Constant;
    class Instruction;

    class IRDumpDotPass final : public libpass::Pass
    {
      public:
        static char id;

        void usage( libpass::PassUsage& pu ) override;

        u1 run( libpass::PassResult& pr ) override;
    };

    class IRDumpDotVisitor final : public RecursiveVisitor
    {
      public:
        IRDumpDotVisitor( std::ostream& stream );

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

        void visit( AssertInstruction& value ) override;
        void visit( SelectInstruction& value ) override;
        void visit( SymbolicInstruction& value ) override;

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
        void visit( BooleanConstant& value ) override;
        void visit( IntegerConstant& value ) override;
        void visit( BitConstant& value ) override;
        void visit( StringConstant& value ) override;
        void visit( FloatingConstant& value ) override;
        void visit( RationalConstant& value ) override;
        void visit( EnumerationConstant& value ) override;

        void visit( Identifier& value ) override;

      private:
        void dump( Block& value ) const;
        void dump( ExecutionSemanticsBlock& value ) const;
        void dump( Statement& value ) const;
        void dump( Instruction& value ) const;
        void dump( Constant& value ) const;

        std::ostream& m_stream;
        std::unordered_set< u8 > m_first;
    };
}

#endif // _LIB_CASMIR_IR_DUMP_DOT_PASS_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

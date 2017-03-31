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

#include "uts/main.h"

using namespace libcasm_ir;
using namespace libstdhl;
using namespace libpass;

TEST( libcasm_ir__transform_BranchEliminationPass, case_true )
{
    BranchEliminationPass pass;
    PassLogger log( &BranchEliminationPass::id, pass.stream() );

    ConsistencyCheckVisitor check( log );

#ifndef NDEBUG
    IRDumpSourceVisitor dump( std::cerr );
#endif

    auto rule = make< Rule >( TEST_NAME, make< VoidType >() );
    rule->setContext( ParallelBlock::create() );

    auto val_T = get< BooleanConstant >( true );
    auto val_F = get< BooleanConstant >( false );

    auto br0 = rule->context()->add< BranchStatement >();

    auto lbl_T = br0->add( ParallelBlock::create() );
    auto stmtT = lbl_T->add< TrivialStatement >();
    stmtT->add< SkipInstruction >();

    auto lbl_F = br0->add( ParallelBlock::create() );
    auto stmtF = lbl_F->add< TrivialStatement >();
    stmtF->add< SkipInstruction >();

    auto inst = br0->add< SelectInstruction >( val_T );
    inst->add( val_T );
    inst->add( lbl_T );
    inst->add( val_F );
    inst->add( lbl_F );

    check.visit( *rule );
    EXPECT_EQ( check.errors(), 0 );

#ifndef NDEBUG
    dump.visit( *rule );
#endif

    EXPECT_EQ( pass.optimize( rule ), 1 );

#ifndef NDEBUG
    dump.visit( *rule );
#endif

    if( check.errors() )
    {
        pass.stream().dump();
    }
}

TEST( libcasm_ir__transform_BranchEliminationPass, case_false )
{
    BranchEliminationPass pass;
    PassLogger log( &BranchEliminationPass::id, pass.stream() );

    ConsistencyCheckVisitor check( log );

#ifndef NDEBUG
    IRDumpSourceVisitor dump( std::cerr );
#endif

    auto rule = make< Rule >( TEST_NAME, make< VoidType >() );
    rule->setContext( ParallelBlock::create() );

    auto val_T = get< BooleanConstant >( true );
    auto val_F = get< BooleanConstant >( false );

    auto br0 = rule->context()->add< BranchStatement >();

    auto lbl_T = br0->add( ParallelBlock::create() );
    auto stmtT = lbl_T->add< TrivialStatement >();
    stmtT->add< SkipInstruction >();

    auto lbl_F = br0->add( ParallelBlock::create() );
    auto stmtF = lbl_F->add< TrivialStatement >();
    stmtF->add< SkipInstruction >();

    auto inst = br0->add< SelectInstruction >( val_F );
    inst->add( val_T );
    inst->add( lbl_T );
    inst->add( val_F );
    inst->add( lbl_F );

    check.visit( *rule );
    EXPECT_EQ( check.errors(), 0 );

#ifndef NDEBUG
    dump.visit( *rule );
#endif

    EXPECT_EQ( pass.optimize( rule ), 1 );

#ifndef NDEBUG
    dump.visit( *rule );
#endif

    if( check.errors() )
    {
        pass.stream().dump();
    }
}

TEST( libcasm_ir__transform_BranchEliminationPass, case_true_to_skip )
{
    BranchEliminationPass pass;
    PassLogger log( &BranchEliminationPass::id, pass.stream() );

    ConsistencyCheckVisitor check( log );

#ifndef NDEBUG
    IRDumpSourceVisitor dump( std::cerr );
#endif

    auto rule = make< Rule >( TEST_NAME, make< VoidType >() );
    rule->setContext( ParallelBlock::create() );

    auto val_T = get< BooleanConstant >( true );
    auto val_F = get< BooleanConstant >( false );

    auto br0 = rule->context()->add< BranchStatement >();

    auto lbl_T = br0->add( ParallelBlock::create() );
    auto stmtT = lbl_T->add< TrivialStatement >();
    stmtT->add< SkipInstruction >();

    auto inst = br0->add< SelectInstruction >( val_T );
    inst->add( val_T );
    inst->add( lbl_T );

    check.visit( *rule );
    EXPECT_EQ( check.errors(), 0 );

#ifndef NDEBUG
    dump.visit( *rule );
#endif

    EXPECT_EQ( pass.optimize( rule ), 1 );

#ifndef NDEBUG
    dump.visit( *rule );
#endif

    if( check.errors() )
    {
        pass.stream().dump();
    }
}

TEST( libcasm_ir__transform_BranchEliminationPass, case_false_to_skip )
{
    BranchEliminationPass pass;
    PassLogger log( &BranchEliminationPass::id, pass.stream() );

    ConsistencyCheckVisitor check( log );

#ifndef NDEBUG
    IRDumpSourceVisitor dump( std::cerr );
#endif

    auto rule = make< Rule >( TEST_NAME, make< VoidType >() );
    rule->setContext( ParallelBlock::create() );

    auto val_T = get< BooleanConstant >( true );
    auto val_F = get< BooleanConstant >( false );

    auto br0 = rule->context()->add< BranchStatement >();

    auto lbl_T = br0->add( ParallelBlock::create() );
    auto stmtT = lbl_T->add< TrivialStatement >();
    stmtT->add< SkipInstruction >();

    auto inst = br0->add< SelectInstruction >( val_F );
    inst->add( val_T );
    inst->add( lbl_T );

    check.visit( *rule );
    EXPECT_EQ( check.errors(), 0 );

#ifndef NDEBUG
    dump.visit( *rule );
#endif

    EXPECT_EQ( pass.optimize( rule ), 1 );

#ifndef NDEBUG
    dump.visit( *rule );
#endif

    // pass.stream().dump();
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

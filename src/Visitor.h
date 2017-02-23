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

#ifndef _LIB_CASMIR_VISITOR_H_
#define _LIB_CASMIR_VISITOR_H_

#include "CasmIR.h"
//#include "Type.h"

namespace libcasm_ir
{
    class Value;
    class Specification;
    class Agent;

    class Function;
    class Rule;
    class Derived;
    class Builtin;

    class ParallelBlock;
    class SequentialBlock;

    class TrivialStatement;
    class BranchStatement;

    class LocalInstruction;

    class AssertInstruction;
    class SelectInstruction;

    class SkipInstruction;

    class ForkInstruction;
    class MergeInstruction;

    class LocationInstruction;
    class LookupInstruction;
    class UpdateInstruction;

    class CallInstruction;
    class PrintInstruction;

    class AddInstruction;
    class SubInstruction;
    class MulInstruction;
    class ModInstruction;
    class DivInstruction;

    class AndInstruction;
    class XorInstruction;
    class OrInstruction;

    class EquInstruction;
    class NeqInstruction;
    class LthInstruction;

    class AgentConstant;
    class RuleReferenceConstant;
    class BooleanConstant;
    class IntegerConstant;
    class BitConstant;
    class StringConstant;

    enum class Traversal
    {
        PREORDER,
        POSTORDER
    };

    class Context : public CasmIR
    {
    };

    class Visitor : public CasmIR
    {
      public:
        enum class Stage
        {
            PROLOG,
            INTERLOG,
            EPILOG
        };

        virtual ~Visitor( void ) = default;

        virtual void dispatch( Stage stage, Value& value, Context& cxt ) final;

#define LIB_CASMIR_VISITOR_INTERFACE_( PREFIX, POSTFIX )                       \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::Specification& value, libcasm_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::Specification& value, libcasm_ir::Context& cxt ) POSTFIX;  \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::Agent& value, libcasm_ir::Context& cxt ) POSTFIX;          \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::Agent& value, libcasm_ir::Context& cxt ) POSTFIX;          \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::Function& value, libcasm_ir::Context& cxt ) POSTFIX;       \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::Function& value, libcasm_ir::Context& cxt ) POSTFIX;       \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::Derived& value, libcasm_ir::Context& cxt ) POSTFIX;        \
    PREFIX void visit_interlog(                                                \
        libcasm_ir::Derived& value, libcasm_ir::Context& cxt ) POSTFIX;        \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::Derived& value, libcasm_ir::Context& cxt ) POSTFIX;        \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::Rule& value, libcasm_ir::Context& cxt ) POSTFIX;           \
    PREFIX void visit_interlog(                                                \
        libcasm_ir::Rule& value, libcasm_ir::Context& cxt ) POSTFIX;           \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::Rule& value, libcasm_ir::Context& cxt ) POSTFIX;           \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::Builtin& value, libcasm_ir::Context& cxt ) POSTFIX;        \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::Builtin& value, libcasm_ir::Context& cxt ) POSTFIX;        \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::ParallelBlock& value, libcasm_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::ParallelBlock& value, libcasm_ir::Context& cxt ) POSTFIX;  \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::SequentialBlock& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::SequentialBlock& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::TrivialStatement& value,             \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::TrivialStatement& value,             \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::BranchStatement& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_interlog( libcasm_ir::BranchStatement& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::BranchStatement& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::LocalInstruction& value,             \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::LocalInstruction& value,             \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::LocationInstruction& value,          \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::LocationInstruction& value,          \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::AssertInstruction& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::AssertInstruction& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::SelectInstruction& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::SelectInstruction& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::SkipInstruction& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::SkipInstruction& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::ForkInstruction& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::ForkInstruction& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::MergeInstruction& value,             \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::MergeInstruction& value,             \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::LookupInstruction& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::LookupInstruction& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::UpdateInstruction& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::UpdateInstruction& value,            \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::CallInstruction& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::CallInstruction& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::PrintInstruction& value,             \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::PrintInstruction& value,             \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    /* ArithmeticInstruction */                                                \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::AddInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::AddInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::SubInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::SubInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::MulInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::MulInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::ModInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::ModInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::DivInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::DivInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    /* LogicalInstruction */                                                   \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::AndInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::AndInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::XorInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::XorInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::OrInstruction& value, libcasm_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::OrInstruction& value, libcasm_ir::Context& cxt ) POSTFIX;  \
                                                                               \
    /* CompareInstruction */                                                   \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::EquInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::EquInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::NeqInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::NeqInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::LthInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::LthInstruction& value, libcasm_ir::Context& cxt ) POSTFIX; \
                                                                               \
    /* Constant */                                                             \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::AgentConstant& value, libcasm_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::AgentConstant& value, libcasm_ir::Context& cxt ) POSTFIX;  \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::RuleReferenceConstant& value,        \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::RuleReferenceConstant& value,        \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::BooleanConstant& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::BooleanConstant& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcasm_ir::IntegerConstant& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcasm_ir::IntegerConstant& value,              \
        libcasm_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::BitConstant& value, libcasm_ir::Context& cxt ) POSTFIX;    \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::BitConstant& value, libcasm_ir::Context& cxt ) POSTFIX;    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcasm_ir::StringConstant& value, libcasm_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcasm_ir::StringConstant& value, libcasm_ir::Context& cxt ) POSTFIX

#define LIB_CASMIR_VISITOR_INTERFACE LIB_CASMIR_VISITOR_INTERFACE_(, override )

        LIB_CASMIR_VISITOR_INTERFACE_( virtual, = 0 );
    };
}

#endif // _LIB_CASMIR_VISITOR_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

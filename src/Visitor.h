//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  

#ifndef _LIB_CASMIR_VISITOR_H_
#define _LIB_CASMIR_VISITOR_H_

#include "CasmIR.h"
#include "Type.h"

// #include "Instruction.h"
// #include "Scope.h"

// #include "Module.h"
// #include "Component.h"
// #include "Function.h"
// #include "Reference.h"
// #include "Memory.h"



namespace libcasm_ir
{
	class Value;
	class Specification;
	class Agent;
	class Function;
	class Rule;

	class ParallelBlock;
	class SequentialBlock;
	class TrivialStatement;
	
	class LocationInstruction;
	class LookupInstruction;
	class UpdateInstruction;
	class AddInstruction;
	class DivInstruction;
	class AndInstruction;

	class IntegerConstant;
	
	enum class Traversal
	{ PREORDER
	, POSTORDER
    };
	
	class Visitor //: public CasmIR
	{
    public:
		enum class Stage
		{ PROLOG
		, INTERLOG
		, EPILOG
		};
		
		virtual void dispatch( Stage stage, Value* value ) final;
	    
#define LIB_CASMIR_VISITOR_INTERFACE_( PREFIX, POSTFIX )				\
		PREFIX void visit_prolog( libcasm_ir::Specification& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::Specification& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::Agent& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::Agent& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::Function& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::Function& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::Rule& value ) POSTFIX; \
		PREFIX void visit_interlog( libcasm_ir::Rule& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::Rule& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::ParallelBlock& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::ParallelBlock& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::SequentialBlock& value ) POSTFIX;	\
		PREFIX void visit_epilog( libcasm_ir::SequentialBlock& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( libcasm_ir::TrivialStatement& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::TrivialStatement& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::LocationInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::LocationInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::LookupInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::LookupInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::UpdateInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::UpdateInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::AddInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::AddInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::DivInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::DivInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::AndInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::AndInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( libcasm_ir::IntegerConstant& value ) POSTFIX; \
		PREFIX void visit_epilog( libcasm_ir::IntegerConstant& value ) POSTFIX
		
		LIB_CASMIR_VISITOR_INTERFACE_( virtual, = 0 );
	};
}

#define LIB_CASMIR_VISITOR_INTERFACE LIB_CASMIR_VISITOR_INTERFACE_(,)


#endif /* _LIB_CASMIR_VISITOR_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//  WITH THE SOFTWARE.
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

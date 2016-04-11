//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
//  
//  This software is licensed under the Open Software License v. 3.0 (OSL-3.0).
//  You either find the license text below or in the root directory of this 
//  project in the LICENSE.txt file. If you cannot find the LICENSE.txt file,
//  please refer to https://opensource.org/licenses/OSL-3.0.
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

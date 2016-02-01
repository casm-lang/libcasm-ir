//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/casm-ir
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

#ifndef _LIB_CASMIR_ASTTOCASMIRPASS_H_
#define _LIB_CASMIR_ASTTOCASMIRPASS_H_

#include "Pass.h"
#include "libcasm-ir.h"

#include "AstInterface.h"
#include "TypeCheckPass.h"

/**
   @brief    TODO
   
   TODO
*/

#define T bool

namespace libcasm_ir
{
	class AstToCasmIRPass : public libpass::Pass, AstInterface< bool >
	{
	public:
		static char id;

		virtual bool run( libpass::PassResult& pr );
	
	private:
		std::unordered_map< AstNode*, Value* >   ast2casmir;
		std::unordered_map< AstNode*, AstNode* > ast2parent;

		std::vector< Value* > current_scope;

		Specification* specification;
		
	public:
		Specification* getSpecification( void ) const;
		
		void visit_init( UnaryNode* node );
		void visit_specification( AstNode* node );	
		void visit_body_elements( AstNode* node );
		void visit_function_def( FunctionDefNode* node, const std::vector<std::pair<T, T>>& inits );
		void visit_derived_function_atom_pre( FunctionAtom* node, T args[], uint16_t argc );
		void visit_derived_def_pre( FunctionDefNode* node );
		void visit_derived_def( FunctionDefNode* node, T expr );
		void visit_rule( RuleNode* node );
		void visit_rule_post( RuleNode* node );
		void visit_skip(AstNode* node);
		void visit_statements( AstListNode* node );
		void visit_parblock( UnaryNode* node );
		void visit_seqblock( UnaryNode* node );
		void visit_forall_pre( AstNode* node );
		void visit_forall_post( AstNode* node );
		void visit_iterate( AstNode* node );
		void visit_update( UpdateNode* node, T func, T expr );
		void visit_update_dumps( UpdateNode* node, T func, T expr );
		void visit_update_subrange( UpdateNode* node, T func, T expr);
		void visit_call_pre( CallNode* node );
		void visit_call_pre( CallNode* node, T expr );
		void visit_call( CallNode* node, std::vector< T >& args );
		void visit_call_post( CallNode* node );
		void visit_print( PrintNode* node, std::vector< T >& args );
		void visit_diedie( DiedieNode* node, T msg );
		void visit_impossible( AstNode* node );
		void visit_assert( UnaryNode* node, T expr );
		void visit_assure( UnaryNode* node, T expr );
		void visit_let( LetNode* node, T var );
		void visit_let_post( LetNode* node );
		void visit_push( PushNode* node, T expr, T atom );
		void visit_pop( PopNode* node );
		void visit_ifthenelse( IfThenElseNode* node, T cond );
		void visit_case_pre( CaseNode* node, T val );
		void visit_case( CaseNode* node, T val, const std::vector< T >& case_labels );
		T visit_expression( Expression* node, T lhs, T rhs );
		T visit_expression_single( Expression* node, T val );
		T visit_function_atom( FunctionAtom* node, T args[], uint16_t argc );
		T visit_function_atom_subrange( FunctionAtom* node, T args[], uint16_t argc	);
		T visit_derived_function_atom( FunctionAtom* node, T expr );
		T visit_int_atom( IntegerAtom* node );
		T visit_bit_atom( IntegerAtom* node );
		T visit_float_atom( FloatAtom* node );
		T visit_rational_atom( RationalAtom* node );
		T visit_undef_atom( UndefAtom* node );
		T visit_self_atom( SelfAtom* node );
		T visit_rule_atom( RuleAtom* node );
		T visit_boolean_atom( BooleanAtom* node );
		T visit_string_atom( StringAtom* node );
		T visit_list_atom( ListAtom* node, std::vector< T >& args );
		T visit_number_range_atom( NumberRangeAtom* node );
		T visit_builtin_atom( BuiltinAtom* node, T args[], uint16_t argc );

	private:
		template< class C >
		C* lookupParent( AstNode* node );
	
		template< class C >
		C* lookup( AstNode* node );
	};
}

#endif /* _LIB_CASMIR_PASSASTTOCASMIR_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

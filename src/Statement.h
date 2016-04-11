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

#ifndef _LIB_CASMIR_STATEMENT_H_
#define _LIB_CASMIR_STATEMENT_H_

#include "Block.h"

namespace libcasm_ir
{
	class Statement : public Block
	{
    protected:
		ExecutionSemanticsBlock* scope;
		
	private:
		std::vector< Value* > instructions;
		
	public:
		Statement( const char* name, Type* type, ExecutionSemanticsBlock* scope
				 , Value::ID id = Value::STATEMENT );
	    
		ExecutionSemanticsBlock* getScope( void ) const;

		const std::vector< Value* >& getInstructions( void ) const;
		
		void add( Value* instruction );

		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	class TrivialStatement : public Statement
	{
	public:
		TrivialStatement( ExecutionSemanticsBlock* scope = 0 );

		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};


	
	class BranchStatement : public Statement
	{
	private:
		std::vector< Block* > blocks;
		
	public:
		BranchStatement( ExecutionSemanticsBlock* scope = 0 );
	    
		void addBlock( Value* block );

		const std::vector< Block* >& getBlocks( void ) const;
		
		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};

}


#endif /* _LIB_CASMIR_STATEMENT_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

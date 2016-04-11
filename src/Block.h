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

#ifndef _LIB_CASMIR_BLOCK_H_
#define _LIB_CASMIR_BLOCK_H_

#include "stdhl/cpp/Binding.h"

#include "Value.h"

namespace libcasm_ir
{
	class Statement;
	
	class Block : public Value
	{
	public:
		Block( const char* name, Type* type, Value::ID id = Value::BLOCK );
	    
		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	class ExecutionSemanticsBlock : public Block
	{
	private:
		const u1 is_parallel;
		u64 pseudo_state;
		ExecutionSemanticsBlock* parent;
		std::vector< Block* > blocks;
		
	public:
		ExecutionSemanticsBlock
		( const char* name
		, Type* type
		, const u1 is_parallel
		, ExecutionSemanticsBlock* parent = 0
		, Value::ID id = Value::EXECUTION_SEMANTICS_BLOCK
		);
		
		const u1 isParallel( void ) const;
		
		const u64 getPseudoState( void ) const;
		
	    ExecutionSemanticsBlock* getParent( void ) const;
		
		void setParent( ExecutionSemanticsBlock* parent_block );

		const std::vector< Block* >& getBlocks( void ) const;

		void add( Block* block );
		
	    void dump( void ) const;
		
		static bool classof( Value const* obj );
	};

	
	class ParallelBlock : public ExecutionSemanticsBlock, public libstdhl::Binding< Rule >
	{
	public:
		ParallelBlock( ExecutionSemanticsBlock* parent = 0 );
		
		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	class SequentialBlock : public ExecutionSemanticsBlock
	{
	public:
		SequentialBlock( ExecutionSemanticsBlock* parent = 0 );
	    
		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_CASMIR_BASICBLOCK_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

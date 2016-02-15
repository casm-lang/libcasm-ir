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

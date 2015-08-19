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

#include "Block.h"
#include "Statement.h"

using namespace libcasm_ir;


Block::Block( const char* name, Type* type, Value::ID id )
: Value( name, type, id )
{
}

void Block::dump( void ) const
{
	// printf( "[Block] %p\n", this );
	((Value*)this)->dump();
}

bool Block::classof( Value const* obj )
{
	return obj->getValueID() == Value::BLOCK
		or ExecutionSemanticsBlock::classof( obj )
		or Statement::classof( obj );
}



ExecutionSemanticsBlock::ExecutionSemanticsBlock
( const char* name
, Type* type
, const u1 is_parallel
, ExecutionSemanticsBlock* parent
, Value::ID id
)
: Block( name, type, id )
, is_parallel( is_parallel )
, pseudo_state( 0 )
, parent( parent )
{
	setParent( parent );
}

const u1 ExecutionSemanticsBlock::isParallel( void ) const
{
	return is_parallel;
}
		
const u64 ExecutionSemanticsBlock::getPseudoState( void ) const
{
	return pseudo_state;
}
		
ExecutionSemanticsBlock* ExecutionSemanticsBlock::getParent( void ) const
{
	return parent;
}
		
void ExecutionSemanticsBlock::setParent( ExecutionSemanticsBlock* parent_block )
{
	parent = parent_block;
			
	if( parent )
	{
		pseudo_state = parent->getPseudoState();

		if( parent->isParallel() != this->isParallel() )
		{
			pseudo_state++;
		}
	}
}
		
void ExecutionSemanticsBlock::add( Block* block )
{
	assert( block );
			
			
	if( Value::isa< ExecutionSemanticsBlock >( block ) )
	{
		ExecutionSemanticsBlock* inner = static_cast< ExecutionSemanticsBlock* >( block );
		inner->setParent( this );
	}
			
	blocks.push_back( block );
}
		
void ExecutionSemanticsBlock::dump( void ) const
{
	printf( "[ESBlk] %p, %p, %u @ %lu\n"
			, this, parent, isParallel(), getPseudoState() );
			
	for( Block* block : blocks )
	{
		assert( block );

		block->dump();
	}
}
		




ParallelBlock::ParallelBlock( ExecutionSemanticsBlock* parent )
: ExecutionSemanticsBlock( "par", 0, true, parent, Value::PARALLEL_BLOCK )
{
}

void ParallelBlock::dump( void ) const
{
	((ExecutionSemanticsBlock*)this)->dump();
}
		


SequentialBlock::SequentialBlock( ExecutionSemanticsBlock* parent )
: ExecutionSemanticsBlock( "seq", 0, false, parent, Value::SEQUENTIAL_BLOCK )
{
}
		
void SequentialBlock::dump( void ) const
{
	((ExecutionSemanticsBlock*)this)->dump();
}





bool ExecutionSemanticsBlock::classof( Value const* obj )
{
	return obj->getValueID() == Value::EXECUTION_SEMANTICS_BLOCK
		or ParallelBlock::classof( obj )
		or SequentialBlock::classof( obj );
}

bool ParallelBlock::classof( Value const* obj )
{
	return obj->getValueID() == Value::PARALLEL_BLOCK;
}

bool SequentialBlock::classof( Value const* obj )
{
	return obj->getValueID() == Value::SEQUENTIAL_BLOCK;
}



//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

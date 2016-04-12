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

const std::vector< Block* >& ExecutionSemanticsBlock::getBlocks( void ) const
{
	return blocks;
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

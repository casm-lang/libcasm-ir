//  
//  Copyright (c) 2015-2016 CASM Organization
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
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

#include "Block.h"
#include "Statement.h"

using namespace libcasm_ir;


Block::Block( const char* name, Type* type, Value::ID id )
: Value( name, type, id )
, parent( 0 )
{
}

void Block::setParent( Value* parent )
{
    assert( parent );
    this->parent = parent;
}

Value* Block::getParent( void ) const
{
    return parent;
}

void Block::dump( void ) const
{
    // printf( "[Block] %p\n", this );
    ((Value*)this)->dump();
}

bool Block::classof( Value const* obj )
{
    return obj->getValueID() == classid()
        or ExecutionSemanticsBlock::classof( obj )
        or Statement::classof( obj );
}



ExecutionSemanticsBlock::ExecutionSemanticsBlock
( const char* name
, Type* type
, const u1 is_parallel
, ExecutionSemanticsBlock* scope
, Value::ID id
)
: Block( name, type, id )
, is_parallel( is_parallel )
, pseudo_state( 0 )
, scope( scope )
{
    setScope( scope );
}

const u1 ExecutionSemanticsBlock::isParallel( void ) const
{
    return is_parallel;
}
        
const u64 ExecutionSemanticsBlock::getPseudoState( void ) const
{
    return pseudo_state;
}
        
ExecutionSemanticsBlock* ExecutionSemanticsBlock::getScope( void ) const
{
    return scope;
}
        
void ExecutionSemanticsBlock::setScope( ExecutionSemanticsBlock* scope_block )
{
    scope = scope_block;
            
    if( scope )
    {
        pseudo_state = scope->getPseudoState();

        if( scope->isParallel() != this->isParallel() )
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
        inner->setScope( this );
    }
            
    blocks.push_back( block );
}
        
void ExecutionSemanticsBlock::dump( void ) const
{
    printf( "[ESBlk] %p, %p, %u @ %lu\n"
            , this, scope, isParallel(), getPseudoState() );
            
    for( Block* block : blocks )
    {
        assert( block );

        block->dump();
    }
}
        




ParallelBlock::ParallelBlock( ExecutionSemanticsBlock* scope )
: ExecutionSemanticsBlock( "par", 0, true, scope, Value::PARALLEL_BLOCK )
{
}

void ParallelBlock::dump( void ) const
{
    ((ExecutionSemanticsBlock*)this)->dump();
}
        


SequentialBlock::SequentialBlock( ExecutionSemanticsBlock* scope )
: ExecutionSemanticsBlock( "seq", 0, false, scope, Value::SEQUENTIAL_BLOCK )
{
}
        
void SequentialBlock::dump( void ) const
{
    ((ExecutionSemanticsBlock*)this)->dump();
}





bool ExecutionSemanticsBlock::classof( Value const* obj )
{
    return obj->getValueID() == classid()
        or ParallelBlock::classof( obj )
        or SequentialBlock::classof( obj );
}

bool ParallelBlock::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

bool SequentialBlock::classof( Value const* obj )
{
    return obj->getValueID() == classid();
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

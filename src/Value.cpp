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

/**
   @file     Value.cpp
   @author   Philipp Paulweber
   @date     2015-02-20
*/

#include "Value.h"
#include "libcasm-ir.h"
#include "Specification.h"

using namespace libcasm_ir;


Value::Value( const char* name, Type* type, Value::ID id )
: name( name )
, type( type )
, id( id )
, type_lock( false )
{
	SymbolTable& symbols = *getSymbols();
	symbols[ name ].insert( this );
	printf( "[Value] created '%s' @ %p", name, this );
	if( type )
	{
		printf( " of type '%s' (=0x%lx)", type->getName(), type->getID() );
	}
	printf( "\n" );
}

Value::~Value()
{
	SymbolTable& symbols = *getSymbols();
	symbols[ name ].erase( this );
	printf( "[Value] deleted '%s' @ %p of type %p\n", name, this, type );
}

const char* Value::getName( void ) const
{
	return name;
}

Type* Value::getType( void ) const
{
	return type;
}

void Value::setType( Type* type )
{
    assert( !type_lock );
	type_lock = true;
	
	this->type = type;
}


Value::ID Value::getValueID() const
{
	return id;
}

void Value::debug( void ) const
{
	printf( "%p '%s' : ", this, getName() );
	if( getType() )
	{
		printf( "%s", getType()->getName() );
	}
	printf( "\n" );
}

void Value::dump( void ) const
{    
	switch( this->getValueID() )
	{
	case Value::AGENT:
		((Rule*)this)->dump(); break;
	case Value::RULE:
		((Rule*)this)->dump(); break;
	case Value::DERIVED:
		((Derived*)this)->dump(); break;
	case Value::FUNCTION:
		((Function*)this)->dump(); break;
	
	case Value::BLOCK:
		((Block*)this)->dump(); break;
	case Value::EXECUTION_SEMANTICS_BLOCK:
		((ExecutionSemanticsBlock*)this)->dump(); break;
	case Value::PARALLEL_BLOCK:
		((ParallelBlock*)this)->dump(); break;
	case Value::SEQUENTIAL_BLOCK:
		((SequentialBlock*)this)->dump(); break;
		
	case Value::STATEMENT:
		((Statement*)this)->dump(); break;
	case Value::TRIVIAL_STATEMENT:
		((TrivialStatement*)this)->dump(); break;
	case Value::BRANCH_STATEMENT:
		((BranchStatement*)this)->dump(); break;
		
	case Value::CONSTANT:
		((ConstantValue*)this)->dump(); break;
	case Value::IDENTIFIER:
		((Identifier*)this)->dump(); break;
	case Value::INTEGER_CONSTANT:
		((IntegerConstant*)this)->dump(); break;
	
	default:
		if( Value::isa< Instruction >( this ) )
		{
			((Instruction*)this)->dump();
		}
		else
		{
			debug();
		}
	}
}


void Value::iterate( Traversal order, Visitor* visitor, std::function< void( Value* ) > action )
{
	if( order == Traversal::PREORDER )
	{
	    action( /*order, */ this );
	}
	
	if( visitor )
	{
		visitor->dispatch( Visitor::Stage::PROLOG, this );
	}

	if( Value::isa< Specification >( this ) )
	{
	    Specification* obj = ((Specification*)this);
		
		for( Value* p : obj->getContent() )
		{
			p->iterate( order, visitor, action );
		}
	}
	else if( Value::isa< Rule >( this ) )
	{
	    Rule* obj = ((Rule*)this);
		
		// for( Value* p : obj->getParameters() )
		// {
		// 	p->iterate( order, visitor, action );
		// }
				
		if( visitor )
		{
			visitor->dispatch( Visitor::Stage::INTERLOG, this );
		}

		Value* context = obj->getContext();
		assert( context );
		
	    context->iterate( order, visitor, action );
	}
	else if( Value::isa< ExecutionSemanticsBlock >( this ) )
	{
		for( Value* block : ((ExecutionSemanticsBlock*)this)->getBlocks() )
		{
			assert( block );
			block->iterate( order, visitor, action );
		}
	}
	else if( Value::isa< TrivialStatement >( this ) )
	{
		for( Value* instr : ((TrivialStatement*)this)->getInstructions() )
		{
			assert( instr );
			instr->iterate( order, visitor, action );
		}
	}
	
	
	// else if( Value::isa< Scope >( this ) )
	// {
	// 	for( Block* block : ((Scope*)this)->getBlocks() )
	// 	{
	// 		assert( block );
	// 		block->iterate( order, visitor, action );
	// 	}
	// }
	
	if( visitor )
	{
		visitor->dispatch( Visitor::Stage::EPILOG, this );
	}
	
	if( order == Traversal::POSTORDER )
	{
	    action( /*order, */ this );
	}
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

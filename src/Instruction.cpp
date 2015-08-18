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

#include "Instruction.h"

using namespace libcasm_ir;


Instruction::Instruction( const char* name, Type* type, Value::ID id )
: User( name, type, id )
, statement( 0 )
{
}

void Instruction::setStatement( Statement* stmt )
{
	statement = stmt;
	
	for( auto value : values )
	{
		if( Value::isa< Instruction >( value ) )
		{
			printf( "[Instr] %s: %p, %p\n", __FUNCTION__, stmt, value );
			stmt->add( static_cast< Instruction* >( value ) );
		}
	}
}

void Instruction::add( Value* value )
{
	assert( value );
	
	if( Value::isa< UnaryInstruction >( this ) )
	{
		assert( values.size() < 1 );	
	}
	else if( Value::isa< BinaryInstruction >( this ) )
	{
		assert( values.size() < 2 );		
	}
	
	values.push_back( value );
}

Value* Instruction::getValue( u8 index ) const
{
	assert( index < values.size() );
	return values[ index ];
}

bool Instruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::INSTRUCTION
		or BinaryInstruction::classof( obj )
		or UnaryInstruction::classof( obj )
		or LocationInstruction::classof( obj );
}



UnaryInstruction::UnaryInstruction( const char* name, Type* type, Value* value, Value::ID id )
: Instruction( name, type, id )
, value( value )
{
	add( value );
}	  

Value* UnaryInstruction::get( void ) const
{
	return getValue( 0 );
}

bool UnaryInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::UNARY_INSTRUCTION
		or LookupInstruction::classof( obj );
}



BinaryInstruction::BinaryInstruction( const char* name, Type* type, Value* lhs, Value* rhs, Value::ID id )
: Instruction( name, type, id )
{
	add( lhs );
	add( rhs );
}	  

Value* BinaryInstruction::getLHS( void ) const
{
	return getValue( 0 );
}

Value* BinaryInstruction::getRHS( void ) const
{
	return getValue( 1 );
}

bool BinaryInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::BINARY_INSTRUCTION
		or UpdateInstruction::classof( obj )
		or OperatorInstruction::classof( obj );
}







UpdateInstruction::UpdateInstruction( Value* func, Value* expr )
: BinaryInstruction( "update", 0, func, expr, Value::UPDATE_INSTRUCTION )
{
}

void UpdateInstruction::dump( void ) const
{
	printf( "[UpdIn] %p\n", this );
}

bool UpdateInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::UPDATE_INSTRUCTION;
}




LookupInstruction::LookupInstruction( Value* location )
: UnaryInstruction( "lookup", 0, location, Value::LOOKUP_INSTRUCTION )
{
}

void LookupInstruction::dump( void ) const
{
	printf( "[LupIn] %p\n", this );
}

bool LookupInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::LOOKUP_INSTRUCTION;
}




LocationInstruction::LocationInstruction( Value* function )
: Instruction( "location", 0, Value::LOCATION_INSTRUCTION )
{
	add( function );
}

void LocationInstruction::dump( void ) const
{
	printf( "[LocIn] %p\n", this );
}

bool LocationInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::LOCATION_INSTRUCTION;
}





OperatorInstruction::OperatorInstruction
( const char* name, Type* type, Value* lhs, Value* rhs, Value::ID id )
: BinaryInstruction( name, type, lhs, rhs, id )
{
}

void OperatorInstruction::dump( void ) const
{
	printf( "[OpcIn] %p = %s %p, %p\n", this, getName(), getLHS(), getRHS() );	
}

bool OperatorInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::OPERATOR_INSTRUCTION;
}


AddInstruction::AddInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( "add", 0, lhs, rhs ) //, Value::ADD_INSTRUCTION )
{	
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

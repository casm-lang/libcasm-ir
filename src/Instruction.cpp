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

const Statement* Instruction::getStatement( void ) const
{
	assert( statement );
	return statement;
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

const std::vector< Value* >& Instruction::getValues( void ) const
{
	return values;
}

void Instruction::dump( void ) const
{
	printf( "[Instr] %p %s ", this, &getName()[1] );
	u1 flag = 0;
	for( auto instr : values )
	{
		if( flag )
		{
			printf( ", " );
		}
		else
		{
			flag = 1;
		}
		printf( "%p", instr );
	}
	printf( "\n" );
}

bool Instruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::INSTRUCTION
		or BinaryInstruction::classof( obj )
		or UnaryInstruction::classof( obj )
		or LocationInstruction::classof( obj )
		or CallInstruction::classof( obj )
		or PrintInstruction::classof( obj )
		;
}



UnaryInstruction::UnaryInstruction( const char* name, Type* type, Value* value, Value::ID id )
: Instruction( name, type, id )
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
		or LetInstruction::classof( obj )
		or OperatorInstruction::classof( obj )
		;
}







UpdateInstruction::UpdateInstruction( Value* location, Value* expr )
: BinaryInstruction( ".update", 0, location, expr, Value::UPDATE_INSTRUCTION )
{
	assert( location->getType() );
	assert( expr->getType() );
	setType( expr->getType() );
}

bool UpdateInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::UPDATE_INSTRUCTION;
}


LetInstruction::LetInstruction( Value* ident, Value* expr )
: BinaryInstruction( ".let", 0, ident, expr, Value::LET_INSTRUCTION )
{
	assert( expr->getType() );
	setType( expr->getType() );
}

bool LetInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::LET_INSTRUCTION;
}




LookupInstruction::LookupInstruction( Value* location )
: UnaryInstruction( ".lookup", 0, location, Value::LOOKUP_INSTRUCTION )
{
	assert( location->getType() );
	setType( location->getType()->getResultType() );
}

bool LookupInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::LOOKUP_INSTRUCTION;
}



LocationInstruction::LocationInstruction( Value* function )
: Instruction( ".location", 0, Value::LOCATION_INSTRUCTION )
{
	add( function );
	
	assert( function->getType() );
	setType( function->getType() );
}

bool LocationInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::LOCATION_INSTRUCTION;
}


CallInstruction::CallInstruction( Value* symbol )
: Instruction( ".call", 0, Value::CALL_INSTRUCTION )
{
	add( symbol );
}

bool CallInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::CALL_INSTRUCTION;
}



PrintInstruction::PrintInstruction( Value* channel )
: Instruction( ".print", 0, Value::PRINT_INSTRUCTION )
{
	if( channel )
	{
		assert( 0 && "debug channel not implemented yet!" );
	}
}

bool PrintInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::PRINT_INSTRUCTION;
}




OperatorInstruction::OperatorInstruction
( const char* name, Type* type, Value* lhs, Value* rhs, Value::ID id )
: BinaryInstruction( name, type, lhs, rhs, id )
{
	assert( getLHS()->getType() );
	assert( getRHS()->getType() );
	assert( getLHS()->getType()->getID() == getRHS()->getType()->getID() );

	if( !getType() )
	{
		// dynamic type assignement, lert and right is the same type
		// just use the left one for now!

		setType( getLHS()->getType() );
	}
}

bool OperatorInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::OPERATOR_INSTRUCTION
		or AddInstruction::classof( obj )
		or SubInstruction::classof( obj )
		or MulInstruction::classof( obj )
		or DivInstruction::classof( obj )
		or RivInstruction::classof( obj )
		or ModInstruction::classof( obj )
		or EquInstruction::classof( obj )
		or NeqInstruction::classof( obj )
		or LthInstruction::classof( obj )
		or LeqInstruction::classof( obj )
		or GthInstruction::classof( obj )
		or GeqInstruction::classof( obj )
		or  OrInstruction::classof( obj )
		or XorInstruction::classof( obj )
		or AndInstruction::classof( obj )
		or NotInstruction::classof( obj )
		;
}


AddInstruction::AddInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".add", 0, lhs, rhs, Value::ADD_INSTRUCTION )
{
}
bool AddInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::ADD_INSTRUCTION;
}

SubInstruction::SubInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".sub", 0, lhs, rhs, Value::SUB_INSTRUCTION )
{	
}
bool SubInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::SUB_INSTRUCTION;
}

MulInstruction::MulInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".mul", 0, lhs, rhs, Value::MUL_INSTRUCTION )
{	
}
bool MulInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::MUL_INSTRUCTION;
}

DivInstruction::DivInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".div", 0, lhs, rhs, Value::DIV_INSTRUCTION )
{	
}
bool DivInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::DIV_INSTRUCTION;
}

RivInstruction::RivInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".riv", 0, lhs, rhs, Value::RIV_INSTRUCTION )
{	
}
bool RivInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::RIV_INSTRUCTION;
}

ModInstruction::ModInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".mod", 0, lhs, rhs, Value::MOD_INSTRUCTION )
{	
}
bool ModInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::MOD_INSTRUCTION;
}



EquInstruction::EquInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".equ", &BooleanType, lhs, rhs, Value::EQU_INSTRUCTION )
{	
}
bool EquInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::EQU_INSTRUCTION;
}

NeqInstruction::NeqInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".neq", &BooleanType, lhs, rhs, Value::NEQ_INSTRUCTION )
{	
}
bool NeqInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::NEQ_INSTRUCTION;
}

LthInstruction::LthInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".lth", &BooleanType, lhs, rhs, Value::LTH_INSTRUCTION )
{	
}
bool LthInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::LTH_INSTRUCTION;
}

LeqInstruction::LeqInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".leq", &BooleanType, lhs, rhs, Value::LEQ_INSTRUCTION )
{	
}
bool LeqInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::LEQ_INSTRUCTION;
}

GthInstruction::GthInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".gth", &BooleanType, lhs, rhs, Value::GTH_INSTRUCTION )
{	
}
bool GthInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::GTH_INSTRUCTION;
}

GeqInstruction::GeqInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".geq", &BooleanType, lhs, rhs, Value::GEQ_INSTRUCTION )
{	
}
bool GeqInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::GEQ_INSTRUCTION;
}

OrInstruction::OrInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".or", &BooleanType, lhs, rhs, Value::OR_INSTRUCTION )
{	
}
bool OrInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::OR_INSTRUCTION;
}

XorInstruction::XorInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".xor", &BooleanType, lhs, rhs, Value::XOR_INSTRUCTION )
{	
}
bool XorInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::XOR_INSTRUCTION;
}

AndInstruction::AndInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".and", &BooleanType, lhs, rhs, Value::AND_INSTRUCTION )
{	
}
bool AndInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::AND_INSTRUCTION;
}




NotInstruction::NotInstruction( Value* lhs )
: UnaryInstruction( ".not", &BooleanType, lhs, Value::NOT_INSTRUCTION )
{
	assert( get()->getType() );
	assert( get()->getType()->getID() == getType()->getID() );
}
bool NotInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::NOT_INSTRUCTION;
}

MovInstruction::MovInstruction( Value* lhs )
: UnaryInstruction( ".mov", 0, lhs, Value::MOV_INSTRUCTION )
{	
	assert( get()->getType() );
	setType( get()->getType() );
}
bool MovInstruction::classof( Value const* obj )
{
	return obj->getValueID() == Value::MOV_INSTRUCTION;
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

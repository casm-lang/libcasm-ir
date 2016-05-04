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

#include "Instruction.h"
#include "Derived.h"

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
			Instruction* instr = (Instruction*)value;
			if( instr->getStatement() == 0 )
			{			
				stmt->add( ( value ) );
			}
			else if( instr->getStatement() != stmt )
			{
				fprintf
				( stderr
				, "error: %s:%i: Instruction %p does belong to a different Statement block\n"
				, __FUNCTION__, __LINE__, value
				);
				assert(0);
			}
		}
	}
}

const Statement* Instruction::getStatement( void ) const
{
	//assert( statement );
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
	return obj->getValueID() == classid()
		or BinaryInstruction::classof( obj )
		or UnaryInstruction::classof( obj )
		or SkipInstruction::classof( obj )
		or LocationInstruction::classof( obj )
		or CallInstruction::classof( obj )
		or PrintInstruction::classof( obj )
		or SwitchInstruction::classof( obj )
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
	return obj->getValueID() == classid()
		or AssertInstruction::classof( obj )
		or LookupInstruction::classof( obj )
	    ;
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
	return obj->getValueID() == classid()
		or UpdateInstruction::classof( obj )
		or LetInstruction::classof( obj )
		or OperatorInstruction::classof( obj )
		;
}




SkipInstruction::SkipInstruction( void )
: Instruction( ".skip", 0, Value::SKIP_INSTRUCTION )
{
}

bool SkipInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
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
	return obj->getValueID() == classid();
}


LetInstruction::LetInstruction( Value* ident, Value* expr )
: BinaryInstruction( ".let", 0, ident, expr, Value::LET_INSTRUCTION )
{
	assert( expr->getType() );
	setType( expr->getType() );
}

bool LetInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}




LookupInstruction::LookupInstruction( Value* location )
: UnaryInstruction( ".lookup", 0, location, Value::LOOKUP_INSTRUCTION )
{
	assert( location->getType() );
	setType( location->getType()->getResultType() );
}

bool LookupInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
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
	return obj->getValueID() == classid();
}


CallInstruction::CallInstruction( Value* symbol )
: Instruction( ".call", 0, Value::CALL_INSTRUCTION )
{
	add( symbol );
	
	if( Value::isa< Builtin >( symbol )
	 or Value::isa< Derived >( symbol )
	)
	{
		assert( symbol->getType() );
		setType( symbol->getType()->getResultType() );
	}
}

bool CallInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}



PrintInstruction::PrintInstruction( Value* channel )
: Instruction( ".print", &StringType, Value::PRINT_INSTRUCTION )
{
	if( channel )
	{
		assert( 0 && "debug channel not implemented yet!" );
	}
}

bool PrintInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}



AssertInstruction::AssertInstruction( Value* condition )
: UnaryInstruction( ".assert", 0, condition, Value::ASSERT_INSTRUCTION )
{
	assert( condition->getType() );
	setType( condition->getType()->getResultType() );
}

bool AssertInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}



SwitchInstruction::SwitchInstruction( Value* expression )
: Instruction( ".switch", 0, Value::SWITCH_INSTRUCTION )
{
	add( expression );
	
	assert( expression->getType() );
	setType( expression->getType()->getResultType() );
}

bool SwitchInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}




OperatorInstruction::OperatorInstruction
( const char* name, Type* type, Value* lhs, Value* rhs, Value::ID id )
: BinaryInstruction( name, type, lhs, rhs, id )
{
	assert( getLHS()->getType() );
	assert( getRHS()->getType() );

	Type* lhs_ty = getLHS()->getType()->getResultType();
	Type* rhs_ty = getRHS()->getType()->getResultType();
	
	assert( lhs_ty->getID() == rhs_ty->getID() );
	
	if( !getType() )
	{
	    // dynamic type assignment, left and right is the same type
		Type* op_ty = 0;
		
		switch( id )
		{
			case Value::ID::ADD_INSTRUCTION:
			case Value::ID::SUB_INSTRUCTION:
			case Value::ID::MUL_INSTRUCTION:
			case Value::ID::DIV_INSTRUCTION:
			case Value::ID::RIV_INSTRUCTION:
			case Value::ID::MOD_INSTRUCTION:
			{
				op_ty = lhs_ty;
				break;
			}
			case Value::ID::EQU_INSTRUCTION:
			case Value::ID::NEQ_INSTRUCTION:
			case Value::ID::LTH_INSTRUCTION:
			case Value::ID::LEQ_INSTRUCTION:
			case Value::ID::GTH_INSTRUCTION:
			case Value::ID::GEQ_INSTRUCTION:
			{
				break;
			}
			case Value::ID::OR_INSTRUCTION:
			case Value::ID::XOR_INSTRUCTION:
			case Value::ID::AND_INSTRUCTION:
			case Value::ID::NOT_INSTRUCTION:
			{
				if( lhs_ty->getIDKind() == Type::ID::BOOLEAN
				 or lhs_ty->getIDKind() == Type::ID::BIT
					)
				{
					op_ty = lhs_ty;
				}
			}
		    default: break;
	    }
		assert( op_ty && " unimplemented case! " );
				
		setType( op_ty );
	}
}

bool OperatorInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid()
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
	return obj->getValueID() == classid();
}

SubInstruction::SubInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".sub", 0, lhs, rhs, Value::SUB_INSTRUCTION )
{	
}
bool SubInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

MulInstruction::MulInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".mul", 0, lhs, rhs, Value::MUL_INSTRUCTION )
{	
}
bool MulInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

DivInstruction::DivInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".div", 0, lhs, rhs, Value::DIV_INSTRUCTION )
{	
}
bool DivInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

RivInstruction::RivInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".riv", 0, lhs, rhs, Value::RIV_INSTRUCTION )
{	
}
bool RivInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

ModInstruction::ModInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".mod", 0, lhs, rhs, Value::MOD_INSTRUCTION )
{	
}
bool ModInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}



EquInstruction::EquInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".equ", &BooleanType, lhs, rhs, Value::EQU_INSTRUCTION )
{	
}
bool EquInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

NeqInstruction::NeqInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".neq", &BooleanType, lhs, rhs, Value::NEQ_INSTRUCTION )
{	
}
bool NeqInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

LthInstruction::LthInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".lth", &BooleanType, lhs, rhs, Value::LTH_INSTRUCTION )
{	
}
bool LthInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

LeqInstruction::LeqInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".leq", &BooleanType, lhs, rhs, Value::LEQ_INSTRUCTION )
{	
}
bool LeqInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

GthInstruction::GthInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".gth", &BooleanType, lhs, rhs, Value::GTH_INSTRUCTION )
{	
}
bool GthInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

GeqInstruction::GeqInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".geq", &BooleanType, lhs, rhs, Value::GEQ_INSTRUCTION )
{	
}
bool GeqInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

OrInstruction::OrInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".or", 0, lhs, rhs, Value::OR_INSTRUCTION )
{
}
bool OrInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

XorInstruction::XorInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".xor", 0, lhs, rhs, Value::XOR_INSTRUCTION )
{	
}
bool XorInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

AndInstruction::AndInstruction( Value* lhs, Value* rhs )
: OperatorInstruction( ".and", 0, lhs, rhs, Value::AND_INSTRUCTION )
{	
}
bool AndInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}


NotInstruction::NotInstruction( Value* lhs )
: UnaryInstruction( ".not", 0, lhs, Value::NOT_INSTRUCTION )
{
	Type* ty = get()->getType();
	assert( get()->getType() );

	ty = ty->getResultType();
	
    if( ty->getIDKind() == Type::ID::BOOLEAN
	 or ty->getIDKind() == Type::ID::BIT
	  )
	{
		setType( ty );
	}
	else
	{
		assert( !" invalid type case for NOT instruction " );
	}
}
bool NotInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

MovInstruction::MovInstruction( Value* lhs )
: UnaryInstruction( ".mov", 0, lhs, Value::MOV_INSTRUCTION )
{	
	assert( get()->getType() );
	setType( get()->getType() );
}
bool MovInstruction::classof( Value const* obj )
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

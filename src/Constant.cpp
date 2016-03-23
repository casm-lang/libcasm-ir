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

#include "Constant.h"

using namespace libcasm_ir;


template< typename V >
Constant< V >::Constant( const char* name, Type* type, V value, u1 defined, Value::ID id )
: User( name, type, id )
, value( value )
, defined( defined )
{
	(*Value::getSymbols())[ ".constant" ].insert( this );
}

template< typename V >
Constant< V >::~Constant( void )
{
	(*Value::getSymbols())[ ".constant" ].erase( this );	
}

template< typename T >
bool Constant< T >::classof( Value const* obj )
{
	assert( 0 && "invalid to check 'classof' Constant< V >, use ConstantValue" );
	return false;
}



template< typename V >
const V Constant< V >::getValue( void ) const
{
	return value;
}

template< typename V >
void Constant< V >::setValue( V val )
{
	value = val;
}


bool ConstantValue::classof( Value const* obj )
{
	return obj->getValueID() == Value::CONSTANT
	    or AgentConstant::classof( obj )
		or RulePointerConstant::classof( obj )
		or BooleanConstant::classof( obj )
		or IntegerConstant::classof( obj )
		or BitConstant::classof( obj )
		or StringConstant::classof( obj )
		or Identifier::classof( obj )
		;
}


AgentConstant::AgentConstant( Type::Agent value, u1 defined )
: Constant< Type::Agent >( ".agent", &AgentType, value, defined, Value::AGENT_CONSTANT )
{
}

AgentConstant* AgentConstant::create( Type::Agent value )
{
	static std::unordered_map< Type::Agent, AgentConstant* > cache;
	
	auto result = cache.find( value );
	if( result != cache.end() )
	{
		assert( result->second );
		printf( "[Const] found %p\n", result->second );
		return result->second;
	}
	
	AgentConstant* obj = new AgentConstant( value, true );
	cache[ value ] = obj;
	return obj;
}

AgentConstant* AgentConstant::create( void )
{
	static AgentConstant* cache = new AgentConstant( 0, false );
	return cache;
}

void AgentConstant::dump( void ) const
{
	printf( "[Const] %p = agent %p\n", this, getValue() );
}

bool AgentConstant::classof( Value const* obj )
{
	return obj->getValueID() == Value::AGENT_CONSTANT;
}



BooleanConstant::BooleanConstant( Type::Boolean value, u1 defined )
: Constant< Type::Boolean >( ".boolean", &BooleanType, value, defined, Value::BOOLEAN_CONSTANT )
{
}


BooleanConstant* BooleanConstant::create( Type::Boolean value )
{
	static BooleanConstant* cache[ 2 ] =
	{ new BooleanConstant( false, true )
    , new BooleanConstant( true,  true )
	};

	return cache[ value ];
}

BooleanConstant* BooleanConstant::create( void )
{
	static BooleanConstant* cache = new BooleanConstant( 0, false );
	return cache;
}

void BooleanConstant::dump( void ) const
{
	printf( "[Const] %p = bool %u\n", this, getValue() );
}

bool BooleanConstant::classof( Value const* obj )
{
	return obj->getValueID() == Value::BOOLEAN_CONSTANT;
}



IntegerConstant::IntegerConstant( Type::Integer value, u1 defined )
: Constant< Type::Integer >( ".integer", &IntegerType, value, defined, Value::INTEGER_CONSTANT )
{
}

IntegerConstant* IntegerConstant::create( Type::Integer value )
{
	static std::unordered_map< Type::Integer, IntegerConstant* > cache;
	
	auto result = cache.find( value );
	if( result != cache.end() )
	{
		assert( result->second );
		printf( "[Const] found %p\n", result->second );
		return result->second;
	}
	
	IntegerConstant* obj = new IntegerConstant( value, true );
	cache[ value ] = obj;
	return obj;
}

IntegerConstant* IntegerConstant::create( void )
{
	static IntegerConstant* cache = new IntegerConstant( 0, false );
	return cache;
}

void IntegerConstant::dump( void ) const
{
	printf( "[Const] %p = integer %li\n", this, getValue() );
}

bool IntegerConstant::classof( Value const* obj )
{
	return obj->getValueID() == Value::INTEGER_CONSTANT;
}



BitConstant::BitConstant( Type* type, u64 value, u1 defined )
: Constant< Type::Bit >( ".bit", type, this->value, defined, Value::BIT_CONSTANT )
{
	this->value[0] = value;
}

BitConstant* BitConstant::create( u64 value, u16 bitsize )
{
	static std::unordered_map< u64, BitConstant* > cache[256];
	
	assert( bitsize > 0 and bitsize <= 256 and "invalid 'Bit' constant bit size" );
	
	auto result = cache[bitsize].find( value );
	if( result != cache[bitsize].end() )
	{
		assert( result->second );
		printf( "[Const] found %p\n", result->second );
		return result->second;
	}
	
	BitConstant* obj = new BitConstant( new Type( Type::ID::BIT, bitsize ), value, true );
	cache[bitsize][ value ] = obj;
	return obj;
}

BitConstant* BitConstant::create( u16 bitsize )
{
	static BitConstant* cache = new BitConstant( new Type( Type::ID::BIT, bitsize ), 0, false );
	return cache;
}

void BitConstant::dump( void ) const
{
	printf( "[Const] %p = bit %li\n", this, getValue()[0] );
}

bool BitConstant::classof( Value const* obj )
{
	return obj->getValueID() == Value::BIT_CONSTANT;
}



StringConstant::StringConstant( Type::String value, u1 defined )
: Constant< Type::String >( ".string", &StringType, value, defined, Value::STRING_CONSTANT )
{
}

StringConstant* StringConstant::create( Type::String value )
{
	static std::unordered_map< Type::String, StringConstant* > cache;
	
	auto result = cache.find( value );
	if( result != cache.end() )
	{
		assert( result->second );
		printf( "[Const] found %p\n", result->second );
		return result->second;
	}
	
	StringConstant* obj = new StringConstant( value, true );
	cache[ value ] = obj;
	return obj;
}

StringConstant* StringConstant::create( const char* value )
{
	return create( (char*)value );
}

StringConstant* StringConstant::create( void )
{
	static StringConstant* cache = new StringConstant( 0, false );
	return cache;
}

void StringConstant::dump( void ) const
{
	printf( "[Const] %p = string %s\n", this, getValue() );
}

bool StringConstant::classof( Value const* obj )
{
	return obj->getValueID() == Value::STRING_CONSTANT;
}





RulePointerConstant::RulePointerConstant( Type::RulePointer value, u1 defined )
: Constant< Type::RulePointer >( ".rulepointer", &RulePointerType, value, defined, Value::RULE_POINTER_CONSTANT )
, resolve_identifier( 0 )
{
}

RulePointerConstant* RulePointerConstant::create( Type::RulePointer value )
{
	static std::unordered_map< Type::RulePointer, RulePointerConstant* > cache;
	
	auto result = cache.find( value );
	if( result != cache.end() )
	{
		assert( result->second );
		printf( "[Const] found %p\n", result->second );
		return result->second;
	}
	
	RulePointerConstant* obj = new RulePointerConstant( value, true );
	cache[ value ] = obj;
	return obj;
}

// use this ruleconstpointer for not finished resolved rule*'s
RulePointerConstant* RulePointerConstant::create( const char* name )
{
	static std::unordered_map< const char*, RulePointerConstant* > cache;
	
	auto rsym = (*Value::getSymbols()).find( name );
	if( rsym != (*Value::getSymbols()).end() )
	{
		return create( ((Rule*)(*rsym->second.begin())) );
	}
	
	auto result = cache.find( name );
	if( result != cache.end() )
	{
		assert( result->second );
		printf( "[Const] found %p\n", result->second );
		return result->second;
	}
	
	RulePointerConstant* obj = new RulePointerConstant( 0, true );
	assert( obj );
	obj->setResolveIdentifier( name );
	cache[ name ] = obj;
	return obj;
}

RulePointerConstant* RulePointerConstant::create( void )
{
	static RulePointerConstant* cache = new RulePointerConstant( 0, false );
	return cache;
}

void RulePointerConstant::setResolveIdentifier( const char* name  )
{
    resolve_identifier = name;
}

void RulePointerConstant::resolve( void )
{
	if( !resolve_identifier )
	{
		return;
	}
	
	auto result = (*Value::getSymbols()).find( resolve_identifier );
	if( result != (*Value::getSymbols()).end() )
	{
		assert( result->second.size() == 1 );
		Value* val = *(result->second.begin());
		assert( Value::isa< Rule >( val ) );
		setValue( (Rule*)val );
	}
}

void RulePointerConstant::dump( void ) const
{
	printf( "[Const] %p = rule %p\n", this, getValue() );
}

bool RulePointerConstant::classof( Value const* obj )
{
	return obj->getValueID() == Value::RULE_POINTER_CONSTANT;
}




Identifier::Identifier( Type* type, const char* value )
: Constant< const char* >( value, type, value, true, Value::IDENTIFIER )
{
    (*Value::getSymbols())[ ".identifier" ].insert( this );
}

Identifier::~Identifier( void )
{
	(*Value::getSymbols())[ ".identifier" ].erase( this );
}

Identifier* Identifier::create( Type* type, const char* value, Value* scope )
{
	SymbolTable& symbols = *getSymbols();

	const char* tmp = value;
	
	// if( scope )
	// {
	// 	tmp = libstdhl::Allocator::string( string(scope->getName()) + "::" + string(tmp) );
	// 	// std::string tmp;
	// 	// tmp.append( scope->getName() );
	// 	// tmp.append( "::" );
	// 	// tmp.append( value );
	// 	// tmp_scope = tmp.c_str();
	// }
	
	auto result = symbols.find( tmp );
	if( result != symbols.end() )
	{
		assert( result->second.size() == 1 );
		Value* x = *result->second.begin();
		
		assert( x->getType()->getID() == type->getID() );
		//if( x->getType()->getID() == type->getID() )
		//{
		printf( "[Ident] found '%s' of type %lu @ %p\n", value, type->getID(), x );
		return (Identifier*)x;
		//}
	}
	
	printf( "[Ident] creating '%s' of type %lu\n", value, type->getID() );
	return new Identifier( type, value );
}

// Identifier* Identifier::create( Type* type )
// {
// 	static std::unordered_map< u64, Identifier* > cache;
// 	auto result = cache.find( type->getID() );
// 	Identifier* x = 0;
	
// 	if( result != cache.end() )
// 	{
// 		x = result->second;
// 	 	assert( x->getType()->getID() == type->getID() );
// 		printf( "[Ident] found 'undef' of type %lu @ %p\n", type->getID(), x );
// 		return x;
// 	}
	
// 	x = new Identifier( type, 0, false );
	
// 	printf( "[Ident] creating '%s' of type %lu\n", value, type->getID() );
// 	return 
// }

void Identifier::forgetSymbol( const char* value )
{
	printf( "[Ident] forgetting '%s'\n", value );
	getSymbols()->erase( value );
}

void Identifier::dump( void ) const
{
	printf( "[Ident] " );
	debug();
}

bool Identifier::classof( Value const* obj )
{
	return obj->getValueID() == Value::IDENTIFIER;
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

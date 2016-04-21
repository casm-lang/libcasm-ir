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

#ifndef _LIB_CASMIR_CONSTANT_H_
#define _LIB_CASMIR_CONSTANT_H_

#include "User.h"

namespace libcasm_ir
{
	class Statement;

	template< typename V >
	class Constant : public User
	{
	private:
		V value;
		u1 defined;
		
    protected:
		Constant( const char* name, Type* type, V value, u1 defined, Value::ID id = Value::CONSTANT );

	public:
		~Constant( void );
		
		const V getValue( void ) const;
		
		const u1 isDefined( void ) const
		{
			return defined;
		}
		
		const u1 isUndef( void ) const
		{
			return not defined;
		}
		
		static inline Value::ID classid( void ) { return Value::CONSTANT; };
		static bool classof( Value const* obj );

	protected:
	    void setValue( V val );
	};
	
	class ConstantValue : public Constant< Type::Undef >
	{
	public:
		static inline Value::ID classid( void ) { return Value::CONSTANT; };
		static bool classof( Value const* obj );
	};
	
	
	class AgentConstant : public Constant< Type::Agent >
	{
	private:
		AgentConstant( Type::Agent value, u1 defined );

	public:
		static AgentConstant* create( Type::Agent value );
		static AgentConstant* create( void );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::AGENT_CONSTANT; };
		static bool classof( Value const* obj );
	};
	
	
	class RulePointerConstant : public Constant< Type::RulePointer >
	{
	private:
		const char* resolve_identifier;
		
		RulePointerConstant( Type::RulePointer value, u1 defined );
	    
	public:
		static RulePointerConstant* create( Type::RulePointer value );
		static RulePointerConstant* create( const char* name );
		static RulePointerConstant* create( void );

		void setResolveIdentifier( const char* name );
		void resolve( void );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::RULE_POINTER_CONSTANT; };
		static bool classof( Value const* obj );
	};
	
	
	class BooleanConstant : public Constant< Type::Boolean >
	{
	private:
		BooleanConstant( Type::Boolean value, u1 defined );
		
	public:
		static BooleanConstant* create( Type::Boolean value );
		static BooleanConstant* create( void );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::BOOLEAN_CONSTANT; };
		static bool classof( Value const* obj );
	};

	class IntegerConstant : public Constant< Type::Integer >
	{
	private:
		IntegerConstant( Type::Integer value, u1 defined );

	public:
		static IntegerConstant* create( Type::Integer value );
		static IntegerConstant* create( void );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::INTEGER_CONSTANT; };
		static bool classof( Value const* obj );
	};
	
	class BitConstant : public Constant< Type::Bit >
	{
	private:
		u64 value[1];
		BitConstant( Type* type, u64 value, u1 defined );

	public:
		static BitConstant* create( u64 value, u16 bitsize );
		static BitConstant* create( u16 bitsize );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::BIT_CONSTANT; };
		static bool classof( Value const* obj );
	};
	
	class StringConstant : public Constant< Type::String >
	{
	private:
		StringConstant( Type::String value, u1 defined );

	public:
		static StringConstant* create( Type::String value );
		static StringConstant* create( const char* value );
		static StringConstant* create( void );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::STRING_CONSTANT; };
		static bool classof( Value const* obj );
	};
	
	class Identifier : public Constant< const char* >
	{
	private:
		Identifier( Type* type, const char* value );

	public:
	    ~Identifier( void );

		static Identifier* create( Type* type, const char* value, Value* scope = 0 );
		
		static void forgetSymbol( const char* value );
		
		void dump( void ) const;

		static inline Value::ID classid( void ) { return Value::IDENTIFIER; };
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_CASMIR_CONSTANT_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

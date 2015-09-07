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

#ifndef _LIB_CASMIR_CONSTANT_H_
#define _LIB_CASMIR_CONSTANT_H_

#include "libcasm-ir.h"

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
		
		static bool classof( Value const* obj );
	};
	
	class ConstantValue : public Constant< Type::Undef >
	{
	public:
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
		
		static bool classof( Value const* obj );
	};
	
	
	class RulePointerConstant : public Constant< Type::RulePointer >
	{
	private:
		RulePointerConstant( Type::RulePointer value, u1 defined );

	public:
		static RulePointerConstant* create( Type::RulePointer value );
		static RulePointerConstant* create( const char* name );
		static RulePointerConstant* create( void );
		
		void dump( void ) const;
		
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
		
		static bool classof( Value const* obj );
	};
	
	class Identifier : public Constant< const char* >
	{
	private:
		Identifier( Type* type, const char* value );

	public:
	    ~Identifier( void );

		static Identifier* create( Type* type, const char* value );

		static void forgetSymbol( const char* value );
		
		void dump( void ) const;

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

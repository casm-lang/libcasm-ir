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

#ifndef _LIB_CASMIR_INSTRUCTION_H_
#define _LIB_CASMIR_INSTRUCTION_H_

#include "User.h"
#include "Statement.h"

namespace libcasm_ir
{
	class Statement;
	
	class Instruction : public User
	{
	private:
		Statement* statement;
		std::vector< Value* > values;
	    
	public:
		Instruction( const char* name, Type* type, Value::ID id = Value::INSTRUCTION );
		void setStatement( Statement* stmt );
		
	    void add( Value* value );
		Value* getValue( u8 index ) const;
		
		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	class UnaryInstruction : public Instruction
	{
	private:
		Value* value;
	
	public:
		UnaryInstruction( const char* name, Type* type, Value* value
						, Value::ID id = Value::UNARY_INSTRUCTION );
		Value* get( void ) const;

		
		static bool classof( Value const* obj );
	};
	
	class BinaryInstruction : public Instruction
	{
	private:
		Value* lhs;
		Value* rhs;
		
	public:
		BinaryInstruction( const char* name, Type* type, Value* lhs, Value* rhs
						 , Value::ID id = Value::BINARY_INSTRUCTION );
		
		Value* getLHS( void ) const;
		Value* getRHS( void ) const;
		
		
		static bool classof( Value const* obj );
	};
	
	
	
	
	
	class LookupInstruction : public UnaryInstruction
	{
	public :
		LookupInstruction( Value* location );

		//void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	
	class UpdateInstruction : public BinaryInstruction
	{
	public:
		UpdateInstruction( Value* func, Value* expr );

		//void dump( void ) const;
	    
		static bool classof( Value const* obj );
	};

	
	class LetInstruction : public BinaryInstruction
	{
	public:
	    LetInstruction( Value* lhs, Value* rhs );
        
		static bool classof( Value const* obj );
	};
	
	class LocationInstruction : public Instruction
	{
	public:
		LocationInstruction( Value* function );

		//void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	class CallInstruction : public Instruction
	{
	public:
		CallInstruction( Value* symbol );
		
		//void dump( void ) const;
		
		static bool classof( Value const* obj );
	};

	class PrintInstruction : public Instruction
	{
	public:
		PrintInstruction( Value* channel = 0 );
		static bool classof( Value const* obj );
	};


	
	class OperatorInstruction : public BinaryInstruction
	{
	public:
		OperatorInstruction( const char* name, Type* type, Value* lhs, Value* rhs
						   , Value::ID id = Value::OPERATOR_INSTRUCTION );
		
		//void dump( void ) const;
	    
		static bool classof( Value const* obj );
	};

	class AddInstruction : public OperatorInstruction
	{
	public:
		AddInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class SubInstruction : public OperatorInstruction
	{
	public:
		SubInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class MulInstruction : public OperatorInstruction
	{
	public:
		MulInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class DivInstruction : public OperatorInstruction
	{
	public:
		DivInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class RivInstruction : public OperatorInstruction
	{
	public:
		RivInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class ModInstruction : public OperatorInstruction
	{
	public:
		ModInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class EquInstruction : public OperatorInstruction
	{
	public:
		EquInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class NeqInstruction : public OperatorInstruction
	{
	public:
		NeqInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class LesInstruction : public OperatorInstruction
	{
	public:
		LesInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class LeqInstruction : public OperatorInstruction
	{
	public:
		LeqInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class GreInstruction : public OperatorInstruction
	{
	public:
		GreInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class GeqInstruction : public OperatorInstruction
	{
	public:
		GeqInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class OrInstruction : public OperatorInstruction
	{
	public:
		OrInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class XorInstruction : public OperatorInstruction
	{
	public:
		XorInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class AndInstruction : public OperatorInstruction
	{
	public:
		AndInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};
	
	class NotInstruction : public UnaryInstruction
	{
	public:
		NotInstruction( Value* lhs );
		static bool classof( Value const* obj );
	};
	
}


#endif /* _LIB_CASMIR_INSTRUCTION_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

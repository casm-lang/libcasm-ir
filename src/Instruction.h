//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
//  
//  This software is licensed under the Open Software License v. 3.0 (OSL-3.0).
//  You either find the license text below or in the root directory of this 
//  project in the LICENSE.txt file. If you cannot find the LICENSE.txt file,
//  please refer to https://opensource.org/licenses/OSL-3.0.
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
#include "Builtin.h"
#include "Constant.h"
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
		const Statement* getStatement( void ) const;
		
	    void add( Value* value );
		Value* getValue( u8 index ) const;
		const std::vector< Value* >& getValues( void ) const;
		
		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	class UnaryInstruction : public Instruction
	{
	public:
		UnaryInstruction( const char* name, Type* type, Value* value
						, Value::ID id = Value::UNARY_INSTRUCTION );
		Value* get( void ) const;

		
		static bool classof( Value const* obj );
	};
	
	class BinaryInstruction : public Instruction
	{
	public:
		BinaryInstruction( const char* name, Type* type, Value* lhs, Value* rhs
						 , Value::ID id = Value::BINARY_INSTRUCTION );
		
		Value* getLHS( void ) const;
		Value* getRHS( void ) const;
				
		static bool classof( Value const* obj );
	};
	
	
	
	class SkipInstruction : public Instruction
	{
	public:
		SkipInstruction( void );
		
		//void dump( void ) const;
		
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
		UpdateInstruction( Value* location, Value* expr );

		//void dump( void ) const;
	    
		static bool classof( Value const* obj );
	};
	
	
	class LetInstruction : public BinaryInstruction
	{
	public:
	    LetInstruction( Value* ident, Value* expr );
        
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


	class AssertInstruction : public UnaryInstruction
	{
	public :
		AssertInstruction( Value* condition );

		//void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	
	class SwitchInstruction : public Instruction
	{
	public:
		SwitchInstruction( Value* expression );
		
		//void dump( void ) const;
		
		static bool classof( Value const* obj );
	};	
	
	class BranchInstruction : public UnaryInstruction
	{
	private:
		Value* case_true;
	    Value* case_false;
		
	public :
	    BranchInstruction( Value* condition, Value* case_true, Value* case_false = 0 );
		
		static bool classof( Value const* obj );

		Value* getTrue ( void ) const;
		Value* getFalse( void ) const;
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

	class LthInstruction : public OperatorInstruction
	{
	public:
		LthInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class LeqInstruction : public OperatorInstruction
	{
	public:
		LeqInstruction( Value* lhs, Value* rhs );
		static bool classof( Value const* obj );
	};

	class GthInstruction : public OperatorInstruction
	{
	public:
		GthInstruction( Value* lhs, Value* rhs );
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
	
	class MovInstruction : public UnaryInstruction
	{
	public:
		MovInstruction( Value* lhs );
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

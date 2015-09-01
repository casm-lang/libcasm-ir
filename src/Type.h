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

/**
   @brief    TODO
   
   TODO
*/

#ifndef _LIB_CASMIR_TYPE_H_
#define _LIB_CASMIR_TYPE_H_

#include "stdhl/cpp/Type.h"


namespace libcasm_ir
{
	class Rule;
	
	class Type // : public Value
	{
	public:		
		typedef u1     Undef;
		typedef u1     Boolean;
		typedef i64    Integer;
		typedef Rule*  RulePointer;
		typedef void*  Agent;
		
		enum ID
		{ UNDEF = 0
		, AGENT
		, BOOLEAN
		, INTEGER
		, RULE_POINTER
		
		  // , STRING
		  // , FLOAT
		  // , BIT

		  // , TUPLE
		  // , LIST
		  
		// , DERIVED
		// , FUNCTION
		, _TOP_
		};
		
		enum STATE
		{ UNCHANGED
		, CHANGED
		, LOCKED
		};
		
	private:		
		ID type_id;
		u64 type_uid_hash;
		STATE type_state;
		std::string description;
		std::vector< Type* > parameters;
		std::vector< Type* > subtypes;
		
		static const char* ID2str[ ID::_TOP_ ];

	public:
		Type( ID id, STATE state = STATE::UNCHANGED );
		const u64 getID( void ) const;
		const char* getName( void );
		
		const std::vector< Type* >& getParameters( void ) const;
	    const std::vector< Type* >& getSubTypes( void ) const;
	    
		void addParameter( Type* parameter );
		void addSubType( Type* subtype );
		
		Type* getResultType( void );
		
	private:
		void setID( ID id );	
	};
	
	static Type UndefType       = Type( Type::UNDEF,        Type::STATE::LOCKED );
	static Type AgentType       = Type( Type::AGENT,        Type::STATE::LOCKED );
	static Type BooleanType     = Type( Type::BOOLEAN,      Type::STATE::LOCKED );
	static Type IntegerType     = Type( Type::INTEGER,      Type::STATE::LOCKED );
	static Type RulePointerType = Type( Type::RULE_POINTER, Type::STATE::LOCKED );
	
	// static Type DerivedType  = Type( Type::DERIVED,  "Derived" );
	// static Type FunctionType = Type( Type::FUNCTION, "Function");
}

#endif /* _LIB_CASMIR_TYPE_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

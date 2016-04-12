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
   @brief    TODO
   
   TODO
*/

#ifndef _LIB_CASMIR_TYPE_H_
#define _LIB_CASMIR_TYPE_H_

#include "stdhl/cpp/Type.h"

#include "CasmIR.h"

namespace libcasm_ir
{
	class Rule;
	
	class Type : public CasmIR
	{
	public:		
		typedef u1     Undef; // PPA: this is obsolete ??? FIXME: TODO:
		typedef void*  Agent;
		typedef Rule*  RulePointer;
		typedef u1     Boolean;
		typedef i64    Integer;
		typedef u64*   Bit;
		typedef char*  String;
		
		enum ID
		{ UNDEF = 0
		, AGENT
		, RULE_POINTER
		, BOOLEAN
		, INTEGER
		, BIT
		, STRING
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
		i16 bitsize;
		std::string description;
		std::vector< Type* > parameters;
		std::vector< Type* > subtypes;
		
		static const char* ID2str[ ID::_TOP_ ];
		
	public:
		Type( ID id, i16 bitsize = -1, STATE state = STATE::UNCHANGED );
		const ID getIDKind( void ) const;
		const u64 getID( void ) const;
		const char* getName( void );
		const i16 getBitsize( void );

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
	static Type RulePointerType = Type( Type::RULE_POINTER, Type::STATE::LOCKED );
	static Type BooleanType     = Type( Type::BOOLEAN,      Type::STATE::LOCKED );
	static Type IntegerType     = Type( Type::INTEGER,      Type::STATE::LOCKED );
	static Type StringType      = Type( Type::STRING,       Type::STATE::LOCKED );	
	
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

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

#ifndef _LIB_CASMIR_STATEMENT_H_
#define _LIB_CASMIR_STATEMENT_H_

#include "Block.h"

namespace libcasm_ir
{
	class Statement : public Block
	{
    protected:
		ExecutionSemanticsBlock* scope;
		
	private:
		std::vector< Value* > instructions;
		
	public:
		Statement( const char* name, Type* type, ExecutionSemanticsBlock* scope
				 , Value::ID id = Value::STATEMENT );
	    
		ExecutionSemanticsBlock* getScope( void ) const;

		const std::vector< Value* >& getInstructions( void ) const;
		
		void add( Value* instruction );

		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::STATEMENT; };
		static bool classof( Value const* obj );
	};
	
	class TrivialStatement : public Statement
	{
	public:
		TrivialStatement( ExecutionSemanticsBlock* scope = 0 );

		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::TRIVIAL_STATEMENT; };
		static bool classof( Value const* obj );
	};


	
	class BranchStatement : public Statement
	{
	private:
		std::vector< Block* > blocks;
		
	public:
		BranchStatement( ExecutionSemanticsBlock* scope = 0 );
	    
		void addBlock( Value* block );

		const std::vector< Block* >& getBlocks( void ) const;
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::BRANCH_STATEMENT; };
		static bool classof( Value const* obj );
	};
	
	// TODO: FIXME: PPA: add ForallStatement and IterateStatement etc.
}


#endif /* _LIB_CASMIR_STATEMENT_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

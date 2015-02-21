
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'libcasm-ir' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _LIB_CASMIR_VALUE_H_
#define _LIB_CASMIR_VALUE_H_

#include "Type.h"

/**
   @file     Value.h
   @class    Value
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-20
*/

namespace libcasm_ir
{
	class Value  
	{
	public:
		enum ValueId
		{ RULE_VID = 0
		, SCOPE_VID
		, BASIC_BLOCK_VID
		, STATEMENT_VID
		, INSTRUCTION_VID
		, CONSTANT_INT_VID
		// TODO: PPA: extend here!
		};
	
	private:
		ValueId value_id;
		Type* type;
	
	public:
		Value( ValueId value_id )
		: value_id( value_id )
		{
		}
	
		Type* getType( void ) const
		{
			return type;
		}
		
		u8 getValueId( void ) const
		{
			return value_id;
		}
		
		void dump() const
		{
			// GDB dbg function
		}
		
		/**
		   @brief    TODO
		   
		   TODO
		   
		   @param    arg0    TODO
		   @return   TODO
		   @retval   TODO
		*/
	};	
}

#endif /* _LIB_CASMIR_VALUE_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/

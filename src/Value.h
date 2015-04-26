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


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

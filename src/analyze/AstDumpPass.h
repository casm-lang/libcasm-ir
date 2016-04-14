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
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  

#ifndef _LIB_CASMIR_ASTDUMPPASS_H_
#define _LIB_CASMIR_ASTDUMPPASS_H_

#include "Pass.h"
#include "libcasm-ir.h"

#include "AstInterface.h"
#include "TypeCheckPass.h"

/**
   @brief    TODO
   
   TODO
*/

namespace libcasm_ir
{
	class AstDumpPass : public libpass::Pass, AstDumpVisitor
	{
	public:
		static char id;
		
		virtual bool run( libpass::PassResult& pr );
	};
}
	
#endif /* _LIB_CASMIR_ASTDUMPPASS_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

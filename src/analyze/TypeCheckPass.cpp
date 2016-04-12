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

#include "TypeCheckPass.h"

using namespace libcasm_ir;


char TypeCheckPass::id = 0;

static libpass::PassRegistration< TypeCheckPass > PASS
( "Type Check Pass"
, "type check the AST and translate it to a typed AST"
, 0
, 'T'
);


bool TypeCheckPass::run( libpass::PassResult& pr )
{
	AstNode* node = (AstNode*)pr.getResult< SourceToAstPass >();
	
	if( !casm_frontend_pass_2_typecheck( node ) )
	{
		// TODO: PPA: better error message!
		std::cerr << "Error typecheck file" << std::endl;
		
		return false;
	}

	pr.setResult< TypeCheckPass >( node );
	pr.setResult< AstDumpPass >( node );
    
	return true;
}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

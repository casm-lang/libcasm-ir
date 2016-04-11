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

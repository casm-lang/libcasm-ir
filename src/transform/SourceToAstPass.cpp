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

#include "SourceToAstPass.h"

using namespace libcasm_ir;


char SourceToAstPass::id = 0;

static libpass::PassRegistration< SourceToAstPass > PASS
( "Source To AST Pass"
, "parse the source code and generate an AST"
, 0
, 'P'
);


bool SourceToAstPass::run( libpass::PassResult& pr )
{
	const char* file_name = (const char*)pr.getResults()[ 0 ];
	
	casm_frontend_init();
	
	AstNode* node = casm_frontend_pass_1_parse( file_name );
	
    if( !node ) 
	{
		// TODO: PPA: better error messages!
        std::cerr << "Error parsing file" << std::endl;
		
		return false;
    }
	
	pr.setResult< SourceToAstPass >( node );
    
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

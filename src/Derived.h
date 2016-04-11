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

#ifndef _LIB_CASMIR_DERIVED_H_
#define _LIB_CASMIR_DERIVED_H_

#include "User.h"
#include "Constant.h"
#include "Statement.h"

namespace libcasm_ir
{
	class Identifier;
	
	class Derived : public User
	{
	private:
		Identifier* ident;
	    TrivialStatement* context;
		std::vector< Value* > parameter;
		
	public:
		Derived( const char* name, Type* result );
		
		~Derived( void );
		
	    TrivialStatement* getContext( void ) const;
		
		void setContext( TrivialStatement* scope );

		void addParameter( Value* value );

		const std::vector< Value* >& getParameters( void ) const;
		
		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_CASMIR_DERIVED_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

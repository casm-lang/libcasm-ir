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

#include "Statement.h"

#include "Constant.h"
#include "Instruction.h"

using namespace libcasm_ir;


Statement::Statement( const char* name, Type* type, ExecutionSemanticsBlock* scope, Value::ID id )
: Block( name, type, id )
, scope( scope )
{
	//assert( scope );
	if( !scope ) return;
	
	scope->add( this );
	
	printf( "[Statement] '%s' at %lu\n", name, scope->getPseudoState() );
}

ExecutionSemanticsBlock* Statement::getScope( void ) const
{
	return scope;
}

const std::vector< Value* >& Statement::getInstructions( void ) const
{
	return instructions;
}

void Statement::add( Value* instruction )
{
	printf( "%s: %p\n", __FUNCTION__, instruction );
	assert( instruction );

	if( Value::isa< ConstantValue >( instruction ) )
	{
		printf( "%s: %p --> Constant, omitted\n", __FUNCTION__, instruction );
		return;
	}
	
	if( Value::isa< Instruction >( instruction ) )
	{
		printf( "%s: %p --> Instruction\n", __FUNCTION__, instruction );
	    static_cast< Instruction* >( instruction )->setStatement( this );
	}
	
	instructions.push_back( instruction );
	printf( "[Stmt] add: %p\n", instruction );	
}

void Statement::dump( void ) const
{
	for( auto instr : instructions )
	{
		static_cast< Value* >( instr )->dump();
	}
}

bool Statement::classof( Value const* obj )
{
	return obj->getValueID() == Value::STATEMENT
		or TrivialStatement::classof( obj )
		or BranchStatement::classof( obj );
}





TrivialStatement::TrivialStatement( ExecutionSemanticsBlock* scope )
: Statement( ".statement", 0, scope, Value::TRIVIAL_STATEMENT )
{
}

void TrivialStatement::dump( void ) const
{
	printf( "[TrStm] %p", this );
	if( scope )
	{
		printf( " @ %lu (%p)", scope->getPseudoState(), scope );
	}
	printf( "\n" );
	
	((Statement*)this)->dump();	
}

bool TrivialStatement::classof( Value const* obj )
{
	return obj->getValueID() == Value::TRIVIAL_STATEMENT;
}





BranchStatement::BranchStatement( ExecutionSemanticsBlock* scope )
: Statement( ".branch", 0, scope, Value::BRANCH_STATEMENT )
{
}

void BranchStatement::addBlock( Value* block )
{
	assert( Value::isa< Block >( block ) );
	
	blocks.push_back( (Block*)block );
}

const std::vector< Block* >& BranchStatement::getBlocks( void ) const
{
	return blocks;
}

void BranchStatement::dump( void ) const
{
	printf( "[BranchStatement] %p\n", this );
	
	((Statement*)this)->dump();
	
	// TODO: here the branches etc.
}

bool BranchStatement::classof( Value const* obj )
{
	return obj->getValueID() == Value::BRANCH_STATEMENT;
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

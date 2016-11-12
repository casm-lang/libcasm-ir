//  
//  Copyright (c) 2015-2016 CASM Organization
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcasm-ir
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

#ifndef _LIB_CASMIR_BLOCK_H_
#define _LIB_CASMIR_BLOCK_H_

#include "cpp/Binding.h"

#include "Value.h"

namespace libcasm_ir
{
    class Statement;
    
    class Block : public Value
    {
    private:
        Value* parent;
        
    public:
        Block( const char* name, Type* type, Value::ID id = Value::BLOCK );

        void setParent( Value* parent );
        Value* getParent( void ) const;
        
        void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::BLOCK; };
        static bool classof( Value const* obj );
    };
    
    class ExecutionSemanticsBlock : public Block
    {
    private:
        const u1 is_parallel;
        u64 pseudo_state;
        ExecutionSemanticsBlock* scope;
        std::vector< Block* > blocks;
        
    public:
        ExecutionSemanticsBlock
        ( const char* name
        , Type* type
        , const u1 is_parallel
        , ExecutionSemanticsBlock* scope = 0
        , Value::ID id = Value::EXECUTION_SEMANTICS_BLOCK
        );
        
        const u1 isParallel( void ) const;
        
        const u64 getPseudoState( void ) const;
        
        ExecutionSemanticsBlock* getScope( void ) const;
        
        void setScope( ExecutionSemanticsBlock* scope_block );

        const std::vector< Block* >& getBlocks( void ) const;

        void add( Block* block );
        
        void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::EXECUTION_SEMANTICS_BLOCK; };
        static bool classof( Value const* obj );
    };

    
    class ParallelBlock : public ExecutionSemanticsBlock, public libstdhl::Binding< Rule >
    {
    public:
        ParallelBlock( ExecutionSemanticsBlock* scope = 0 );
        
        void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::PARALLEL_BLOCK; };
        static bool classof( Value const* obj );
    };
    
    class SequentialBlock : public ExecutionSemanticsBlock
    {
    public:
        SequentialBlock( ExecutionSemanticsBlock* scope = 0 );
        
        void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::SEQUENTIAL_BLOCK; };
        static bool classof( Value const* obj );
    };
}


#endif /* _LIB_CASMIR_BASICBLOCK_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

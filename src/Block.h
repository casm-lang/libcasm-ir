//
//  Copyright (c) 2015-2017 CASM Organization
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

#include "Value.h"

#include "../stdhl/cpp/List.h"

namespace libcasm_ir
{
    class Statement;
    class ForkInstruction;
    class MergeInstruction;

    class ExecutionSemanticsBlock;

    class Block : public Value
    {
      public:
        using Ptr = std::shared_ptr< Block >;

        Block( const std::string& name, Value::ID id = classid() );

        void setParent( const Ptr& parent );

        Ptr parent( void ) const;

        void setScope(
            const std::shared_ptr< ExecutionSemanticsBlock >& scope );

        std::shared_ptr< ExecutionSemanticsBlock > scope( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BLOCK;
        }

        static u1 classof( Value const* obj );

      private:
        std::weak_ptr< Block > m_parent;
        std::weak_ptr< ExecutionSemanticsBlock > m_scope;
    };

    using Blocks = libstdhl::List< Block >;

    class ExecutionSemanticsBlock : public Block
    {
      public:
        using Ptr = std::shared_ptr< ExecutionSemanticsBlock >;

        ExecutionSemanticsBlock( const std::string& name, u1 parallel,
            const ExecutionSemanticsBlock::Ptr& scope,
            Value::ID id = classid() );

        ~ExecutionSemanticsBlock( void );

        u1 parallel( void ) const;

        u64 pseudostate( void ) const;

        Blocks blocks( void ) const;

        void add( const Block::Ptr& block );

        static inline Value::ID classid( void )
        {
            return Value::EXECUTION_SEMANTICS_BLOCK;
        }

        static u1 classof( Value const* obj );

      private:
        const u1 m_parallel;
        u64 m_pseudostate;

        std::unique_ptr< Block > m_entry;
        std::unique_ptr< Block > m_exit;

        Blocks m_blocks;
    };

    using ExecutionSemanticsBlocks = libstdhl::List< ExecutionSemanticsBlock >;

    class ParallelBlock : public ExecutionSemanticsBlock
    {
      public:
        ParallelBlock( void );

        static inline Value::ID classid( void )
        {
            return Value::PARALLEL_BLOCK;
        }

        static u1 classof( Value const* obj );
    };

    class SequentialBlock : public ExecutionSemanticsBlock
    {
      public:
        SequentialBlock( void );

        static inline Value::ID classid( void )
        {
            return Value::SEQUENTIAL_BLOCK;
        }

        static u1 classof( Value const* obj );
    };
}

#endif // _LIB_CASMIR_BLOCK_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

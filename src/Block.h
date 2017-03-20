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

namespace libcasm_ir
{
    class Rule;
    class Statement;
    class ForkInstruction;
    class MergeInstruction;
    class ExecutionSemanticsBlock;

    class Block : public Value
    {
      public:
        using Ptr = std::shared_ptr< Block >;

        Block( const std::string& name, Value::ID id = classid() );

        void setParent( const Block::Ptr& parent );

        Block::Ptr parent( void ) const;

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

    using Blocks = ValueList< Block >;

    class ExecutionSemanticsBlock : public Block
    {
      public:
        using Ptr = std::shared_ptr< ExecutionSemanticsBlock >;

        ExecutionSemanticsBlock(
            const std::string& name, u1 parallel, Value::ID id = classid() );

        u1 parallel( void ) const;

        u64 pseudostate( void ) const;

        std::shared_ptr< Statement > entry( void ) const;

        std::shared_ptr< Statement > exit( void ) const;

        Blocks blocks( void ) const;

        void add( const Block::Ptr& block );

        static inline Value::ID classid( void )
        {
            return Value::EXECUTION_SEMANTICS_BLOCK;
        }

        static u1 classof( Value const* obj );

      protected:
        ExecutionSemanticsBlock::Ptr init( void );

      private:
        const u1 m_parallel;
        u64 m_pseudostate;

        std::shared_ptr< Statement > m_entry;
        std::shared_ptr< Statement > m_exit;

        Blocks m_blocks;
    };

    using ExecutionSemanticsBlocks = ValueList< ExecutionSemanticsBlock >;

    class ParallelBlock final : public ExecutionSemanticsBlock
    {
      public:
        using Ptr = std::shared_ptr< ParallelBlock >;

        ParallelBlock( void );

        ParallelBlock::Ptr init( void )
        {
            return std::static_pointer_cast< ParallelBlock >(
                ExecutionSemanticsBlock::init() );
        }

        void setRule( const std::shared_ptr< Rule >& rule );

        /**
           @return rule pointer if this parallel block is the context of a rule,
                   otherwise nullptr
         */
        std::shared_ptr< Rule > rule( void ) const;

        void accept( Visitor& visitor ) override;

        static inline Value::ID classid( void )
        {
            return Value::PARALLEL_BLOCK;
        }

        static u1 classof( Value const* obj );

      private:
        std::weak_ptr< Rule > m_rule;
    };

    class SequentialBlock final : public ExecutionSemanticsBlock
    {
      public:
        using Ptr = std::shared_ptr< SequentialBlock >;

        SequentialBlock( void );

        SequentialBlock::Ptr init( void )
        {
            return std::static_pointer_cast< SequentialBlock >(
                ExecutionSemanticsBlock::init() );
        }

        void accept( Visitor& visitor ) override;

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

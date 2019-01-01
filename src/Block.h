//
//  Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
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
//  Additional permission under GNU GPL version 3 section 7
//
//  libcasm-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcasm-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcasm-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcasm-ir give you permission to link libcasm-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcasm-ir. If you modify libcasm-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#ifndef _LIBCASM_IR_BLOCK_H_
#define _LIBCASM_IR_BLOCK_H_

#include <libcasm-ir/Value>

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

        Block( const Value::ID id = classid() );

        void clear( void );

        void setParent( const Block::Ptr& parent );

        Block::Ptr parent( void ) const;

        void setScope( const std::shared_ptr< ExecutionSemanticsBlock >& scope );

        std::shared_ptr< ExecutionSemanticsBlock > scope( void ) const;

        virtual void replaceWith( const Block::Ptr block ) = 0;

        std::string name( void ) const override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

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

      protected:
        ExecutionSemanticsBlock( const u1 parallel, const Value::ID id = classid() );

      public:
        u1 parallel( void ) const;

        u64 pseudostate( void ) const;

        std::shared_ptr< Statement > entry( void ) const;

        std::shared_ptr< Statement > exit( void ) const;

        Blocks& blocks( void );

        void add( const Block::Ptr& block );

        template < typename T, typename... Args >
        typename T::Ptr add( Args&&... args )
        {
            auto obj = libstdhl::Memory::make< T >( std::forward< Args >( args )... );
            add( obj );
            return obj;
        }

        void replace( Block& from, const Block::Ptr to );

        std::string name( void ) const override final;

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

        static ParallelBlock::Ptr create( const u1 empty = false );

      private:
        ParallelBlock( void );

      public:
        void setRule( const std::shared_ptr< Rule >& rule );

        /**
           @return rule pointer if this parallel block is the context of a rule,
                   otherwise nullptr
         */
        std::shared_ptr< Rule > rule( void ) const;

        void replaceWith( const Block::Ptr block ) override;

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

        static SequentialBlock::Ptr create( u1 empty = false );

      private:
        SequentialBlock( void );

      public:
        void replaceWith( const Block::Ptr block ) override;

        void accept( Visitor& visitor ) override;

        static inline Value::ID classid( void )
        {
            return Value::SEQUENTIAL_BLOCK;
        }

        static u1 classof( Value const* obj );
    };
}

#endif  // _LIBCASM_IR_BLOCK_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

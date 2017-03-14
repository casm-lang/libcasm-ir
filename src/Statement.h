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

#ifndef _LIB_CASMIR_STATEMENT_H_
#define _LIB_CASMIR_STATEMENT_H_

#include "Block.h"

#include "Instruction.h"

namespace libcasm_ir
{
    class Statement : public Block
    {
      public:
        using Ptr = std::shared_ptr< Statement >;

        Statement( const std::string& name, Value::ID id = classid() );

        void add( const Instruction::Ptr& instruction );

        Instructions instructions( void ) const;

        void add( const ExecutionSemanticsBlock::Ptr& block );

        ExecutionSemanticsBlocks blocks( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STATEMENT;
        }

        static u1 classof( Value const* obj );

      private:
        Instructions m_instructions;
        ExecutionSemanticsBlocks m_blocks;
    };

    class TrivialStatement : public Statement
    {
      public:
        using Ptr = std::shared_ptr< TrivialStatement >;

        TrivialStatement( void );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::TRIVIAL_STATEMENT;
        }

        static u1 classof( Value const* obj );
    };

    class BranchStatement : public Statement
    {
      public:
        using Ptr = std::shared_ptr< BranchStatement >;

        BranchStatement( void );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::BRANCH_STATEMENT;
        }

        static u1 classof( Value const* obj );
    };

    // TODO: FIXME: PPA: add ForallStatement and IterateStatement etc.
}

#endif // _LIB_CASMIR_STATEMENT_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

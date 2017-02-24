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

#ifndef _LIB_CASMIR_INSTRUCTION_H_
#define _LIB_CASMIR_INSTRUCTION_H_

#include "TypeAnnotation.h"
#include "User.h"

#include "../stdhl/cpp/List.h"

namespace libcasm_ir
{
    class Statement;

    class Instruction : public User
    {
      public:
        using Ptr = std::shared_ptr< Instruction >;

        Instruction( const std::string& name, const Type::Ptr& type,
            const std::vector< Value::Ptr >& operands,
            Value::ID id = classid() );

        void add( const Value::Ptr& operand );

        Value::Ptr operand( u8 position ) const;

        Values operands( void ) const;

        void replace( const Value::Ptr& from, const Value::Ptr& to );

        void setStatement( const std::shared_ptr< Statement >& statement );

        std::shared_ptr< Statement > statement( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::INSTRUCTION;
        }

        static u1 classof( Value const* obj );

      private:
        Values m_operands;

        std::weak_ptr< Statement > m_statement;
    };

    using Instructions = libstdhl::List< Instruction >;

    class UnaryInstruction
    {
      public:
        UnaryInstruction( Instruction* self );

        Value::Ptr get( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::UNARY_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

      private:
        Instruction& m_self;
    };

    class BinaryInstruction
    {
      public:
        BinaryInstruction( Instruction* self );

        Value::Ptr lhs( void ) const;
        Value::Ptr rhs( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BINARY_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

      private:
        Instruction& m_self;
    };

    class SkipInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< SkipInstruction >;

        SkipInstruction( void );

        static inline Value::ID classid( void )
        {
            return Value::SKIP_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class ForkInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< ForkInstruction >;

        ForkInstruction( void );

        static inline Value::ID classid( void )
        {
            return Value::FORK_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class MergeInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< MergeInstruction >;

        MergeInstruction( void );

        static inline Value::ID classid( void )
        {
            return Value::MERGE_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LookupInstruction : public Instruction, public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< LookupInstruction >;

        LookupInstruction( const Value::Ptr& location );

        static inline Value::ID classid( void )
        {
            return Value::LOOKUP_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class UpdateInstruction : public Instruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< UpdateInstruction >;

        UpdateInstruction( const Value::Ptr& location, const Value::Ptr& expr );

        static inline Value::ID classid( void )
        {
            return Value::UPDATE_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LocalInstruction : public Instruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< LocalInstruction >;

        LocalInstruction( const Value::Ptr& ident, const Value::Ptr& expr );

        static inline Value::ID classid( void )
        {
            return Value::LOCAL_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LocationInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< LocationInstruction >;

        LocationInstruction( const Value::Ptr& function );

        static inline Value::ID classid( void )
        {
            return Value::LOCATION_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class CallInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< CallInstruction >;

        CallInstruction( const Value::Ptr& symbol,
            const std::vector< Value::Ptr >& operands = {} );

        Value::Ptr callee( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::CALL_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class PrintInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< PrintInstruction >;

        PrintInstruction( const Value::Ptr& channel = 0 );

        static inline Value::ID classid( void )
        {
            return Value::PRINT_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class AssertInstruction : public Instruction, public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< AssertInstruction >;

        AssertInstruction( const Value::Ptr& condition );

        static inline Value::ID classid( void )
        {
            return Value::ASSERT_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class SelectInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< SelectInstruction >;

        SelectInstruction( const Value::Ptr& expression );

        static inline Value::ID classid( void )
        {
            return Value::SELECT_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class OperatorInstruction : public Instruction, public TypeAnnotation
    {
      public:
        using Ptr = std::shared_ptr< OperatorInstruction >;

        OperatorInstruction( const std::string& name, const Type::Ptr& type,
            const std::vector< Value::Ptr >& operands,
            const TypeAnnotation& info, Value::ID id = classid() );

        const Type::ID resolved( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::OPERATOR_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

      private:
        Type::ID m_resolved;
    };

    class ArithmeticInstruction : public OperatorInstruction
    {
      public:
        using Ptr = std::shared_ptr< ArithmeticInstruction >;

        ArithmeticInstruction( const std::string& name,
            const std::vector< Value::Ptr >& operands,
            const TypeAnnotation& info, Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::ARITHMETIC_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class CompareInstruction : public OperatorInstruction
    {
      public:
        using Ptr = std::shared_ptr< CompareInstruction >;

        CompareInstruction( const std::string& name,
            const std::vector< Value::Ptr >& operands,
            const TypeAnnotation& info, Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::COMPARE_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LogicalInstruction : public OperatorInstruction
    {
      public:
        using Ptr = std::shared_ptr< LogicalInstruction >;

        LogicalInstruction( const std::string& name,
            const std::vector< Value::Ptr >& operands,
            const TypeAnnotation& info, Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::LOGICAL_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    //
    // Arithmetic Instructions
    //

    class AddInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< AddInstruction >;

        AddInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::ADD_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class SubInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< SubInstruction >;

        SubInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::SUB_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class MulInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< MulInstruction >;

        MulInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::MUL_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class DivInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< DivInstruction >;

        DivInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::DIV_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class ModInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< ModInstruction >;

        ModInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::MOD_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    //
    // Compare Instructions
    //

    class EquInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< EquInstruction >;

        EquInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::EQU_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class NeqInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< NeqInstruction >;

        NeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::NEQ_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class LthInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< LthInstruction >;

        LthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::LTH_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class LeqInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< LeqInstruction >;

        LeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::LEQ_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class GthInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< GthInstruction >;

        GthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::GTH_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class GeqInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< GeqInstruction >;

        GeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::GEQ_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    //
    // Logical Instructions
    //

    class OrInstruction : public LogicalInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< OrInstruction >;

        OrInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::OR_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class XorInstruction : public LogicalInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< XorInstruction >;

        XorInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::XOR_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AndInstruction : public LogicalInstruction, public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< AndInstruction >;

        AndInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::AND_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class NotInstruction : public LogicalInstruction, public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< NotInstruction >;

        NotInstruction( const Value::Ptr& lhs );

        static inline Value::ID classid( void )
        {
            return Value::NOT_INSTRUCTION;
        }

        static u1 classof( Value const* obj );

        static const TypeAnnotation info;
    };
}

#endif // _LIB_CASMIR_INSTRUCTION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

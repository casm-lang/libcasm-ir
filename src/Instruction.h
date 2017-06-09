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

#include "User.h"

#include "Annotation.h"
#include "Constant.h"

namespace libcasm_ir
{
    class Statement;

    class Instruction : public User
    {
      public:
        using Ptr = std::shared_ptr< Instruction >;

        Instruction( const Type::Ptr& type, const Value::ID id,
            const Constant* constants, const std::size_t size );

        Instruction( const Type::Ptr& type,
            const Value::ID id,
            const std::vector< Value::Ptr >& operands = {} );

        void add( const Value::Ptr& operand );

        inline std::size_t size( void ) const
        {
            return m_size;
        }

        inline const Constant* constants( void ) const
        {
            return m_constants;
        }

        Value::Ptr operand( u8 position ) const;

        const Values& operands( void ) const;

        void replace( Value& from, const Value::Ptr& to );

        void setStatement( const std::shared_ptr< Statement >& statement );

        std::shared_ptr< Statement > statement( void ) const;

        void setNext( const Instruction::Ptr& instruction );

        Instruction::Ptr next( void ) const;

        std::string name( void ) const override;

        std::size_t hash( void ) const override;

        static inline Value::ID classid( void )
        {
            return Value::INSTRUCTION;
        }

        static u1 classof( Value const* obj );

      private:
        std::size_t m_size;
        const Constant* m_constants;

        Values m_operands;

        std::weak_ptr< Statement > m_statement;

        std::weak_ptr< Instruction > m_next;
    };

    using Instructions = ValueList< Instruction >;

    class UnaryInstruction
    {
      public:
        static u1 classof( Value const* obj );
    };

    class BinaryInstruction
    {
      public:
        static u1 classof( Value const* obj );
    };

    class SkipInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< SkipInstruction >;

        SkipInstruction( void );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::SKIP_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class ForkInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< ForkInstruction >;

        ForkInstruction( void );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::FORK_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class MergeInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< MergeInstruction >;

        MergeInstruction( void );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::MERGE_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LookupInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< LookupInstruction >;

        LookupInstruction( const Value::Ptr& location );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::LOOKUP_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class UpdateInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< UpdateInstruction >;

        UpdateInstruction( const Value::Ptr& location, const Value::Ptr& expr );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::UPDATE_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LocalInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< LocalInstruction >;

        LocalInstruction( const Value::Ptr& ident, const Value::Ptr& expr );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::LOCAL_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LocationInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< LocationInstruction >;

        LocationInstruction( const Value::Ptr& function,
            const std::vector< Value::Ptr >& operands = {} );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::LOCATION_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class CallInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< CallInstruction >;

        CallInstruction( const Value::Ptr& symbol,
            const std::vector< Value::Ptr >& operands = {} );

        Value::Ptr callee( void ) const;

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::CALL_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class SelectInstruction final : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< SelectInstruction >;

        SelectInstruction( const Value::Ptr& expression,
            const std::vector< Value::Ptr >& operands );

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::SELECT_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class OperatorInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< OperatorInstruction >;

        OperatorInstruction( const Type::Ptr& type, const Value::ID id,
            const std::vector< Value::Ptr >& operands );

        OperatorInstruction( const Type::Ptr& type, const Value::ID id,
            const Constant* operands, const std::size_t size )
        : Instruction( type, id, operands, size )
        {
        }

        static inline Value::ID classid( void )
        {
            return Value::OPERATOR_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class ArithmeticInstruction : public OperatorInstruction
    {
      public:
        using Ptr = std::shared_ptr< ArithmeticInstruction >;

        ArithmeticInstruction(
            const Value::ID id, const std::vector< Value::Ptr >& operands );

        ArithmeticInstruction( const Type::Ptr& type, const Value::ID id,
            const Constant* operands, const std::size_t size )
        : OperatorInstruction( type, id, operands, size )
        {
        }

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

        CompareInstruction(
            const Value::ID id, const std::vector< Value::Ptr >& operands );

        CompareInstruction( const Value::ID id, const Constant* operands,
            const std::size_t size );

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

        LogicalInstruction(
            const Value::ID id, const std::vector< Value::Ptr >& operands );

        LogicalInstruction( const Type::Ptr& type, const Value::ID id,
            const Constant* operands, const std::size_t size )
        : OperatorInstruction( type, id, operands, size )
        {
        }

        static inline Value::ID classid( void )
        {
            return Value::LOGICAL_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    //
    // Arithmetic Instructions
    //

    class InvInstruction final : public ArithmeticInstruction
    {
      public:
        using Ptr = std::shared_ptr< InvInstruction >;

        InvInstruction( const Value::Ptr& lhs );

        InvInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::INV_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class AddInstruction final : public ArithmeticInstruction
    {
      public:
        using Ptr = std::shared_ptr< AddInstruction >;

        AddInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        AddInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::ADD_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class SubInstruction final : public ArithmeticInstruction
    {
      public:
        using Ptr = std::shared_ptr< SubInstruction >;

        SubInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        SubInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::SUB_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class MulInstruction final : public ArithmeticInstruction
    {
      public:
        using Ptr = std::shared_ptr< MulInstruction >;

        MulInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        MulInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::MUL_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class ModInstruction final : public ArithmeticInstruction
    {
      public:
        using Ptr = std::shared_ptr< ModInstruction >;

        ModInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        ModInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::MOD_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class DivInstruction final : public ArithmeticInstruction
    {
      public:
        using Ptr = std::shared_ptr< DivInstruction >;

        DivInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        DivInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::DIV_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class PowInstruction final : public ArithmeticInstruction
    {
      public:
        using Ptr = std::shared_ptr< PowInstruction >;

        PowInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        PowInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::POW_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    //
    // Logical Instructions
    //

    class AndInstruction final : public LogicalInstruction
    {
      public:
        using Ptr = std::shared_ptr< AndInstruction >;

        AndInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        AndInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::AND_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class XorInstruction final : public LogicalInstruction
    {
      public:
        using Ptr = std::shared_ptr< XorInstruction >;

        XorInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        XorInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::XOR_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class OrInstruction final : public LogicalInstruction
    {
      public:
        using Ptr = std::shared_ptr< OrInstruction >;

        OrInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        OrInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::OR_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class ImpInstruction final : public LogicalInstruction
    {
      public:
        using Ptr = std::shared_ptr< ImpInstruction >;

        ImpInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        ImpInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::IMP_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class NotInstruction final : public LogicalInstruction
    {
      public:
        using Ptr = std::shared_ptr< NotInstruction >;

        NotInstruction( const Value::Ptr& lhs );

        NotInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::NOT_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    //
    // Compare Instructions
    //

    class EquInstruction final : public CompareInstruction
    {
      public:
        using Ptr = std::shared_ptr< EquInstruction >;

        EquInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        EquInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::EQU_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class NeqInstruction final : public CompareInstruction
    {
      public:
        using Ptr = std::shared_ptr< NeqInstruction >;

        NeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        NeqInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::NEQ_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LthInstruction final : public CompareInstruction
    {
      public:
        using Ptr = std::shared_ptr< LthInstruction >;

        LthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        LthInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::LTH_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class LeqInstruction final : public CompareInstruction
    {
      public:
        using Ptr = std::shared_ptr< LeqInstruction >;

        LeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        LeqInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::LEQ_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class GthInstruction final : public CompareInstruction
    {
      public:
        using Ptr = std::shared_ptr< GthInstruction >;

        GthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        GthInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::GTH_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
    };

    class GeqInstruction final : public CompareInstruction
    {
      public:
        using Ptr = std::shared_ptr< GeqInstruction >;

        GeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        GeqInstruction( const Constant* operands, const std::size_t size );

        void accept( Visitor& visitor ) override final;

        static const Annotation info;

        static inline Value::ID classid( void )
        {
            return Value::GEQ_INSTRUCTION;
        }

        static u1 classof( Value const* obj );
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

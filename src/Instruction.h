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

#ifndef _LIB_CASMIR_INSTRUCTION_H_
#define _LIB_CASMIR_INSTRUCTION_H_

#include "Builtin.h"
#include "Constant.h"
#include "Statement.h"
#include "User.h"

namespace libcasm_ir
{
    class Statement;

    class Instruction : public User
    {
      private:
        Statement* statement;
        std::vector< Value* > values;

      public:
        Instruction( const char* name, Type* type,
            const std::vector< Value* >& values,
            Value::ID id = Value::INSTRUCTION );
        void setStatement( Statement* stmt );
        const Statement* getStatement( void ) const;

        void add( Value* value );
        Value* getValue( u8 index ) const;
        const std::vector< Value* >& getValues( void ) const;

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class UnaryInstruction : public CasmIR
    {
      private:
        Instruction& self;

      public:
        UnaryInstruction( Instruction* self );

        Value* get( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::UNARY_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class BinaryInstruction : public CasmIR
    {
      private:
        Instruction& self;

      public:
        BinaryInstruction( Instruction* self );

        Value* getLHS( void ) const;
        Value* getRHS( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BINARY_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class SkipInstruction : public Instruction
    {
      public:
        SkipInstruction( void );

        static inline Value::ID classid( void )
        {
            return Value::SKIP_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class LookupInstruction : public Instruction, public UnaryInstruction
    {
      public:
        LookupInstruction( Value* location );

        static inline Value::ID classid( void )
        {
            return Value::LOOKUP_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class UpdateInstruction : public Instruction, public BinaryInstruction
    {
      public:
        UpdateInstruction( Value* location, Value* expr );

        static inline Value::ID classid( void )
        {
            return Value::UPDATE_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class LocalInstruction : public Instruction, public BinaryInstruction
    {
      public:
        LocalInstruction( Value* ident, Value* expr );

        static inline Value::ID classid( void )
        {
            return Value::LOCAL_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class LocationInstruction : public Instruction
    {
      public:
        LocationInstruction( Value* function );

        static inline Value::ID classid( void )
        {
            return Value::LOCATION_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class CallInstruction : public Instruction
    {
      public:
        CallInstruction( Value* symbol );

        static inline Value::ID classid( void )
        {
            return Value::CALL_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class PrintInstruction : public Instruction
    {
      public:
        PrintInstruction( Value* channel = 0 );

        static inline Value::ID classid( void )
        {
            return Value::PRINT_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class AssertInstruction : public Instruction, public UnaryInstruction
    {
      public:
        AssertInstruction( Value* condition );

        static inline Value::ID classid( void )
        {
            return Value::ASSERT_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class SwitchInstruction : public Instruction
    {
      public:
        SwitchInstruction( Value* expression );

        static inline Value::ID classid( void )
        {
            return Value::SWITCH_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class OperatorInstruction : public Instruction, public TypeAnnotation
    {
      private:
        Type::ID resolved;

      public:
        OperatorInstruction( const char* name, Type* type,
            std::vector< Value* > values, const TypeAnnotation& info,
            Value::ID id = Value::OPERATOR_INSTRUCTION );

        const Type::ID getResolved( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::OPERATOR_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class ArithmeticInstruction : public OperatorInstruction
    {
      public:
        ArithmeticInstruction( const char* name, Type* type,
            std::vector< Value* > values, const TypeAnnotation& info,
            Value::ID id = Value::ARITHMETIC_INSTRUCTION );

        static inline Value::ID classid( void )
        {
            return Value::ARITHMETIC_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class CompareInstruction : public OperatorInstruction
    {
      public:
        CompareInstruction( const char* name, std::vector< Value* > values,
            const TypeAnnotation& info,
            Value::ID id = Value::COMPARE_INSTRUCTION );

        static inline Value::ID classid( void )
        {
            return Value::COMPARE_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class LogicalInstruction : public OperatorInstruction
    {
      public:
        LogicalInstruction( const char* name, Type* type,
            std::vector< Value* > values, const TypeAnnotation& info,
            Value::ID id = Value::LOGICAL_INSTRUCTION );

        static inline Value::ID classid( void )
        {
            return Value::LOGICAL_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class AddInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        AddInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::ADD_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class SubInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        SubInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::SUB_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class MulInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        MulInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::MUL_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class DivInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        DivInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::DIV_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class ModInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        ModInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::MOD_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class EquInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        EquInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::EQU_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class NeqInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        NeqInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::NEQ_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class LthInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        LthInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::LTH_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class LeqInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        LeqInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::LEQ_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class GthInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        GthInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::GTH_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class GeqInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        GeqInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::GEQ_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class OrInstruction : public LogicalInstruction, public BinaryInstruction
    {
      public:
        OrInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::OR_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class XorInstruction : public LogicalInstruction, public BinaryInstruction
    {
      public:
        XorInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::XOR_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AndInstruction : public LogicalInstruction, public BinaryInstruction
    {
      public:
        AndInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void )
        {
            return Value::AND_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class NotInstruction : public LogicalInstruction, public UnaryInstruction
    {
      public:
        NotInstruction( Value* lhs );

        static inline Value::ID classid( void )
        {
            return Value::NOT_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    // class MovInstruction : public UnaryInstruction
    // {
    //   public:
    //     MovInstruction( Value* lhs );

    //     static inline Value::ID classid( void )
    //     {
    //         return Value::MOV_INSTRUCTION;
    //     };
    //     static bool classof( Value const* obj );
    // };
}

#endif /* _LIB_CASMIR_INSTRUCTION_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

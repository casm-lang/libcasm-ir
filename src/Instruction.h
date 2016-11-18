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

#include "User.h"
#include "Builtin.h"
#include "Constant.h"
#include "Statement.h"

namespace libcasm_ir
{
    class Statement;
    
    class Instruction : public User
    {
    private:
        Statement* statement;
        std::vector< Value* > values;
        
    public:
        Instruction( const char* name, Type* type, Value::ID id = Value::INSTRUCTION );
        void setStatement( Statement* stmt );
        const Statement* getStatement( void ) const;
        
        void add( Value* value );
        Value* getValue( u8 index ) const;
        const std::vector< Value* >& getValues( void ) const;
        
        void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    class UnaryInstruction : public Instruction
    {
    public:
        UnaryInstruction( const char* name, Type* type, Value* value
                        , Value::ID id = Value::UNARY_INSTRUCTION );
        Value* get( void ) const;

        static inline Value::ID classid( void ) { return Value::UNARY_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    class BinaryInstruction : public Instruction
    {
    public:
        BinaryInstruction( const char* name, Type* type, Value* lhs, Value* rhs
                         , Value::ID id = Value::BINARY_INSTRUCTION );
        
        Value* getLHS( void ) const;
        Value* getRHS( void ) const;
                
        static inline Value::ID classid( void ) { return Value::BINARY_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    
    
    class SkipInstruction : public Instruction
    {
    public:
        SkipInstruction( void );
        
        //void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::SKIP_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    
    
    class LookupInstruction : public UnaryInstruction
    {
    public :
        LookupInstruction( Value* location );

        //void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::LOOKUP_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    
    class UpdateInstruction : public BinaryInstruction
    {
    public:
        UpdateInstruction( Value* location, Value* expr );

        //void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::UPDATE_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    
    class LetInstruction : public BinaryInstruction
    {
    public:
        LetInstruction( Value* ident, Value* expr );
        
        static inline Value::ID classid( void ) { return Value::LET_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    class LocationInstruction : public Instruction
    {
    public:
        LocationInstruction( Value* function );

        //void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::LOCATION_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    class CallInstruction : public Instruction
    {
    public:
        CallInstruction( Value* symbol );
        
        //void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::CALL_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class PrintInstruction : public Instruction
    {
    public:
        PrintInstruction( Value* channel = 0 );
        
        static inline Value::ID classid( void ) { return Value::PRINT_INSTRUCTION; };
        static bool classof( Value const* obj );
    };


    class AssertInstruction : public UnaryInstruction
    {
    public :
        AssertInstruction( Value* condition );

        //void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::ASSERT_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    
    class SwitchInstruction : public Instruction
    {
    public:
        SwitchInstruction( Value* expression );
        
        //void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::SWITCH_INSTRUCTION; };
        static bool classof( Value const* obj );
    };    
    
    
    class OperatorInstruction : public BinaryInstruction
    {
    public:
        OperatorInstruction( const char* name, Type* type, Value* lhs, Value* rhs
                           , Value::ID id = Value::OPERATOR_INSTRUCTION );
        
        //void dump( void ) const;
        
        static inline Value::ID classid( void ) { return Value::OPERATOR_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class AddInstruction : public OperatorInstruction
    {
    public:
        AddInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void ) { return Value::ADD_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class SubInstruction : public OperatorInstruction
    {
    public:
        SubInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::SUB_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class MulInstruction : public OperatorInstruction
    {
    public:
        MulInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::MUL_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class DivInstruction : public OperatorInstruction
    {
    public:
        DivInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::DIV_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class RivInstruction : public OperatorInstruction
    {
    public:
        RivInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::RIV_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class ModInstruction : public OperatorInstruction
    {
    public:
        ModInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::MOD_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class EquInstruction : public OperatorInstruction
    {
    public:
        EquInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::EQU_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class NeqInstruction : public OperatorInstruction
    {
    public:
        NeqInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::NEQ_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class LthInstruction : public OperatorInstruction
    {
    public:
        LthInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::LTH_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class LeqInstruction : public OperatorInstruction
    {
    public:
        LeqInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void ) { return Value::LEQ_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class GthInstruction : public OperatorInstruction
    {
    public:
        GthInstruction( Value* lhs, Value* rhs );

        static inline Value::ID classid( void ) { return Value::GTH_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class GeqInstruction : public OperatorInstruction
    {
    public:
        GeqInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::GEQ_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class OrInstruction : public OperatorInstruction
    {
    public:
        OrInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::OR_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class XorInstruction : public OperatorInstruction
    {
    public:
        XorInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::XOR_INSTRUCTION; };
        static bool classof( Value const* obj );
    };

    class AndInstruction : public OperatorInstruction
    {
    public:
        AndInstruction( Value* lhs, Value* rhs );
        
        static inline Value::ID classid( void ) { return Value::AND_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    class NotInstruction : public UnaryInstruction
    {
    public:
        NotInstruction( Value* lhs );
        
        static inline Value::ID classid( void ) { return Value::NOT_INSTRUCTION; };
        static bool classof( Value const* obj );
    };
    
    class MovInstruction : public UnaryInstruction
    {
    public:
        MovInstruction( Value* lhs );
        
        static inline Value::ID classid( void ) { return Value::MOV_INSTRUCTION; };
        static bool classof( Value const* obj );
    };    
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

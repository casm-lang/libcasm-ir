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

#include "Instruction.h"

#include "Builtin.h"
#include "Derived.h"
#include "Function.h"
#include "Rule.h"
#include "Statement.h"

using namespace libcasm_ir;

static const auto VOID = libstdhl::get< VoidType >();
static const auto BOOLEAN = libstdhl::get< BooleanType >();
static const auto INTEGER = libstdhl::get< IntegerType >();

static const std::string EMPTY = "";

//
// Instruction
//

Instruction::Instruction( const Type::Ptr& type,
    const Value::ID id,
    const std::vector< Value::Ptr >& operands )
: User( EMPTY, type, id )
, m_size( operands.size() )
, m_constants( nullptr )
{
    for( auto operand : operands )
    {
        add( operand );
    }
}

void Instruction::add( const Value::Ptr& operand )
{
    if( isa< UnaryInstruction >( this ) and m_operands.size() >= 1 )
    {
        throw std::domain_error(
            "impossible to add more arguments to unary instruction" );
    }
    else if( isa< BinaryInstruction >( this ) and m_operands.size() >= 2 )
    {
        throw std::domain_error(
            "impossible to add more arguments to binary instruction" );
    }

    if( not operand )
    {
        throw std::domain_error( "instruction operand is a null pointer" );
    }

    m_operands.emplace_back( operand );

    if( isa< User >( operand ) )
    {
        User* user = static_cast< User* >( operand.get() );

        user->setUse( *this );
    }
}

Value::Ptr Instruction::operand( u8 position ) const
{
    if( position >= m_operands.size() )
    {
        throw std::domain_error( "instruction operand position '"
                                 + std::to_string( position )
                                 + "' does not exist!" );
    }

    if( auto element = m_operands[ position ] )
    {
        return element;
    }
    else
    {
        throw std::domain_error( "operand points to null pointer" );
    }
}

const std::vector< Value::Ptr >& Instruction::operands( void ) const
{
    return m_operands;
}

void Instruction::replace( Value& from, const Value::Ptr& to )
{
    std::cerr << "replace: " << from.label() << " -> " << to->label() << "\n";

    std::replace_if( m_operands.begin(), m_operands.end(),
        [&]( const Value::Ptr& v ) { return *v.get() == from; }, to );

    if( isa< User >( from ) )
    {
        std::cerr << "replace-from: remove use of " << this->label() << " -> "
                  << from.label() << "\n";

        User& user = static_cast< User& >( from );
        user.removeUse( *this );
    }

    if( isa< User >( to ) )
    {
        std::cerr << "replace-to: set use of " << to->label() << "\n";

        auto user = std::static_pointer_cast< User >( to );
        user->setUse( *this );
    }
}

void Instruction::setStatement( const Statement::Ptr& statement )
{
    m_statement = statement;
}

Statement::Ptr Instruction::statement( void ) const
{
    return m_statement.lock();
}

void Instruction::setNext( const Instruction::Ptr& instruction )
{
    assert( instruction );
    assert( instruction.get() != this );
    assert( next() == nullptr );

    m_next = instruction;
}

Instruction::Ptr Instruction::next( void ) const
{
    return m_next.lock();
}

std::string Instruction::name( void ) const
{
    return Value::token( id() );
}

std::size_t Instruction::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

u1 Instruction::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    return true;
}

u1 Instruction::classof( Value const* obj )
{
    return obj->id() == classid() or SkipInstruction::classof( obj )
           or ForkInstruction::classof( obj )
           or MergeInstruction::classof( obj )
           or LookupInstruction::classof( obj )
           or UpdateInstruction::classof( obj )
           or LocalInstruction::classof( obj )
           or LocationInstruction::classof( obj )
           or CallInstruction::classof( obj )
           or SelectInstruction::classof( obj )
           or OperatorInstruction::classof( obj );
}

//
// Unary Instruction
//

u1 UnaryInstruction::classof( Value const* obj )
{
    if( Instruction::classof( obj ) )
    {
        if( isa< LocationInstruction >( obj ) or isa< CallInstruction >( obj ) )
        {
            return false;
        }

        const auto& instr = static_cast< const Instruction& >( *obj );
        return instr.size() == 1;
    }
    else
    {
        return false;
    }
}

//
// Binary Instruction
//

u1 BinaryInstruction::classof( Value const* obj )
{
    if( Instruction::classof( obj ) )
    {
        if( isa< LocationInstruction >( obj ) or isa< CallInstruction >( obj ) )
        {
            return false;
        }

        const auto& instr = static_cast< const Instruction& >( *obj );
        return instr.size() == 2;
    }
    else
    {
        return false;
    }
}

//
// INSTRUCTION IMPLEMENTATIONS
//

//
// Skip Instruction
//

SkipInstruction::SkipInstruction( void )
: Instruction( VOID, classid() )
{
}

void SkipInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 SkipInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Fork Instruction
//

ForkInstruction::ForkInstruction( void )
: Instruction( VOID, classid() )
{
}

void ForkInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 ForkInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Merge Instruction
//

MergeInstruction::MergeInstruction( void )
: Instruction( VOID, classid() )
{
}

void MergeInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 MergeInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Lookup Instruction
//

LookupInstruction::LookupInstruction( const Value::Ptr& location )
: Instruction( location->ptr_type()->ptr_result(), classid(), { location } )
{
}

void LookupInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 LookupInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Update Instruction
//

UpdateInstruction::UpdateInstruction(
    const Value::Ptr& location, const Value::Ptr& expr )
: Instruction( expr->ptr_type(), classid(), { location, expr } )
{
}

void UpdateInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 UpdateInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Local Instruction
//

LocalInstruction::LocalInstruction(
    const Value::Ptr& ident, const Value::Ptr& expr )
: Instruction( expr->ptr_type(), classid(), { ident, expr } )
{
}

void LocalInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 LocalInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Location Instruction
//

LocationInstruction::LocationInstruction(
    const Value::Ptr& function, const std::vector< Value::Ptr >& operands )
: Instruction( function->ptr_type(), classid(), { function } )
{
    assert( isa< Function >( function ) );

    for( auto operand : operands )
    {
        add( operand );
    }
}

void LocationInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 LocationInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Call Instruction
//

CallInstruction::CallInstruction(
    const Value::Ptr& symbol, const std::vector< Value::Ptr >& operands )
: Instruction( symbol->ptr_type()->ptr_result(), classid(), { symbol } )
{
    assert( isa< Rule >( symbol ) or isa< Derived >( symbol )
            or isa< Builtin >( symbol ) );

    for( auto operand : operands )
    {
        add( operand );
    }
}

Value::Ptr CallInstruction::callee( void ) const
{
    return operand( 0 );
}

void CallInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 CallInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Select Instruction
//

SelectInstruction::SelectInstruction(
    const Value::Ptr& expression, const std::vector< Value::Ptr >& operands )
: Instruction( expression->type().ptr_result(), classid(), operands )
{
}

void SelectInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 SelectInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Operator Instruction
//

OperatorInstruction::OperatorInstruction( const Type::Ptr& type,
    const Value::ID id, const std::vector< Value::Ptr >& operands )
: Instruction( type, id, operands )
{
}

u1 OperatorInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or ArithmeticInstruction::classof( obj )
           or CompareInstruction::classof( obj )
           or LogicalInstruction::classof( obj );
}

//
// Arithmetic Instruction
//

ArithmeticInstruction::ArithmeticInstruction(
    const Value::ID id, const std::vector< Value::Ptr >& operands )
: OperatorInstruction(
      operands[ 0 ] ? operands[ 0 ]->ptr_type() : VOID, id, operands )
{
    assert( operands.size() <= 2 );

    const auto& lhs_ty = operand( 0 )->type();
    if( operands.size() > 1 )
    {
        const auto& rhs_ty = operand( 1 )->type();
        assert( lhs_ty == rhs_ty );
    }
}

u1 ArithmeticInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or InvInstruction::classof( obj )
           or AddInstruction::classof( obj ) or SubInstruction::classof( obj )
           or MulInstruction::classof( obj ) or DivInstruction::classof( obj )
           or PowInstruction::classof( obj ) or ModInstruction::classof( obj )
           or OrInstruction::classof( obj ) or XorInstruction::classof( obj )
           or AndInstruction::classof( obj ) or NotInstruction::classof( obj );
}

static auto arithmetic_instruction_inference
    = []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& ) -> Type::Ptr {
    assert( types.size() >= 1 );
    assert( types.size() <= 2 );

    const auto& lhs = types[ 0 ];
    if( types.size() == 2 )
    {
        const auto& rhs = types[ 1 ];

        if( lhs->isInteger() and rhs->isInteger() )
        {
            return INTEGER;
        }

        if( *lhs != *rhs )
        {
            return nullptr;
        }
    }
    return lhs;
};

//
// Inv Instruction
//

InvInstruction::InvInstruction( const Value::Ptr& lhs )
: ArithmeticInstruction( classid(), { lhs } )
{
}

InvInstruction::InvInstruction(
    const Constant* constants, const std::size_t size )
: ArithmeticInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void InvInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation InvInstruction::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::BIT,
            } },

        { Type::FLOATING,
            {
                Type::FLOATING,
            } },

        { Type::RATIONAL,
            {
                Type::RATIONAL,
            } },

    },
    arithmetic_instruction_inference );

u1 InvInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Add Instruction
//

AddInstruction::AddInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( classid(), { lhs, rhs } )
{
}

AddInstruction::AddInstruction(
    const Constant* constants, const std::size_t size )
: ArithmeticInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void AddInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation AddInstruction::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::INTEGER, Type::INTEGER, // signed integer add no wrap
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT, // unsigned bit add with wrap
            } },

        { Type::STRING,
            {
                Type::STRING, Type::STRING, // concatenation
            } },

    },
    arithmetic_instruction_inference );

u1 AddInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Sub Instruction
//

SubInstruction::SubInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( classid(), { lhs, rhs } )
{
}

SubInstruction::SubInstruction(
    const Constant* constants, const std::size_t size )
: ArithmeticInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void SubInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation SubInstruction::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::INTEGER, Type::INTEGER, // signed integer sub no wrap
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT, // unsigned bit sub with wrap
            } },

    },
    arithmetic_instruction_inference );

u1 SubInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Mul Instruction
//

MulInstruction::MulInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( classid(), { lhs, rhs } )
{
}

MulInstruction::MulInstruction(
    const Constant* constants, const std::size_t size )
: ArithmeticInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void MulInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation MulInstruction::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::INTEGER, Type::INTEGER, // signed integer mul no wrap
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT, // unsigned integer mul with wrap
            } },

    },
    arithmetic_instruction_inference );

u1 MulInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Mod Instruction
//

ModInstruction::ModInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( classid(), { lhs, rhs } )
{
}

ModInstruction::ModInstruction(
    const Constant* constants, const std::size_t size )
: ArithmeticInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void ModInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation ModInstruction::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::INTEGER, Type::INTEGER, // signed integer mod no wrap
            } },

    },
    arithmetic_instruction_inference );

u1 ModInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Div Instruction
//

DivInstruction::DivInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( classid(), { lhs, rhs } )
{
}

DivInstruction::DivInstruction(
    const Constant* constants, const std::size_t size )
: ArithmeticInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void DivInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation DivInstruction::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::INTEGER, Type::INTEGER, // signed integer div no wrap
            } },

        { Type::RATIONAL,
            {
                Type::RATIONAL, Type::RATIONAL, // signed ration div no wrap
            } },

    },
    arithmetic_instruction_inference );

u1 DivInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Pow Instruction
//

PowInstruction::PowInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( classid(), { lhs, rhs } )
{
}

PowInstruction::PowInstruction(
    const Constant* constants, const std::size_t size )
: ArithmeticInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void PowInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation PowInstruction::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::INTEGER, Type::INTEGER, // signed integer pow no wrap
            } },

        // { Type::RATIONAL, // TODO: PPA: enable this after clear semantics
        //     {
        //         Type::RATIONAL, Type::INTEGER,
        //     } },

        // { Type::FLOATING, // TODO: PPA: enable this after clear semantics
        //
        //     {
        //         Type::FLOATING, Type::FLOATING,
        //     } },

    },
    arithmetic_instruction_inference );

u1 PowInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Logical Instruction
//

LogicalInstruction::LogicalInstruction(
    const Value::ID id, const std::vector< Value::Ptr >& operands )
: OperatorInstruction( operands[ 0 ] ? ( operands[ 0 ]->type().isBit()
                                               ? operands[ 0 ]->ptr_type()
                                               : BOOLEAN )
                                     : VOID,
      id, operands )
{
    assert( operands.size() <= 2 );

    const auto& lhs_ty = operands[ 0 ]->type();
    if( operands.size() > 1 )
    {
        const auto& rhs_ty = operands[ 1 ]->type();
        assert( lhs_ty == rhs_ty );
    }
}

static auto logical_instruction_inference
    = []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& ) -> Type::Ptr {
    assert( types.size() >= 1 );
    assert( types.size() <= 2 );

    const auto& lhs = types[ 0 ];
    if( types.size() == 2 )
    {
        const auto& rhs = types[ 1 ];

        if( *lhs != *rhs )
        {
            return nullptr;
        }
    }
    return lhs;
};

u1 LogicalInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or OrInstruction::classof( obj )
           or ImpInstruction::classof( obj ) or XorInstruction::classof( obj )
           or AndInstruction::classof( obj ) or NotInstruction::classof( obj );
}

//
// And Instruction
//

AndInstruction::AndInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( classid(), { lhs, rhs } )
{
}

AndInstruction::AndInstruction(
    const Constant* constants, const std::size_t size )
: LogicalInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void AndInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation AndInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::BOOLEAN, Type::BOOLEAN,
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } },

    },
    logical_instruction_inference );

u1 AndInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Xor Instruction
//

XorInstruction::XorInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( classid(), { lhs, rhs } )
{
}

XorInstruction::XorInstruction(
    const Constant* constants, const std::size_t size )
: LogicalInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void XorInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation XorInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::BOOLEAN, Type::BOOLEAN,
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } },

    },
    logical_instruction_inference );

u1 XorInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Or Instruction
//

OrInstruction::OrInstruction(
    const Constant* constants, const std::size_t size )
: LogicalInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

OrInstruction::OrInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( classid(), { lhs, rhs } )
{
}

void OrInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation OrInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::BOOLEAN, Type::BOOLEAN,
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } },

    },
    logical_instruction_inference );

u1 OrInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Imp Instruction (Implies)
//

ImpInstruction::ImpInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( classid(), { lhs, rhs } )
{
}

ImpInstruction::ImpInstruction(
    const Constant* constants, const std::size_t size )
: LogicalInstruction( constants ? constants[ 0 ].type().ptr_result() : VOID,
      classid(), constants, size )
{
}

void ImpInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation ImpInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::BOOLEAN, Type::BOOLEAN,
            } },

    },
    logical_instruction_inference );

u1 ImpInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Not Instruction
//

NotInstruction::NotInstruction( const Value::Ptr& lhs )
: LogicalInstruction( classid(), { lhs } )
{
}

NotInstruction::NotInstruction(
    const Constant* constants, const std::size_t size )
: LogicalInstruction(
      constants ? ( constants[ 0 ].type().isBit() ? constants[ 0 ].ptr_type()
                                                  : BOOLEAN )
                : VOID,
      classid(), constants, size )
{
}

void NotInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation NotInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::BOOLEAN,
            } },

        { Type::BOOLEAN,
            {
                Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::BIT,
            } },

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& ) -> Type::Ptr {
        assert( types.size() == 1 );
        return types[ 0 ]->isBit() ? types[ 0 ] : BOOLEAN;
    } );

u1 NotInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Compare Instruction
//

CompareInstruction::CompareInstruction(
    const Value::ID id, const std::vector< Value::Ptr >& operands )
: OperatorInstruction( BOOLEAN, id, operands )
{
}

CompareInstruction::CompareInstruction(
    const Value::ID id, const Constant* operands, const std::size_t size )
: OperatorInstruction( BOOLEAN, id, operands, size )
{
}

u1 CompareInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or EquInstruction::classof( obj )
           or NeqInstruction::classof( obj ) or LthInstruction::classof( obj )
           or LeqInstruction::classof( obj ) or GthInstruction::classof( obj )
           or GeqInstruction::classof( obj );
}

static auto compare_instruction_inference
    = []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& ) -> Type::Ptr {
    assert( types.size() == 2 );
    const auto& lhs = types[ 0 ];
    const auto& rhs = types[ 1 ];

    if( lhs->isInteger() and rhs->isInteger() )
    {
        return BOOLEAN;
    }

    if( *lhs != *rhs )
    {
        return nullptr;
    }

    return BOOLEAN;
};

//
// Equ Instruction
//

EquInstruction::EquInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( classid(), { lhs, rhs } )
{
}

EquInstruction::EquInstruction(
    const Constant* constants, const std::size_t size )
: CompareInstruction( classid(), constants, size )
{
}

void EquInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation EquInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::RULE_REFERENCE, Type::RULE_REFERENCE,
            } },

        { Type::BOOLEAN,
            {
                Type::BOOLEAN, Type::BOOLEAN,
            } },

        { Type::BOOLEAN,
            {
                Type::INTEGER, Type::INTEGER,
            } },

        { Type::BOOLEAN,
            {
                Type::BIT, Type::BIT,
            } },

        { Type::BOOLEAN,
            {
                Type::STRING, Type::STRING,
            } },

        { Type::BOOLEAN,
            {
                Type::RATIONAL, Type::RATIONAL,
            } },

        { Type::BOOLEAN,
            {
                Type::ENUMERATION, Type::ENUMERATION,
            } },

    },
    compare_instruction_inference );

u1 EquInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Neq Instruction
//

NeqInstruction::NeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( classid(), { lhs, rhs } )
{
}

NeqInstruction::NeqInstruction(
    const Constant* constants, const std::size_t size )
: CompareInstruction( classid(), constants, size )
{
}

void NeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation NeqInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::RULE_REFERENCE, Type::RULE_REFERENCE,
            } },

        { Type::BOOLEAN,
            {
                Type::BOOLEAN, Type::BOOLEAN,
            } },

        { Type::BOOLEAN,
            {
                Type::INTEGER, Type::INTEGER,
            } },

        { Type::BOOLEAN,
            {
                Type::BIT, Type::BIT,
            } },

        { Type::BOOLEAN,
            {
                Type::STRING, Type::STRING,
            } },

        { Type::BOOLEAN,
            {
                Type::RATIONAL, Type::RATIONAL,
            } },

        { Type::BOOLEAN,
            {
                Type::ENUMERATION, Type::ENUMERATION,
            } },

    },
    compare_instruction_inference );

u1 NeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Lth Instruction
//

LthInstruction::LthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( classid(), { lhs, rhs } )
{
}

LthInstruction::LthInstruction(
    const Constant* constants, const std::size_t size )
: CompareInstruction( classid(), constants, size )
{
}

void LthInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation LthInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::INTEGER, Type::INTEGER,
            } },

    },
    compare_instruction_inference );

u1 LthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Leq Instruction
//

LeqInstruction::LeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( classid(), { lhs, rhs } )
{
}

LeqInstruction::LeqInstruction(
    const Constant* constants, const std::size_t size )
: CompareInstruction( classid(), constants, size )
{
}

void LeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation LeqInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::INTEGER, Type::INTEGER,
            } },

    },
    compare_instruction_inference );

u1 LeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Gth Instruction
//

GthInstruction::GthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( classid(), { lhs, rhs } )
{
}

GthInstruction::GthInstruction(
    const Constant* constants, const std::size_t size )
: CompareInstruction( classid(), constants, size )
{
}

void GthInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation GthInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::INTEGER, Type::INTEGER,
            } },

    },
    compare_instruction_inference );

u1 GthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Geq Instruction
//

GeqInstruction::GeqInstruction(
    const Constant* constants, const std::size_t size )
: CompareInstruction( classid(), constants, size )
{
}

GeqInstruction::GeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( classid(), { lhs, rhs } )
{
}

void GeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation GeqInstruction::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::INTEGER, Type::INTEGER,
            } },

    },
    compare_instruction_inference );

u1 GeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

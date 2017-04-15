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

//
// Instruction
//

Instruction::Instruction( const Type::Ptr& type,
    const std::vector< Value::Ptr >& operands, Value::ID id )
: User( "", type, id )
, m_operands()
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

    m_operands.add( operand );

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

Values Instruction::operands( void ) const
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
           or AssertInstruction::classof( obj )
           or SelectInstruction::classof( obj )
           or SymbolicInstruction::classof( obj )
           or OperatorInstruction::classof( obj );
}

//
// Unary Instruction
//

UnaryInstruction::UnaryInstruction( Instruction* self )
: m_self( *self )
{
}

Value::Ptr UnaryInstruction::get( void ) const
{
    return m_self.operand( 0 );
}

u1 UnaryInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or ForkInstruction::classof( obj )
           or MergeInstruction::classof( obj )
           or LookupInstruction::classof( obj )
           or AssertInstruction::classof( obj )
           or InvInstruction::classof( obj ) or NotInstruction::classof( obj );
}

//
// Binary Instruction
//

BinaryInstruction::BinaryInstruction( Instruction* self )
: m_self( *self )
{
}

Value::Ptr BinaryInstruction::lhs( void ) const
{
    return m_self.operand( 0 );
}

Value::Ptr BinaryInstruction::rhs( void ) const
{
    return m_self.operand( 1 );
}

u1 BinaryInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or UpdateInstruction::classof( obj )
           or LocalInstruction::classof( obj ) or AddInstruction::classof( obj )
           or SubInstruction::classof( obj ) or MulInstruction::classof( obj )
           or DivInstruction::classof( obj ) or PowInstruction::classof( obj )
           or ModInstruction::classof( obj ) or EquInstruction::classof( obj )
           or NeqInstruction::classof( obj ) or LthInstruction::classof( obj )
           or LeqInstruction::classof( obj ) or GthInstruction::classof( obj )
           or GeqInstruction::classof( obj ) or OrInstruction::classof( obj )
           or XorInstruction::classof( obj ) or ImpInstruction::classof( obj )
           or AndInstruction::classof( obj );
}

//
// INSTRUCTION IMPLEMENTATIONS
//

//
// Skip Instruction
//

SkipInstruction::SkipInstruction( void )
: Instruction( libstdhl::get< VoidType >(), {}, classid() )
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
: Instruction( libstdhl::get< VoidType >(), {}, classid() )
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
: Instruction( libstdhl::get< VoidType >(), {}, classid() )
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
: Instruction( location->ptr_type()->ptr_result(), { location }, classid() )
, UnaryInstruction( this )
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
: Instruction( expr->ptr_type(), { location, expr }, classid() )
, BinaryInstruction( this )
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
: Instruction( expr->ptr_type(), { ident, expr }, classid() )
, BinaryInstruction( this )
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

LocationInstruction::LocationInstruction( const Value::Ptr& function )
: Instruction( function->ptr_type(), { function }, classid() )
{
    assert( isa< Function >( function ) );
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
: Instruction( symbol->ptr_type()->ptr_result(), { symbol }, classid() )
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
// Assert Instruction
//

AssertInstruction::AssertInstruction( const Value::Ptr& condition )
: Instruction( condition->ptr_type()->ptr_result(), { condition }, classid() )
, UnaryInstruction( this )
{
}

void AssertInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 AssertInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Select Instruction
//

SelectInstruction::SelectInstruction( const Value::Ptr& expression )
: Instruction( expression->ptr_type()->ptr_result(), { expression }, classid() )
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
// Symbolic Instruction
//

SymbolicInstruction::SymbolicInstruction( const Value::Ptr& symbol )
: Instruction( libstdhl::get< BooleanType >(), { symbol }, classid() )
{
    if( not isa< Function >( symbol ) )
    {
        throw std::domain_error(
            "symbol '" + symbol->name() + "' is not a Function" );
    }
}

void SymbolicInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 SymbolicInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Operator Instruction
//

OperatorInstruction::OperatorInstruction( const Type::Ptr& type,
    const std::vector< Value::Ptr >& operands, const Annotation& info,
    Value::ID id )
: Instruction( type, operands, id )
, Annotation( info )
{
    std::vector< const Type* > arguments;

    for( u32 c = 0; c < operands.size(); c++ )
    {
        arguments.push_back( &operand( c ).get()->type() );
    }

    m_resolved = resultTypeForRelation( arguments );
}

const Type::ID OperatorInstruction::resolved( void ) const
{
    return m_resolved;
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
    const std::vector< Value::Ptr >& operands, const Annotation& info,
    Value::ID id )
: OperatorInstruction(
      operands[ 0 ] ? operands[ 0 ]->ptr_type() : libstdhl::get< VoidType >(),
      operands, info, id )
{
    assert( operands.size() <= 2 );

    const auto& lhs_ty = operand( 0 )->type();
    if( operands.size() > 1 )
    {
        const auto& rhs_ty = operand( 1 )->type();
        assert( lhs_ty == rhs_ty );
    }

    assert( lhs_ty.id() == resolved() );
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

//
// Compare Instruction
//

CompareInstruction::CompareInstruction(
    const std::vector< Value::Ptr >& operands, const Annotation& info,
    Value::ID id )
: OperatorInstruction( libstdhl::get< BooleanType >(), operands, info, id )
{
}

u1 CompareInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or EquInstruction::classof( obj )
           or NeqInstruction::classof( obj ) or LthInstruction::classof( obj )
           or LeqInstruction::classof( obj ) or GthInstruction::classof( obj )
           or GeqInstruction::classof( obj );
}

//
// Logical Instruction
//

LogicalInstruction::LogicalInstruction(
    const std::vector< Value::Ptr >& operands, const Annotation& info,
    Value::ID id )
: OperatorInstruction(
      operands[ 0 ]
          ? ( operands[ 0 ]->type().isBit() ? operands[ 0 ]->ptr_type()
                                            : libstdhl::get< BooleanType >() )
          : libstdhl::get< VoidType >(),
      operands, info, id )
{
    assert( operands.size() <= 2 );

    const auto& lhs_ty = operands[ 0 ]->type();
    if( operands.size() > 1 )
    {
        const auto& rhs_ty = operands[ 1 ]->type();
        assert( lhs_ty == rhs_ty );
    }

    if( lhs_ty.isBit() )
    {
        assert( lhs_ty.id() == resolved() );
    }
}

u1 LogicalInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or OrInstruction::classof( obj )
           or ImpInstruction::classof( obj ) or XorInstruction::classof( obj )
           or AndInstruction::classof( obj ) or NotInstruction::classof( obj );
}

//
// Inv Instruction
//

InvInstruction::InvInstruction( const Value::Ptr& lhs )
: ArithmeticInstruction( { lhs }, info, classid() )
, UnaryInstruction( this )
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

        { Type::FLOATING,
            {
                Type::FLOATING,
            } },

        { Type::RATIONAL,
            {
                Type::RATIONAL,
            } },

    } );

u1 InvInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Add Instruction
//

AddInstruction::AddInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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
                Type::INTEGER, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } },

        { Type::STRING,
            {
                Type::STRING, Type::STRING,
            } },

    } );

u1 AddInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Sub Instruction
//

SubInstruction::SubInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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
                Type::INTEGER, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } },

    } );

u1 SubInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Mul Instruction
//

MulInstruction::MulInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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
                Type::INTEGER, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } },

    } );

u1 MulInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Mod Instruction
//

ModInstruction::ModInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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
                Type::INTEGER, Type::INTEGER,
            } },

    } );

u1 ModInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Div Instruction
//

DivInstruction::DivInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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
                Type::INTEGER, Type::INTEGER,
            } },

        { Type::RATIONAL,
            {
                Type::RATIONAL, Type::RATIONAL,
            } },

    } );

u1 DivInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Pow Instruction
//

PowInstruction::PowInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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
                Type::INTEGER, Type::INTEGER,
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

    } );

u1 PowInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// And Instruction
//

AndInstruction::AndInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 AndInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Xor Instruction
//

XorInstruction::XorInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 XorInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Or Instruction
//

OrInstruction::OrInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 OrInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Imp Instruction (Implies)
//

ImpInstruction::ImpInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 ImpInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Not Instruction
//

NotInstruction::NotInstruction( const Value::Ptr& lhs )
: LogicalInstruction( { lhs }, info, classid() )
, UnaryInstruction( this )
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

    } );

u1 NotInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Equ Instruction
//

EquInstruction::EquInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 EquInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Neq Instruction
//

NeqInstruction::NeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 NeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Lth Instruction
//

LthInstruction::LthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 LthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Leq Instruction
//

LeqInstruction::LeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 LeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Gth Instruction
//

GthInstruction::GthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

u1 GthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Geq Instruction
//

GeqInstruction::GeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
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

    } );

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

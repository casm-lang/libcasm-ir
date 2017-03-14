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

Instruction::Instruction( const std::string& name, const Type::Ptr& type,
    const std::vector< Value::Ptr >& operands, Value::ID id )
: User( name, type, id )
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

void Instruction::replace( const Value& from, const Value::Ptr& to )
{
    assert( !" TODO " );

    // libstdhl::Log::info( "replace: %s -> %s", from->label(), to->label() );

    // std::replace( m_values.begin(), m_values.end(), &from, &to );

    // if( isa< User >( from ) )
    // {
    //     libstdhl::Log::info( "replace-from: remove use of %s -> %s",
    //         this->label(), from.label() );
    //     User& user = static_cast< User& >( from );
    //     user.removeUse( *this );
    // }

    // if( isa< User >( to ) )
    // {
    //     libstdhl::Log::info( "replace-to: set use of %s", to.label() );
    //     User& user = static_cast< User& >( to );
    //     user.setUse( *this );
    // }
}

void Instruction::setStatement( const Statement::Ptr& statement )
{
    m_statement = statement;
}

Statement::Ptr Instruction::statement( void ) const
{
    return m_statement.lock();
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
           or NotInstruction::classof( obj );
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
           or DivInstruction::classof( obj ) or ModInstruction::classof( obj )
           or EquInstruction::classof( obj ) or NeqInstruction::classof( obj )
           or LthInstruction::classof( obj ) or LeqInstruction::classof( obj )
           or GthInstruction::classof( obj ) or GeqInstruction::classof( obj )
           or OrInstruction::classof( obj ) or XorInstruction::classof( obj )
           or AndInstruction::classof( obj );
}

//
// INSTRUCTION IMPLEMENTATIONS
//

//
// Skip Instruction
//

SkipInstruction::SkipInstruction( void )
: Instruction( "skip", libstdhl::get< VoidType >(), {}, classid() )
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
: Instruction( "fork", libstdhl::get< VoidType >(), {}, classid() )
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
: Instruction( "merge", libstdhl::get< VoidType >(), {}, classid() )
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
: Instruction(
      "lookup", location->ptr_type()->ptr_result(), { location }, classid() )
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
: Instruction( "update", expr->ptr_type(), { location, expr }, classid() )
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
: Instruction( "local", expr->ptr_type(), { ident, expr }, classid() )
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
: Instruction( "location", function->ptr_type(), { function }, classid() )
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
: Instruction( "call", symbol->ptr_type()->ptr_result(), { symbol }, classid() )
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
: Instruction(
      "assert", condition->ptr_type()->ptr_result(), { condition }, classid() )
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
: Instruction( "select", expression->ptr_type()->ptr_result(), { expression },
      classid() )
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
: Instruction(
      "symbolic", libstdhl::get< BooleanType >(), { symbol }, classid() )
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

OperatorInstruction::OperatorInstruction( const std::string& name,
    const Type::Ptr& type, const std::vector< Value::Ptr >& operands,
    const TypeAnnotation& info, Value::ID id )
: Instruction( name, type, operands, id )
, TypeAnnotation( info )
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

ArithmeticInstruction::ArithmeticInstruction( const std::string& name,
    const std::vector< Value::Ptr >& operands, const TypeAnnotation& info,
    Value::ID id )
: OperatorInstruction( name,
      operands[ 0 ] ? operands[ 0 ]->ptr_type() : libstdhl::get< VoidType >(),
      operands, info, id )
{
    assert( operands.size() == 2 );

    auto lhs_ty = operand( 0 )->type();
    auto rhs_ty = operand( 1 )->type();

    assert( lhs_ty == rhs_ty );
    assert( lhs_ty.id() == resolved() );
}

u1 ArithmeticInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or AddInstruction::classof( obj )
           or SubInstruction::classof( obj ) or MulInstruction::classof( obj )
           or DivInstruction::classof( obj ) or ModInstruction::classof( obj )
           or OrInstruction::classof( obj ) or XorInstruction::classof( obj )
           or AndInstruction::classof( obj ) or NotInstruction::classof( obj );
}

//
// Compare Instruction
//

CompareInstruction::CompareInstruction( const std::string& name,
    const std::vector< Value::Ptr >& operands, const TypeAnnotation& info,
    Value::ID id )
: OperatorInstruction(
      name, libstdhl::get< BooleanType >(), operands, info, id )
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

LogicalInstruction::LogicalInstruction( const std::string& name,
    const std::vector< Value::Ptr >& operands, const TypeAnnotation& info,
    Value::ID id )
: OperatorInstruction( name,
      operands[ 0 ]
          ? ( operands[ 0 ]->type().isBit() ? operands[ 0 ]->ptr_type()
                                            : libstdhl::get< BooleanType >() )
          : libstdhl::get< VoidType >(),
      operands, info, id )
{
    assert( operands.size() <= 2 );

    auto lhs_ty = operands[ 0 ]->type();
    if( operands.size() > 1 )
    {
        auto rhs_ty = operands[ 1 ]->type();
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
           or XorInstruction::classof( obj ) or AndInstruction::classof( obj )
           or NotInstruction::classof( obj );
}

//
// Add Instruction
//

AddInstruction::AddInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "add", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation AddInstruction::info( TypeAnnotation::Data{

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
        } }

} );

void AddInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 AddInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Sub Instruction
//

SubInstruction::SubInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "sub", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation SubInstruction::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::INTEGER, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );

void SubInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 SubInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Mul Instruction
//

MulInstruction::MulInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "mul", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation MulInstruction::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::INTEGER, Type::INTEGER,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );

void MulInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 MulInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Mod Instruction
//

ModInstruction::ModInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "mod", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation ModInstruction::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::INTEGER, Type::INTEGER,
        } }

} );

void ModInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 ModInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Div Instruction
//

DivInstruction::DivInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "div", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation DivInstruction::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::INTEGER, Type::INTEGER,
        } },
    { Type::RATIONAL,
        {
            Type::RATIONAL, Type::RATIONAL,
        } }

} );

void DivInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 DivInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// And Instruction
//

AndInstruction::AndInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( "and", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation AndInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN,
        {
            Type::BOOLEAN, Type::BOOLEAN,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );

void AndInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 AndInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Xor Instruction
//

XorInstruction::XorInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( "xor", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation XorInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN,
        {
            Type::BOOLEAN, Type::BOOLEAN,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );

void XorInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 XorInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Or Instruction
//

OrInstruction::OrInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction( "or", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation OrInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN,
        {
            Type::BOOLEAN, Type::BOOLEAN,
        } },
    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }

} );

void OrInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 OrInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Not Instruction
//

NotInstruction::NotInstruction( const Value::Ptr& lhs )
: LogicalInstruction( "not", { lhs }, info, classid() )
, UnaryInstruction( this )
{
}

const TypeAnnotation NotInstruction::info( TypeAnnotation::Data{

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
        } }

} );

void NotInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 NotInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Equ Instruction
//

EquInstruction::EquInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( "equ", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation EquInstruction::info( TypeAnnotation::Data{

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
        } }

} );

void EquInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 EquInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Neq Instruction
//

NeqInstruction::NeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( "neq", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation NeqInstruction::info( TypeAnnotation::Data{

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
        } }

} );

void NeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 NeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Lth Instruction
//

LthInstruction::LthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( "lth", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation LthInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN,
        {
            Type::INTEGER, Type::INTEGER,
        } }

} );

void LthInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 LthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Leq Instruction
//

LeqInstruction::LeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( "leq", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation LeqInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN,
        {
            Type::INTEGER, Type::INTEGER,
        } }

} );

void LeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 LeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Gth Instruction
//

GthInstruction::GthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( "gth", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation GthInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN,
        {
            Type::INTEGER, Type::INTEGER,
        } }

} );

void GthInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 GthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Geq Instruction
//

GeqInstruction::GeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( "geq", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}

const TypeAnnotation GeqInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN, { Type::INTEGER, Type::INTEGER } }

} );

void GeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

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

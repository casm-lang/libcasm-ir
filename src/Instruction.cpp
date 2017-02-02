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

#include "Derived.h"
#include "Function.h"
#include "Rule.h"

#include "../stdhl/cpp/Default.h"
#include "../stdhl/cpp/Log.h"

using namespace libcasm_ir;

Instruction::Instruction( const char* name, Type* type,
    const std::vector< Value* >& values, Value::ID id )
: User( name, type, id )
, m_statement( 0 )
, m_values()
{
    for( auto v : values )
    {
        add( v );
    }

    label();
}

void Instruction::setStatement( Statement* stmt )
{
    m_statement = stmt;

    for( auto value : m_values )
    {
        if( isa< Instruction >( value ) )
        {
            Instruction* instr = (Instruction*)value;
            if( instr->statement() == 0 )
            {
                stmt->add( ( value ) );
            }
            else if( instr->statement() != stmt )
            {
                fprintf( stderr,
                    "error: %s:%i: Instruction %p does belong to a different "
                    "Statement block\n",
                    __FUNCTION__, __LINE__, value );
                assert( 0 );
            }
        }
    }
}

const Statement* Instruction::statement( void ) const
{
    return m_statement;
}

void Instruction::add( Value* value )
{
    assert( value );

    if( ( isa< UnaryInstruction >( this ) and values().size() >= 1 )
        or ( isa< BinaryInstruction >( this ) and values().size() >= 2 ) )
    {
        assert(
            !" impossible to add more arguments to this instruction type " );
    }

    m_values.push_back( value );

    if( isa< User >( value ) )
    {
        User* user = static_cast< User* >( value );

        user->setUse( *this );
    }
}

Value* Instruction::value( u8 index ) const
{
    assert( index < m_values.size() );
    return m_values[ index ];
}

void Instruction::replace( Value& from, Value& to )
{
    libstdhl::Log::info( "replace: %s -> %s", from.label(), to.label() );

    std::replace( m_values.begin(), m_values.end(), &from, &to );

    if( isa< User >( from ) )
    {
        libstdhl::Log::info( "replace-from: remove use of %s -> %s",
            this->label(), from.label() );
        User& user = static_cast< User& >( from );
        user.removeUse( *this );
    }

    if( isa< User >( to ) )
    {
        libstdhl::Log::info( "replace-to: set use of %s", to.label() );
        User& user = static_cast< User& >( to );
        user.setUse( *this );
    }
}

const std::vector< Value* >& Instruction::values( void ) const
{
    return m_values;
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
           or PrintInstruction::classof( obj )
           or AssertInstruction::classof( obj )
           or SelectInstruction::classof( obj )
           or OperatorInstruction::classof( obj );
}

UnaryInstruction::UnaryInstruction( Instruction* self )
: m_self( *self )
{
}

Value* UnaryInstruction::get( void ) const
{
    return m_self.value( 0 );
}

u1 UnaryInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or ForkInstruction::classof( obj )
           or MergeInstruction::classof( obj )
           or LookupInstruction::classof( obj )
           or AssertInstruction::classof( obj )
           or NotInstruction::classof( obj );
}

BinaryInstruction::BinaryInstruction( Instruction* self )
: m_self( *self )
{
}

Value& BinaryInstruction::lhs( void ) const
{
    return *m_self.value( 0 );
}

Value& BinaryInstruction::rhs( void ) const
{
    return *m_self.value( 1 );
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

SkipInstruction::SkipInstruction( void )
: Instruction( "skip", 0, {}, classid() )
{
}

u1 SkipInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

ForkInstruction::ForkInstruction( void )
: Instruction( "fork", 0, {}, classid() )
{
}

u1 ForkInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

MergeInstruction::MergeInstruction( void )
: Instruction( "merge", 0, {}, classid() )
{
}

u1 MergeInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

LookupInstruction::LookupInstruction( Value* location )
: Instruction( "lookup", 0, { location }, classid() )
, UnaryInstruction( this )
{
    setType( location->type().result() );
}

u1 LookupInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

UpdateInstruction::UpdateInstruction( Value* location, Value* expr )
: Instruction( "update", 0, { location, expr }, classid() )
, BinaryInstruction( this )
{
    assert( &location->type() );
    assert( &expr->type() );
    setType( (Type*)&expr->type() );
}

u1 UpdateInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

LocalInstruction::LocalInstruction( Value* ident, Value* expr )
: Instruction( "local", 0, { ident, expr }, classid() )
, BinaryInstruction( this )
{
    assert( &expr->type() );
    setType( (Type*)&expr->type() );
}

u1 LocalInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

LocationInstruction::LocationInstruction( Value* function )
: Instruction( "location", 0, { function }, classid() )
{
    assert( isa< Function >( function ) );
    setType( (Type*)&function->type() );
}

u1 LocationInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

CallInstruction::CallInstruction( Value* symbol )
: Instruction( "call", 0, { symbol }, classid() )
{
    assert( isa< Rule >( symbol ) or isa< Derived >( symbol )
            or isa< Builtin >( symbol ) );

    setType( symbol->type().result() );
}

Value& CallInstruction::callee( void ) const
{
    return *value( 0 );
}

u1 CallInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

PrintInstruction::PrintInstruction( Value* channel )
: Instruction( "print", Type::String(), {}, classid() )
{
    if( channel )
    {
        assert( 0 && "debug channel not implemented yet!" );
    }
}

u1 PrintInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

AssertInstruction::AssertInstruction( Value* condition )
: Instruction( "assert", 0, { condition }, classid() )
, UnaryInstruction( this )
{
    setType( condition->type().result() );
}

u1 AssertInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

SelectInstruction::SelectInstruction( Value* expression )
: Instruction( "select", 0, { expression }, classid() )
{
    setType( expression->type().result() );
}

u1 SelectInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

OperatorInstruction::OperatorInstruction( const char* name, Type* type,
    std::vector< Value* > values, const TypeAnnotation& info, Value::ID id )
: Instruction( name, type, values, id )
, TypeAnnotation( info )
{
    std::vector< Type* > arguments;

    for( const auto& v : this->values() )
    {
        arguments.push_back( (Type*)&v->type() );
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

ArithmeticInstruction::ArithmeticInstruction( const char* name, Type* type,
    std::vector< Value* > values, const TypeAnnotation& info, Value::ID id )
: OperatorInstruction( name, type, values, info, id )
{
    assert( this->values().size() == 2 );

    Type& lhs_ty = value( 0 )->type();
    Type& rhs_ty = value( 1 )->type();

    if( &lhs_ty != &rhs_ty )
    {
        assert( strcmp( lhs_ty.name(), rhs_ty.name() ) == 0 );
    }

    assert( lhs_ty.id() == resolved() );

    setType( (Type*)&lhs_ty );
}

u1 ArithmeticInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or AddInstruction::classof( obj )
           or SubInstruction::classof( obj ) or MulInstruction::classof( obj )
           or DivInstruction::classof( obj ) or ModInstruction::classof( obj );
}

CompareInstruction::CompareInstruction( const char* name,
    std::vector< Value* > values, const TypeAnnotation& info, Value::ID id )
: OperatorInstruction( name, Type::Boolean(), values, info, id )
{
}

u1 CompareInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or EquInstruction::classof( obj )
           or NeqInstruction::classof( obj ) or LthInstruction::classof( obj )
           or LeqInstruction::classof( obj ) or GthInstruction::classof( obj )
           or GeqInstruction::classof( obj );
}

LogicalInstruction::LogicalInstruction( const char* name, Type* type,
    std::vector< Value* > values, const TypeAnnotation& info, Value::ID id )
: OperatorInstruction( name, type, values, info, id )
{
    assert( this->values().size() <= 2 );

    const Type& lhs_ty = value( 0 )->type();

    if( this->values().size() > 1 )
    {
        const Type& rhs_ty = value( 1 )->type();

        assert( lhs_ty.id() == rhs_ty.id() );
    }

    assert( lhs_ty.id() == resolved() );
    setType( (Type*)&lhs_ty );
}

u1 LogicalInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or OrInstruction::classof( obj )
           or XorInstruction::classof( obj ) or AndInstruction::classof( obj )
           or NotInstruction::classof( obj );
}

AddInstruction::AddInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "add", 0, { lhs, rhs }, info, classid() )
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
u1 AddInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

SubInstruction::SubInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "sub", 0, { lhs, rhs }, info, classid() )
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
u1 SubInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

MulInstruction::MulInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "mul", 0, { lhs, rhs }, info, classid() )
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
u1 MulInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

DivInstruction::DivInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "div", 0, { lhs, rhs }, info, classid() )
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
u1 DivInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

ModInstruction::ModInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "mod", 0, { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}
const TypeAnnotation ModInstruction::info( TypeAnnotation::Data{

    { Type::INTEGER,
        {
            Type::INTEGER, Type::INTEGER,
        } }

} );
u1 ModInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

EquInstruction::EquInstruction( Value* lhs, Value* rhs )
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
u1 EquInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

NeqInstruction::NeqInstruction( Value* lhs, Value* rhs )
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
u1 NeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

LthInstruction::LthInstruction( Value* lhs, Value* rhs )
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
u1 LthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

LeqInstruction::LeqInstruction( Value* lhs, Value* rhs )
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
u1 LeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

GthInstruction::GthInstruction( Value* lhs, Value* rhs )
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
u1 GthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

GeqInstruction::GeqInstruction( Value* lhs, Value* rhs )
: CompareInstruction( "geq", { lhs, rhs }, info, classid() )
, BinaryInstruction( this )
{
}
const TypeAnnotation GeqInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN, { Type::INTEGER, Type::INTEGER } }

} );
u1 GeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

OrInstruction::OrInstruction( Value* lhs, Value* rhs )
: LogicalInstruction( "or", 0, { lhs, rhs }, info, classid() )
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
u1 OrInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

XorInstruction::XorInstruction( Value* lhs, Value* rhs )
: LogicalInstruction( "xor", 0, { lhs, rhs }, info, classid() )
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
u1 XorInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

AndInstruction::AndInstruction( Value* lhs, Value* rhs )
: LogicalInstruction( "and", 0, { lhs, rhs }, info, classid() )
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
u1 AndInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

NotInstruction::NotInstruction( Value* lhs )
: LogicalInstruction( "not", 0, { lhs }, info, classid() )
, UnaryInstruction( this )
{
    const Type& ty = get()->type();

    if( ty.id() == Type::ID::BOOLEAN or ty.id() == Type::ID::BIT )
    {
        setType( (Type*)&ty );
    }
    else
    {
        assert( !" invalid type case for NOT instruction " );
    }
}
const TypeAnnotation NotInstruction::info( TypeAnnotation::Data{

    { Type::BOOLEAN,
        {
            Type::BOOLEAN,
        } },
    { Type::BIT,
        {
            Type::BIT,
        } }

} );
u1 NotInstruction::classof( Value const* obj )
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

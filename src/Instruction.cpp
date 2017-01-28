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

using namespace libcasm_ir;

Instruction::Instruction( const char* name, Type* type,
    const std::vector< Value* >& values, Value::ID id )
: User( name, type, id )
, statement( 0 )
, values( values )
{
    for( auto v : getValues() )
    {
        assert( v );
    }
}

void Instruction::setStatement( Statement* stmt )
{
    statement = stmt;

    for( auto value : values )
    {
        if( isa< Instruction >( value ) )
        {
            Instruction* instr = (Instruction*)value;
            if( instr->getStatement() == 0 )
            {
                stmt->add( ( value ) );
            }
            else if( instr->getStatement() != stmt )
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

const Statement* Instruction::getStatement( void ) const
{
    // assert( statement );
    return statement;
}

void Instruction::add( Value* value )
{
    assert( value );

    if( isa< UnaryInstruction >( this ) or isa< BinaryInstruction >( this ) )
    {
        assert(
            !" impossible to add more arguments to this instruction type " );
    }

    values.push_back( value );
}

Value* Instruction::getValue( u8 index ) const
{
    assert( index < values.size() );
    return values[ index ];
}

const std::vector< Value* >& Instruction::getValues( void ) const
{
    return values;
}

u1 Instruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or SkipInstruction::classof( obj )
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
: self( *self )
{
}

Value* UnaryInstruction::get( void ) const
{
    return self.getValue( 0 );
}

u1 UnaryInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or ForkInstruction::classof( obj )
           or MergeInstruction::classof( obj )
           or LookupInstruction::classof( obj )
           or AssertInstruction::classof( obj )
           or NotInstruction::classof( obj );
}

BinaryInstruction::BinaryInstruction( Instruction* self )
: self( *self )
{
}

Value* BinaryInstruction::getLHS( void ) const
{
    return self.getValue( 0 );
}

Value* BinaryInstruction::getRHS( void ) const
{
    return self.getValue( 1 );
}

u1 BinaryInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or UpdateInstruction::classof( obj )
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
    return obj->getValueID() == classid();
}

ForkInstruction::ForkInstruction( void )
: Instruction( "fork", 0, {}, classid() )
{
}

u1 ForkInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

MergeInstruction::MergeInstruction( void )
: Instruction( "merge", 0, {}, classid() )
{
}

u1 MergeInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

LookupInstruction::LookupInstruction( Value* location )
: Instruction( "lookup", 0, { location }, classid() )
, UnaryInstruction( this )
{
    assert( location->getType() );
    setType( location->getType()->getResult() );
}

u1 LookupInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

UpdateInstruction::UpdateInstruction( Value* location, Value* expr )
: Instruction( "update", 0, { location, expr }, classid() )
, BinaryInstruction( this )
{
    assert( location->getType() );
    assert( expr->getType() );
    setType( expr->getType() );
}

u1 UpdateInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

LocalInstruction::LocalInstruction( Value* ident, Value* expr )
: Instruction( "local", 0, { ident, expr }, classid() )
, BinaryInstruction( this )
{
    assert( expr->getType() );
    setType( expr->getType() );
}

u1 LocalInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

LocationInstruction::LocationInstruction( Value* function )
: Instruction( "location", 0, { function }, classid() )
{
    assert( function->getType() and isa< Function >( function ) );
    setType( function->getType() );
}

u1 LocationInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

CallInstruction::CallInstruction( Value* symbol )
: Instruction( "call", 0, { symbol }, classid() )
{
    assert( isa< Rule >( symbol ) or isa< Derived >( symbol )
            or isa< Builtin >( symbol ) );

    assert( symbol->getType() );
    setType( symbol->getType()->getResult() );
}

u1 CallInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

PrintInstruction::PrintInstruction( Value* channel )
: Instruction( "print", Type::getString(), {}, classid() )
{
    if( channel )
    {
        assert( 0 && "debug channel not implemented yet!" );
    }
}

u1 PrintInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

AssertInstruction::AssertInstruction( Value* condition )
: Instruction( "assert", 0, { condition }, classid() )
, UnaryInstruction( this )
{
    assert( condition->getType() );
    setType( condition->getType()->getResult() );
}

u1 AssertInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

SelectInstruction::SelectInstruction( Value* expression )
: Instruction( "select", 0, { expression }, classid() )
{
    assert( expression->getType() );
    setType( expression->getType()->getResult() );
}

u1 SelectInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

OperatorInstruction::OperatorInstruction( const char* name, Type* type,
    std::vector< Value* > values, const TypeAnnotation& info, Value::ID id )
: Instruction( name, type, values, id )
, TypeAnnotation( info )
{
    std::vector< Type* > arguments;

    for( const auto& v : getValues() )
    {
        assert( v->getType() );
        arguments.push_back( v->getType() );
    }

    resolved = getResultTypeForRelation( arguments );
}

const Type::ID OperatorInstruction::getResolved( void ) const
{
    return resolved;
}

u1 OperatorInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid()
           or ArithmeticInstruction::classof( obj )
           or CompareInstruction::classof( obj )
           or LogicalInstruction::classof( obj );
}

ArithmeticInstruction::ArithmeticInstruction( const char* name, Type* type,
    std::vector< Value* > values, const TypeAnnotation& info, Value::ID id )
: OperatorInstruction( name, type, values, info, id )
{
    assert( getValues().size() == 2 );

    Type* lhs_ty = getValue( 0 )->getType();
    Type* rhs_ty = getValue( 1 )->getType();

    if( lhs_ty != rhs_ty )
    {
        assert( strcmp( lhs_ty->getName(), rhs_ty->getName() ) == 0 );
    }

    assert( lhs_ty->getID() == getResolved() );

    setType( lhs_ty );
}

u1 ArithmeticInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or AddInstruction::classof( obj )
           or SubInstruction::classof( obj ) or MulInstruction::classof( obj )
           or DivInstruction::classof( obj ) or ModInstruction::classof( obj );
}

CompareInstruction::CompareInstruction( const char* name,
    std::vector< Value* > values, const TypeAnnotation& info, Value::ID id )
: OperatorInstruction( name, Type::getBoolean(), values, info, id )
{
}

u1 CompareInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or EquInstruction::classof( obj )
           or NeqInstruction::classof( obj ) or LthInstruction::classof( obj )
           or LeqInstruction::classof( obj ) or GthInstruction::classof( obj )
           or GeqInstruction::classof( obj );
}

LogicalInstruction::LogicalInstruction( const char* name, Type* type,
    std::vector< Value* > values, const TypeAnnotation& info, Value::ID id )
: OperatorInstruction( name, type, values, info, id )
{
    assert( getValues().size() <= 2 );

    Type* lhs_ty = getValue( 0 )->getType();

    if( getValues().size() > 1 )
    {
        Type* rhs_ty = getValue( 1 )->getType();

        assert( lhs_ty->getID() == rhs_ty->getID() );
    }

    assert( lhs_ty->getID() == getResolved() );
    setType( lhs_ty );
}

u1 LogicalInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or OrInstruction::classof( obj )
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
}

NotInstruction::NotInstruction( Value* lhs )
: LogicalInstruction( "not", 0, { lhs }, info, classid() )
, UnaryInstruction( this )
{
    Type* ty = get()->getType();
    assert( get()->getType() );

    if( ty->getID() == Type::ID::BOOLEAN or ty->getID() == Type::ID::BIT )
    {
        setType( ty );
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
    return obj->getValueID() == classid();
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

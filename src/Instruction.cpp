//
//  Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
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
//  Additional permission under GNU GPL version 3 section 7
//
//  libcasm-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcasm-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcasm-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcasm-ir give you permission to link libcasm-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcasm-ir. If you modify libcasm-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "Instruction.h"

#include "Builtin.h"
#include "Derived.h"
#include "Exception.h"
#include "Function.h"
#include "Rule.h"
#include "Statement.h"

using namespace libcasm_ir;

static const auto VOID = libstdhl::Memory::get< VoidType >();
static const auto BOOLEAN = libstdhl::Memory::get< BooleanType >();
static const auto INTEGER = libstdhl::Memory::get< IntegerType >();

//
// Instruction
//

Instruction::Instruction(
    const Type::Ptr& type, const Value::ID id, const std::vector< Value::Ptr >& operands )
: User( type, id )
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
        throw std::domain_error( "impossible to add more arguments to unary instruction" );
    }
    else if( isa< BinaryInstruction >( this ) and m_operands.size() >= 2 )
    {
        throw std::domain_error( "impossible to add more arguments to binary instruction" );
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
        throw std::domain_error(
            "instruction operand position '" + std::to_string( position ) + "' does not exist!" );
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

    std::replace_if(
        m_operands.begin(),
        m_operands.end(),
        [&]( const Value::Ptr& v ) { return *v.get() == from; },
        to );

    if( isa< User >( from ) )
    {
        std::cerr << "replace-from: remove use of " << this->label() << " -> " << from.label()
                  << "\n";

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
    return libstdhl::Hash::combine( classid(), std::hash< std::string >()( name() ) );
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
    return obj->id() == classid() or SkipInstruction::classof( obj ) or
           ForkInstruction::classof( obj ) or MergeInstruction::classof( obj ) or
           LookupInstruction::classof( obj ) or UpdateInstruction::classof( obj ) or
           LocalInstruction::classof( obj ) or LocationInstruction::classof( obj ) or
           CallInstruction::classof( obj ) or SelectInstruction::classof( obj ) or
           OperatorInstruction::classof( obj );
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

        if( NotInstruction::classof( obj ) or InvInstruction::classof( obj ) )
        {
            return true;
        }
        else
        {
            return false;
        }
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

        if( AddInstruction::classof( obj ) or SubInstruction::classof( obj ) or
            MulInstruction::classof( obj ) or DivInstruction::classof( obj ) or
            PowInstruction::classof( obj ) or ModInstruction::classof( obj ) or
            OrInstruction::classof( obj ) or XorInstruction::classof( obj ) or
            AndInstruction::classof( obj ) or ImpInstruction::classof( obj ) or
            CompareInstruction::classof( obj ) )
        {
            return true;
        }
        else
        {
            return false;
        }
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
: Instruction( location->type().ptr_result(), classid(), { location } )
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

UpdateInstruction::UpdateInstruction( const Value::Ptr& location, const Value::Ptr& expr )
: Instruction( expr->type().ptr_type(), classid(), { location, expr } )
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

LocalInstruction::LocalInstruction( const Value::Ptr& ident, const Value::Ptr& expr )
: Instruction( expr->type().ptr_type(), classid(), { ident, expr } )
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
: Instruction( function->type().ptr_type(), classid(), { function } )
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
: Instruction( symbol->type().ptr_result(), classid(), { symbol } )
{
    assert( isa< Rule >( symbol ) or isa< Derived >( symbol ) or isa< Builtin >( symbol ) );

    for( auto operand : operands )
    {
        add( operand );
    }
}

CallInstruction::CallInstruction( const Type::Ptr& type )
: Instruction( type, classid() )
{
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

OperatorInstruction::OperatorInstruction(
    const Type::Ptr& type, const Value::ID id, const std::vector< Value::Ptr >& operands )
: Instruction( type, id, operands )
{
}

u1 OperatorInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or ArithmeticInstruction::classof( obj ) or
           CompareInstruction::classof( obj ) or LogicalInstruction::classof( obj );
}

//
// Arithmetic Instruction
//

ArithmeticInstruction::ArithmeticInstruction(
    const Type::Ptr& type, const Value::ID id, const std::vector< Value::Ptr >& operands )
: OperatorInstruction( type, id, operands )
{
    assert( operands.size() <= 2 );
    if( operands.size() > 0 )
    {
        assert( this->type() == operands[ 0 ]->type() );
    }
}

u1 ArithmeticInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or InvInstruction::classof( obj ) or
           AddInstruction::classof( obj ) or SubInstruction::classof( obj ) or
           MulInstruction::classof( obj ) or DivInstruction::classof( obj ) or
           PowInstruction::classof( obj ) or ModInstruction::classof( obj ) or
           OrInstruction::classof( obj ) or XorInstruction::classof( obj ) or
           AndInstruction::classof( obj );
}

static const Properties arithmetic_instruction_properties = { Property::CONSTANT, Property::PURE };

static const auto arithmetic_instruction_resolve = []( std::vector< Type::Ptr >& types ) {
    if( types.size() != 2 )
    {
        throw InternalException( "types.size() != 2" );
    }

    if( not types[ 0 ] )
    {
        types[ 0 ] = types[ 1 ];
    }
    else if( not types[ 1 ] )
    {
        types[ 1 ] = types[ 0 ];
    }
};

static const auto arithmetic_instruction_validate = []( const RelationType& type ) -> u1 {
    if( type.arguments().size() == 1 )
    {
        return type.result() == *type.arguments()[ 0 ];
    }
    else if( type.arguments().size() == 2 )
    {
        if( type.result().isInteger() and type.arguments()[ 0 ]->isInteger() and
            type.arguments()[ 1 ]->isInteger() )
        {
            return true;
        }

        return type.result() == *type.arguments()[ 0 ] and type.result() == *type.arguments()[ 1 ];
    }
    else
    {
        return false;
    }
};

//
// Inv Instruction
//

InvInstruction::InvInstruction( const Value::Ptr& lhs )
: ArithmeticInstruction( lhs->type().ptr_type(), classid(), { lhs } )
{
}

InvInstruction::InvInstruction( const Type::Ptr& type )
: ArithmeticInstruction( type->ptr_result(), classid() )
{
}

void InvInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation InvInstruction::annotation(
    classid(),
    arithmetic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
          } },

        { Type::Kind::DECIMAL,
          {
              Type::Kind::DECIMAL,
          } },

        { Type::Kind::RATIONAL,
          {
              Type::Kind::RATIONAL,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }

        const auto& lhs = types[ 0 ];

        if( lhs->isInteger() )
        {
            return INTEGER;
        }

        return lhs;
    },
    arithmetic_instruction_validate );

u1 InvInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Add Instruction
//

AddInstruction::AddInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( lhs->type().ptr_type(), classid(), { lhs, rhs } )
{
}

AddInstruction::AddInstruction( const Type::Ptr& type )
: ArithmeticInstruction( type->ptr_result(), classid() )
{
}

void AddInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation AddInstruction::annotation(
    classid(),
    arithmetic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,  // signed integer add no wrap
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY, Type::Kind::BINARY,  // unsigned binary add with wrap
          } },

        { Type::Kind::RATIONAL,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,  // signed rational add no wrap
          } },

        { Type::Kind::DECIMAL,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,  // signed decimal add no wrap
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::STRING, Type::Kind::STRING,  // concatenation
          } },

    },
    arithmetic_instruction_resolve,
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( lhs->isInteger() and rhs->isInteger() )
        {
            return INTEGER;
        }

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        return lhs;
    },
    arithmetic_instruction_validate );

u1 AddInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Sub Instruction
//

SubInstruction::SubInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( lhs->type().ptr_type(), classid(), { lhs, rhs } )
{
}

SubInstruction::SubInstruction( const Type::Ptr& type )
: ArithmeticInstruction( type->ptr_result(), classid() )
{
}

void SubInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation SubInstruction::annotation(
    classid(),
    arithmetic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,  // signed integer sub no wrap
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY, Type::Kind::BINARY,  // unsigned binary sub with wrap
          } },

        { Type::Kind::RATIONAL,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,  // signed rational sub no wrap
          } },

        { Type::Kind::DECIMAL,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,  // signed decimal sub no wrap
          } },

    },
    arithmetic_instruction_resolve,
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( lhs->isInteger() and rhs->isInteger() )
        {
            return INTEGER;
        }

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        return lhs;
    },
    arithmetic_instruction_validate );

u1 SubInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Mul Instruction
//

MulInstruction::MulInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( lhs->type().ptr_type(), classid(), { lhs, rhs } )
{
}

MulInstruction::MulInstruction( const Type::Ptr& type )
: ArithmeticInstruction( type->ptr_result(), classid() )
{
}

void MulInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation MulInstruction::annotation(
    classid(),
    arithmetic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,  // signed integer mul no wrap
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,  // unsigned integer mul with wrap
          } },

        { Type::Kind::RATIONAL,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,  // signed rational mul no wrap
          } },

        { Type::Kind::DECIMAL,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,  // signed decimal mul no wrap
          } },

    },
    arithmetic_instruction_resolve,
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( lhs->isInteger() and rhs->isInteger() )
        {
            return INTEGER;
        }

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        return lhs;
    },
    arithmetic_instruction_validate );

u1 MulInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Mod Instruction
//

ModInstruction::ModInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( lhs->type().ptr_type(), classid(), { lhs, rhs } )
{
}

ModInstruction::ModInstruction( const Type::Ptr& type )
: ArithmeticInstruction( type->ptr_result(), classid() )
{
}

void ModInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation ModInstruction::annotation(
    classid(),
    arithmetic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,  // signed integer mod no wrap
          } },

    },
    arithmetic_instruction_resolve,
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( lhs->isInteger() and rhs->isInteger() )
        {
            return INTEGER;
        }

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        return lhs;
    },
    arithmetic_instruction_validate );

u1 ModInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Div Instruction
//

DivInstruction::DivInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( lhs->type().ptr_type(), classid(), { lhs, rhs } )
{
}

DivInstruction::DivInstruction( const Type::Ptr& type )
: ArithmeticInstruction( type->ptr_result(), classid() )
{
}

void DivInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation DivInstruction::annotation(
    classid(),
    arithmetic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,  // signed integer div no wrap
          } },

        { Type::Kind::RATIONAL,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,  // signed rational div no wrap
          } },

    },
    arithmetic_instruction_resolve,
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( lhs->isInteger() and rhs->isInteger() )
        {
            return INTEGER;
        }

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        return lhs;
    },
    arithmetic_instruction_validate );

u1 DivInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Pow Instruction
//

PowInstruction::PowInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( lhs->type().ptr_type(), classid(), { lhs, rhs } )
{
}

PowInstruction::PowInstruction( const Type::Ptr& type )
: ArithmeticInstruction( type->ptr_result(), classid() )
{
}

void PowInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation PowInstruction::annotation(
    classid(),
    arithmetic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,  // signed integer pow no wrap
          } },

        // { Type::Kind::RATIONAL, // TODO: PPA: enable this after clear
        // semantics
        //     {
        //         Type::Kind::RATIONAL, Type::Kind::INTEGER,
        //     } },

        // { Type::Kind::DECIMAL, // TODO: PPA: enable this after clear
        // semantics
        //     {
        //         Type::Kind::DECIMAL, Type::Kind::DECIMAL,
        //     } },

        { Type::Kind::DECIMAL,
          {
              Type::Kind::DECIMAL,
              Type::Kind::INTEGER,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        if( not types[ 1 ] )
        {
            types[ 1 ] = INTEGER;
        }
    },
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( lhs->isInteger() and rhs->isInteger() )
        {
            return INTEGER;
        }

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        return lhs;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 2 )
        {
            if( type.result().isInteger() and type.arguments()[ 0 ]->isInteger() and
                type.arguments()[ 1 ]->isInteger() )
            {
                return true;
            }

            return type.result() == *type.arguments()[ 0 ] and type.arguments()[ 1 ]->isInteger();
        }
        else
        {
            return false;
        }
    } );

u1 PowInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Logical Instruction
//

LogicalInstruction::LogicalInstruction(
    const Type::Ptr& type, const Value::ID id, const std::vector< Value::Ptr >& operands )
: OperatorInstruction( type, id, operands )
{
    assert( operands.size() <= 2 );
    if( operands.size() > 0 )
    {
        assert(
            ( operands[ 0 ]->type().isBinary() and this->type().isBinary() ) or
            ( not operands[ 0 ]->type().isBinary() and this->type().isBoolean() ) );
    }
}

u1 LogicalInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or OrInstruction::classof( obj ) or
           ImpInstruction::classof( obj ) or XorInstruction::classof( obj ) or
           AndInstruction::classof( obj ) or NotInstruction::classof( obj );
}

static const auto logic_instruction_properties = arithmetic_instruction_properties;

static const auto logic_instruction_resolve = arithmetic_instruction_resolve;

static const auto logic_instruction_validate = arithmetic_instruction_validate;

//
// And Instruction
//

AndInstruction::AndInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction(
      lhs->type().isBinary() ? lhs->type().ptr_type() : BOOLEAN, classid(), { lhs, rhs } )
{
}

AndInstruction::AndInstruction( const Type::Ptr& type )
: LogicalInstruction( type->ptr_result(), classid() )
{
}

void AndInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation AndInstruction::annotation(
    classid(),
    logic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

    },
    logic_instruction_resolve,
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        if( lhs->isBinary() )
        {
            return lhs;
        }
        else
        {
            return BOOLEAN;
        }
    },
    logic_instruction_validate );

u1 AndInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Xor Instruction
//

XorInstruction::XorInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction(
      lhs->type().isBinary() ? lhs->type().ptr_type() : BOOLEAN, classid(), { lhs, rhs } )
{
}

XorInstruction::XorInstruction( const Type::Ptr& type )
: LogicalInstruction( type->ptr_result(), classid() )
{
}

void XorInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation XorInstruction::annotation(
    classid(),
    logic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

    },
    logic_instruction_resolve,
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        if( lhs->isBinary() )
        {
            return lhs;
        }
        else
        {
            return BOOLEAN;
        }
    },
    logic_instruction_validate );

u1 XorInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Or Instruction
//

OrInstruction::OrInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction(
      lhs->type().isBinary() ? lhs->type().ptr_type() : BOOLEAN, classid(), { lhs, rhs } )
{
}

OrInstruction::OrInstruction( const Type::Ptr& type )
: LogicalInstruction( type->ptr_result(), classid() )
{
}

void OrInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation OrInstruction::annotation(
    classid(),
    logic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

    },
    logic_instruction_resolve,
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        if( lhs->isBinary() )
        {
            return lhs;
        }
        else
        {
            return BOOLEAN;
        }
    },
    logic_instruction_validate );

u1 OrInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Imp Instruction (Implies)
//

ImpInstruction::ImpInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: LogicalInstruction(
      lhs->type().isBinary() ? lhs->type().ptr_type() : BOOLEAN, classid(), { lhs, rhs } )
{
}

ImpInstruction::ImpInstruction( const Type::Ptr& type )
: LogicalInstruction( type->ptr_result(), classid() )
{
}

void ImpInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation ImpInstruction::annotation(
    classid(),
    logic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
              Type::Kind::BOOLEAN,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        if( not types[ 0 ] )
        {
            types[ 0 ] = BOOLEAN;
        }

        if( not types[ 1 ] )
        {
            types[ 1 ] = BOOLEAN;
        }
    },
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];

        if( *lhs != *rhs )
        {
            return nullptr;
        }

        if( lhs->isBinary() )
        {
            return lhs;
        }
        else
        {
            return BOOLEAN;
        }
    },
    logic_instruction_validate );

u1 ImpInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Not Instruction
//

NotInstruction::NotInstruction( const Value::Ptr& lhs )
: LogicalInstruction(
      lhs->type().isBinary() ? lhs->type().ptr_type() : BOOLEAN, classid(), { lhs } )
{
}

NotInstruction::NotInstruction( const Type::Ptr& type )
: LogicalInstruction( type->ptr_result(), classid() )
{
}

void NotInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation NotInstruction::annotation(
    classid(),
    logic_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
          } },

    },
    []( const std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }

        const auto& lhs = types[ 0 ];

        if( lhs->isBinary() )
        {
            return lhs;
        }
        else
        {
            return BOOLEAN;
        }
    },
    logic_instruction_validate );

u1 NotInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Compare Instruction
//

CompareInstruction::CompareInstruction(
    const Type::Ptr& type, const Value::ID id, const std::vector< Value::Ptr >& operands )
: OperatorInstruction( type, id, operands )
{
    assert( operands.size() == 0 or operands.size() == 2 );
    assert( this->type().isBoolean() );
}

u1 CompareInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or EquInstruction::classof( obj ) or
           NeqInstruction::classof( obj ) or LthInstruction::classof( obj ) or
           LeqInstruction::classof( obj ) or GthInstruction::classof( obj ) or
           GeqInstruction::classof( obj );
}

static const auto compare_instruction_properties = arithmetic_instruction_properties;

static const auto compare_instruction_resolve = arithmetic_instruction_resolve;

static auto compare_instruction_inference = []( const std::vector< Type::Ptr >& types,
                                                const std::vector< Value::Ptr >& ) -> Type::Ptr {
    if( types.size() != 2 )
    {
        throw InternalException( "types.size() != 2" );
    }
    const auto& lhs = types[ 0 ];
    const auto& rhs = types[ 1 ];

    if( *lhs != *rhs )
    {
        if( lhs->isInteger() and rhs->isInteger() )
        {
            // relaxation: if mixed ranged and non-ranged integer are compared,
            //             the inference shall return a Boolean type
        }
        else
        {
            return nullptr;
        }
    }

    return BOOLEAN;
};

static const auto compare_instruction_validate = []( const RelationType& type ) -> u1 {
    if( type.arguments().size() == 2 )
    {
        return type.result().isBoolean() and
               ( *type.arguments()[ 0 ] == *type.arguments()[ 1 ] or
                 ( type.arguments()[ 0 ]->isInteger() and type.arguments()[ 1 ]->isInteger() ) );
    }
    else
    {
        return false;
    }
};

//
// Equ Instruction
//

EquInstruction::EquInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( BOOLEAN, classid(), { lhs, rhs } )
{
}

EquInstruction::EquInstruction( const Type::Ptr& type )
: CompareInstruction( BOOLEAN, classid() )
{
    assert( type->isBoolean() );
}

void EquInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation EquInstruction::annotation(
    classid(),
    compare_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::STRING,
              Type::Kind::STRING,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::ENUMERATION,
              Type::Kind::ENUMERATION,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RANGE,
              Type::Kind::RANGE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::TUPLE,
              Type::Kind::TUPLE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::LIST,
              Type::Kind::LIST,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RULE_REFERENCE,
              Type::Kind::RULE_REFERENCE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::FUNCTION_REFERENCE,
              Type::Kind::FUNCTION_REFERENCE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::FILE,
              Type::Kind::FILE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::PORT,
              Type::Kind::PORT,
          } },

    },
    compare_instruction_resolve,
    compare_instruction_inference,
    compare_instruction_validate );

u1 EquInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Neq Instruction
//

NeqInstruction::NeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( BOOLEAN, classid(), { lhs, rhs } )
{
}

NeqInstruction::NeqInstruction( const Type::Ptr& type )
: CompareInstruction( BOOLEAN, classid() )
{
    assert( type->isBoolean() );
}

void NeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation NeqInstruction::annotation(
    classid(),
    compare_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::STRING,
              Type::Kind::STRING,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::ENUMERATION,
              Type::Kind::ENUMERATION,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RANGE,
              Type::Kind::RANGE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::TUPLE,
              Type::Kind::TUPLE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::LIST,
              Type::Kind::LIST,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RULE_REFERENCE,
              Type::Kind::RULE_REFERENCE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::FUNCTION_REFERENCE,
              Type::Kind::FUNCTION_REFERENCE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::FILE,
              Type::Kind::FILE,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::PORT,
              Type::Kind::PORT,
          } },

    },
    compare_instruction_resolve,
    compare_instruction_inference,
    compare_instruction_validate );

u1 NeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Lth Instruction
//

LthInstruction::LthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( BOOLEAN, classid(), { lhs, rhs } )
{
}

LthInstruction::LthInstruction( const Type::Ptr& type )
: CompareInstruction( BOOLEAN, classid() )
{
    assert( type->isBoolean() );
}

void LthInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation LthInstruction::annotation(
    classid(),
    compare_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,
          } },

    },
    compare_instruction_resolve,
    compare_instruction_inference,
    compare_instruction_validate );

u1 LthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Leq Instruction
//

LeqInstruction::LeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( BOOLEAN, classid(), { lhs, rhs } )
{
}

LeqInstruction::LeqInstruction( const Type::Ptr& type )
: CompareInstruction( BOOLEAN, classid() )
{
    assert( type->isBoolean() );
}

void LeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation LeqInstruction::annotation(
    classid(),
    compare_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,
          } },

    },
    compare_instruction_resolve,
    compare_instruction_inference,
    compare_instruction_validate );

u1 LeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Gth Instruction
//

GthInstruction::GthInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( BOOLEAN, classid(), { lhs, rhs } )
{
}

GthInstruction::GthInstruction( const Type::Ptr& type )
: CompareInstruction( BOOLEAN, classid() )
{
    assert( type->isBoolean() );
}

void GthInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation GthInstruction::annotation(
    classid(),
    compare_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,
          } },

    },
    compare_instruction_resolve,
    compare_instruction_inference,
    compare_instruction_validate );

u1 GthInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Geq Instruction
//

GeqInstruction::GeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( BOOLEAN, classid(), { lhs, rhs } )
{
}

GeqInstruction::GeqInstruction( const Type::Ptr& type )
: CompareInstruction( BOOLEAN, classid() )
{
    assert( type->isBoolean() );
}

void GeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

const Annotation GeqInstruction::annotation(
    classid(),
    compare_instruction_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::INTEGER,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RATIONAL,
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::DECIMAL,
              Type::Kind::DECIMAL,
          } },

    },
    compare_instruction_resolve,
    compare_instruction_inference,
    compare_instruction_validate );

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

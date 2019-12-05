//
//  Copyright (C) 2015-2021 CASM Organization <https://casm-lang.org>
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

#include <initializer_list>
#include <libcasm-ir/Builtin>
#include <libcasm-ir/Constant>
#include <libcasm-ir/Derived>
#include <libcasm-ir/Exception>
#include <libcasm-ir/Function>
#include <libcasm-ir/Rule>
#include <libcasm-ir/Statement>

#include <libtptp/Atom>
#include <libtptp/Identifier>
#include <libtptp/Literal>
#include <libtptp/Type>
#include "libstdhl/Optional.h"

using namespace libcasm_ir;
namespace TPTP = libtptp;

static const auto VOID = libstdhl::Memory::get< VoidType >();
static const auto BOOLEAN = libstdhl::Memory::get< BooleanType >();
static const auto INTEGER = libstdhl::Memory::get< IntegerType >();

SymbolicConstant symbolicInstruction(
    const Constant& lhs,
    const Constant& rhs,
    const std::function< TPTP::Logic::Ptr(
        SymbolicExecutionEnvironment& env,
        const TPTP::Atom::Ptr&,
        const TPTP::Atom::Ptr&,
        const TPTP::Atom::Ptr& ) > callback,
    const libstdhl::Optional< Type::Ptr > resType = {} )
{
    auto& env = lhs.symbolic() ? static_cast< const SymbolicConstant& >( lhs ).environment()
                               : static_cast< const SymbolicConstant& >( rhs ).environment();
    SymbolicConstant localRes(
        resType ? *resType : lhs.type().ptr_result(), env.generateSymbolName(), env );
    if( lhs.defined() and rhs.defined() )
    {
        auto lhsSym = env.tptpAtomFromConstant( lhs );
        auto rhsSym = env.tptpAtomFromConstant( rhs );

        auto resSym =
            std::make_shared< TPTP::ConstantAtom >( localRes.name(), TPTP::Atom::Kind::PLAIN );

        const auto atom = callback( env, lhsSym, rhsSym, resSym );
        env.addFormula( atom );
    }

    return localRes;
}

SymbolicConstant symbolicArithmeticInstruction(
    const Constant& lhs, const Constant& rhs, const Value& value )
{
    assert( ArithmeticInstruction::classof( &value ) );
    return symbolicInstruction(
        lhs, rhs, [ & ]( auto& env, const auto& lhsSym, const auto& rhsSym, const auto& resSym ) {
            return std::make_shared< TPTP::FunctorAtom >(
                env.generateOperatorFunction( value ),
                std::initializer_list< TPTP::Logic::Ptr >{ lhsSym, rhsSym, resSym },
                TPTP::Atom::Kind::PLAIN );
        } );
}

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
        [ & ]( const Value::Ptr& v ) { return *v.get() == from; },
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
    return obj->id() == classid()                  //
           or SkipInstruction::classof( obj )      //
           or ForkInstruction::classof( obj )      //
           or MergeInstruction::classof( obj )     //
           or LookupInstruction::classof( obj )    //
           or UpdateInstruction::classof( obj )    //
           or LocalInstruction::classof( obj )     //
           or LocationInstruction::classof( obj )  //
           or CallInstruction::classof( obj )      //
           or SelectInstruction::classof( obj )    //
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

        if( NotInstruction::classof( obj )  //
            or InvInstruction::classof( obj ) )
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

        if( AddInstruction::classof( obj )     //
            or SubInstruction::classof( obj )  //
            or MulInstruction::classof( obj )  //
            or DivInstruction::classof( obj )  //
            or PowInstruction::classof( obj )  //
            or ModInstruction::classof( obj )  //
            or OrInstruction::classof( obj )   //
            or XorInstruction::classof( obj )  //
            or AndInstruction::classof( obj )  //
            or ImpInstruction::classof( obj )  //
            or CompareInstruction::classof( obj ) )
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
// N-ary Instruction
//

u1 NaryInstruction::classof( Value const* obj )
{
    return isa< LocationInstruction >( obj )  //
           or isa< CallInstruction >( obj );
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

void CallInstruction::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

u1 CallInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
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
//
// Self Instruction
//

SelfInstruction::SelfInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: Instruction( lhs->type().ptr_type(), classid(), { lhs } )
{
}

SelfInstruction::SelfInstruction( const Type::Ptr& type )
: Instruction( type->ptr_result(), classid() )
{
}

void SelfInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void SelfInstruction::execute( Constant& res, const Constant& lhs ) const
{
    throw InternalException( "WTF!" );
}

u1 SelfInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
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
    return obj->id() == classid()                    //
           or ArithmeticInstruction::classof( obj )  //
           or CompareInstruction::classof( obj )     //
           or LogicalInstruction::classof( obj );
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
    return obj->id() == classid()             //
           or InvInstruction::classof( obj )  //
           or AddInstruction::classof( obj )  //
           or SubInstruction::classof( obj )  //
           or MulInstruction::classof( obj )  //
           or DivInstruction::classof( obj )  //
           or PowInstruction::classof( obj )  //
           or ModInstruction::classof( obj )  //
           or OrInstruction::classof( obj )   //
           or XorInstruction::classof( obj )  //
           or AndInstruction::classof( obj );
}

static const Properties arithmetic_instruction_properties = { Property::SIDE_EFFECT_FREE,
                                                              Property::PURE };

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

void InvInstruction::execute( Constant& res, const Constant& lhs ) const
{
    // inv : (lhs : T) -> T
    //
    // | case |         | A     | B         | C     |
    // |------+---------+-------+-----------+-------|
    // | lhs  | inv (-) | undef | lhs       | sym   |
    // |------+---------+-------+-----------+-------|
    // |    0 | undef   | undef | -lhs      | sym'  |

    if( not lhs.defined() )
    {
        res = lhs;
        return;
    }

    if( lhs.symbolic() )
    {
        // TODO: FIXME: @ppaulweber: return here a symbolic constant and trace @moosbruggerj
        throw InternalException( "unimplemented '" + description() + "'" );
        // res = SymbolicConstant( ... );
        return;
    }

    switch( lhs.typeId().kind() )
    {
        case Type::Kind::INTEGER:
        {
            const auto& val = static_cast< const IntegerConstant& >( lhs ).value();

            res = IntegerConstant( -val );
            break;
        }
        case Type::Kind::BINARY:
        {
            const auto& t = static_cast< const BinaryType& >( lhs.type() );
            auto val = static_cast< const BinaryConstant& >( lhs ).value();
            val = ~val;
            val++;
            val = val & Utility::createMask( t.bitsize() );
            res = BinaryConstant( lhs.type().ptr_type(), val );
            break;
        }
        case Type::Kind::DECIMAL:
        {
            const auto& val = static_cast< const DecimalConstant& >( lhs ).value();

            res = DecimalConstant( -val );
            break;
        }
        case Type::Kind::RATIONAL:
        {
            const auto& val = static_cast< const RationalConstant& >( lhs ).value();

            res = RationalConstant( -val );
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
            res = VoidConstant();
            break;
        }
    }
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
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
    },
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

void AddInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // add : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs     | A     | B         | C     |
    // |------+---------+-------+-----------+-------|
    // |  lhs | add (+) | undef | rhs       | sym   |
    // |------+---------+-------+-----------+-------|
    // |    0 | undef   | undef | undef     | undef |
    // |    1 | lhs     | undef | lhs + rhs | sym'  |
    // |    2 | sym     | undef | sym'      | sym'  |

    if( not lhs.defined() )
    {
        res = lhs;
        return;
    }

    if( not rhs.defined() )
    {
        res = rhs;
        return;
    }

    if( lhs.symbolic() or rhs.symbolic() )
    {
        res = symbolicArithmeticInstruction( lhs, rhs, *this );
        return;
    }

    switch( typeId().kind() )
    {
        case Type::Kind::INTEGER:
        {
            auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
            auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

            res = IntegerConstant( lval + rval );
            break;
        }
        case Type::Kind::BINARY:
        {
            assert( lhs.type().isBinary() and lhs.type() == rhs.type() );
            const auto& lhsBinaryType = static_cast< const BinaryType& >( lhs.type() );
            auto m = libstdhl::Type::createNatural( 1 ) << lhsBinaryType.bitsize();
            m--;

            const auto& lval = static_cast< const BinaryConstant& >( lhs ).value();
            const auto& rval = static_cast< const BinaryConstant& >( rhs ).value();
            const auto v = libstdhl::Type::createNatural( lval + rval );
            const auto r = v & m;

            res = BinaryConstant( lhs.type().ptr_type(), r );
            break;
        }
        case Type::Kind::STRING:
        {
            auto& lval = static_cast< const StringConstant& >( lhs ).value();
            auto& rval = static_cast< const StringConstant& >( rhs ).value();

            res = StringConstant( lval.toString() + rval.toString() );
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
            break;
        }
    }
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

void SubInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // sub : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs     | A     | B         | C     |
    // |------+---------+-------+-----------+-------|
    // |  lhs | sub (-) | undef | rhs       | sym   |
    // |------+---------+-------+-----------+-------|
    // |    0 | undef   | undef | undef     | undef |
    // |    1 | lhs     | undef | lhs - rhs | sym'  |
    // |    2 | sym     | undef | sym'      | sym'  |

    if( not lhs.defined() )
    {
        res = lhs;
        return;
    }

    if( not rhs.defined() )
    {
        res = rhs;
        return;
    }

    if( lhs.symbolic() or rhs.symbolic() )
    {
        res = symbolicArithmeticInstruction( lhs, rhs, *this );
        return;
    }

    switch( typeId().kind() )
    {
        case Type::Kind::INTEGER:
        {
            const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
            const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

            res = IntegerConstant( lval - rval );
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
            break;
        }
    }
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

void MulInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // mul : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs     | A     | B         | C     |
    // |------+---------+-------+-----------+-------|
    // |  lhs | mul (*) | undef | rhs       | sym   |
    // |------+---------+-------+-----------+-------|
    // |    0 | undef   | undef | undef     | undef |
    // |    1 | lhs     | undef | lhs * rhs | sym'  |
    // |    2 | sym     | undef | sym'      | sym'  |

    if( not lhs.defined() )
    {
        res = lhs;
        return;
    }

    if( not rhs.defined() )
    {
        res = rhs;
        return;
    }

    if( lhs.symbolic() or rhs.symbolic() )
    {
        res = symbolicArithmeticInstruction( lhs, rhs, *this );
        return;
    }

    switch( typeId().kind() )
    {
        case Type::Kind::INTEGER:
        {
            const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
            const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

            res = IntegerConstant( lval * rval );
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
            break;
        }
    }
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

void ModInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // mod : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs     | A     | B         | C     |
    // |------+---------+-------+-----------+-------|
    // |  lhs | mod (%) | undef | rhs       | sym   |
    // |------+---------+-------+-----------+-------|
    // |    0 | undef   | undef | undef     | undef |
    // |    1 | lhs     | undef | lhs % rhs | sym'  |
    // |    2 | sym     | undef | sym'      | sym'  |

    if( not lhs.defined() )
    {
        res = lhs;
        return;
    }

    if( not rhs.defined() )
    {
        res = rhs;
        return;
    }

    if( lhs.symbolic() or rhs.symbolic() )
    {
        res = symbolicArithmeticInstruction( lhs, rhs, *this );
        return;
    }

    switch( typeId().kind() )
    {
        case Type::Kind::INTEGER:
        {
            const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
            const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

            if( rval == 0 )
            {
                res = IntegerConstant();
            }
            else
            {
                res = IntegerConstant( lval % rval );
            }
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
            break;
        }
    }
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

void DivInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // div : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs      | A     | B         | C        | D     |
    // |------+----------+-------+-----------+----------+-------|
    // |  lhs | div (/)  | undef | rhs != 0  | rhs == 0 | sym   |
    // |------+----------+-------+-----------+----------+-------|
    // |    0 | undef    | undef | undef     | undef    | undef |
    // |    1 | lhs != 0 | undef | lhs / rhs | undef    | sym'  |
    // |    2 | lhs == 0 | undef | 0 : T     | undef    | 0 : T |
    // |    3 | sym      | undef | sym'      | undef    | sym'  |

    if( not lhs.defined() )
    {
        res = lhs;
        return;
    }

    if( not rhs.defined() )
    {
        res = rhs;
        return;
    }

    if( lhs.symbolic() or rhs.symbolic() )
    {
        res = symbolicArithmeticInstruction( lhs, rhs, *this );
        return;
    }

    switch( typeId().kind() )
    {
        case Type::Kind::INTEGER:
        {
            const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
            const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

            if( rval == 0 )
            {
                res = IntegerConstant();
            }
            else
            {
                res = IntegerConstant( lval / rval );
            }
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
            break;
        }
    }
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

void PowInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // pow : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs      | A     | B               | C       | D       | E          | F     |
    // |------+----------+-------+-----------------+---------+---------+------------+-------|
    // |  lhs | pow (^)  | undef | rhs < 0         | rhs = 0 | rhs = 1 | rhs > 1    | sym   |
    // |------+----------+-------+-----------------+---------+---------+------------+-------|
    // |    0 | undef    | undef | undef           |   undef | undef   | undef      | undef |
    // |    1 | lhs < -1 | undef | undef           |       1 | lhs     | lhs ^ rhs  | sym'  |
    // |    2 | lhs = -1 | undef | (-1) ^ abs(rhs) |       1 | -1      | (-1) ^ rhs | sym'  |
    // |    3 | lhs = 0  | undef | undef           |   undef | 0       | 0          | sym'  |
    // |    4 | lhs = 1  | 1     | 1               |       1 | 1       | 1          | 1     |
    // |    5 | lhs > 1  | undef | undef           |       1 | lhs     | lhs ^ rhs  | sym'  |
    // |    6 | sym      | sym'  | sym'            |    sym' | sym'    | sym'       | sym'  |

    if( not lhs.defined() )
    {
        res = lhs;
        return;
    }

    // TODO: FIXME: @ppaulweber: symbolic constant and trace @moosbruggerj

    if( not rhs.defined() )
    {
        switch( typeId().kind() )
        {
            case Type::Kind::INTEGER:
            {
                const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();

                if( lval == 1 )
                {
                    res = lhs;
                }
                else
                {
                    res = Constant::undef( type().ptr_type() );
                }
                break;
            }
            case Type::Kind::DECIMAL:
            {
                const auto& lval = static_cast< const DecimalConstant& >( lhs ).value();

                if( lval == 1 )
                {
                    res = lhs;
                }
                else
                {
                    res = Constant::undef( type().ptr_type() );
                }
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
    else
    {
        switch( typeId().kind() )
        {
            case Type::Kind::INTEGER:
            {
                const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
                const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

                if( rval < 0 )
                {
                    if( lval == 1 or -lval == 1 )
                    {
                        auto neg_rval = -rval;
                        const auto& nat_rval =
                            static_cast< const libstdhl::Type::Natural& >( neg_rval );

                        IntegerConstant( lval ^ nat_rval );
                    }
                    else
                    {
                        Constant::undef( type().ptr_type() );
                    }
                }
                else if( rval == 0 )
                {
                    if( lval == 0 )
                    {
                        res = Constant::undef( type().ptr_type() );
                    }
                    else
                    {
                        res = IntegerConstant( 1 );
                    }
                }
                else
                {
                    const auto& nat_rval = static_cast< const libstdhl::Type::Natural& >( rval );
                    res = IntegerConstant( lval ^ nat_rval );
                }
                break;
            }
            case Type::Kind::DECIMAL:
            {
                const auto& lval = static_cast< const DecimalConstant& >( lhs ).value();
                const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

                if( rval < 0 )
                {
                    if( lval == 1 or -lval == 1 )
                    {
                        auto neg_rval = -rval;
                        const auto& nat_rval =
                            static_cast< const libstdhl::Type::Natural& >( neg_rval );

                        DecimalConstant( lval ^ nat_rval );
                    }
                    else
                    {
                        Constant::undef( type().ptr_type() );
                    }
                }
                else if( rval == 0 )
                {
                    if( lval == 0 )
                    {
                        res = Constant::undef( type().ptr_type() );
                    }
                    else
                    {
                        res = DecimalConstant( 1 );
                    }
                }
                else
                {
                    const auto& nat_rval = static_cast< const libstdhl::Type::Natural& >( rval );
                    res = DecimalConstant( lval ^ nat_rval );
                }
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
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
    return obj->id() == classid()             //
           or OrInstruction::classof( obj )   //
           or ImpInstruction::classof( obj )  //
           or XorInstruction::classof( obj )  //
           or AndInstruction::classof( obj )  //
           or NotInstruction::classof( obj );
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

void AndInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // and : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs   | A     | B     | C     | D     |
    // |------+-------+-------+-------+-------+-------|
    // |  lhs | and   | undef | false | true  | sym   |
    // |------+-------+-------+-------+-------+-------|
    // |    0 | undef | undef | false | undef | sym'  |
    // |    1 | false | false | false | false | false |
    // |    2 | true  | undef | false | true  | sym'  |
    // |    3 | sym   | sym'  | false | sym'  | sym'  |

    if( lhs.symbolic() or rhs.symbolic() )
    {
        switch( typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                if( lhs.defined() and rhs.defined() )
                {
                    res = symbolicInstruction(
                        lhs,
                        rhs,
                        [ & ](
                            auto& env,
                            const auto& lhsSym,
                            const auto& rhsSym,
                            const auto& resSym ) {
                            using Connective = TPTP::BinaryLogic::Connective;

                            auto equ = std::make_shared< TPTP::BinaryLogic >(
                                lhsSym, Connective::CONJUNCTION, rhsSym );

                            equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                            equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                            return std::make_shared< TPTP::BinaryLogic >(
                                resSym, Connective::EQUIVALENCE, equ );
                        } );
                    return;
                }
                else
                {
                    throw InternalException( "unimplemented '" + description() + "'" );
                    break;
                }
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
    else
    {
        switch( typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                const auto& lval = static_cast< const BooleanConstant& >( lhs ).value();
                const auto& rval = static_cast< const BooleanConstant& >( rhs ).value();

                if( lhs.defined() and rhs.defined() )
                {
                    res = BooleanConstant( lval.value() and rval.value() );
                }
                else
                {
                    if( ( lhs.defined() and ( not lval.value() ) ) or
                        ( rhs.defined() and ( not rval.value() ) ) )
                    {
                        res = BooleanConstant( false );
                    }
                    else
                    {
                        res = BooleanConstant();
                    }
                }
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& lval = static_cast< const BinaryConstant& >( lhs ).value();
                const auto& rval = static_cast< const BinaryConstant& >( rhs ).value();

                assert( type().isBinary() );
                const auto resultType = std::static_pointer_cast< BinaryType >( type().ptr_type() );

                if( lhs.defined() and rhs.defined() )
                {
                    res = BinaryConstant( resultType, lval.value() & rval.value() );
                }
                else
                {
                    res = BinaryConstant( resultType );
                }
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
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

void XorInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // xor : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs   | A     | B     | C     | D    |
    // |------+-------+-------+-------+-------+------|
    // |  lhs | xor   | undef | false | true  | sym  |
    // |------+-------+-------+-------+-------+------|
    // |    0 | undef | undef | undef | undef | sym' |
    // |    1 | false | undef | false | true  | sym' |
    // |    2 | true  | undef | true  | false | sym' |
    // |    3 | sym   | sym'  | sym'  | sym'  | sym' |

    if( lhs.symbolic() or rhs.symbolic() )
    {
        switch( typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                if( lhs.defined() and rhs.defined() )
                {
                    res = symbolicInstruction(
                        lhs,
                        rhs,
                        [ & ](
                            auto& env,
                            const auto& lhsSym,
                            const auto& rhsSym,
                            const auto& resSym ) {
                            using Connective = TPTP::BinaryLogic::Connective;

                            auto equ = std::make_shared< TPTP::BinaryLogic >(
                                lhsSym, Connective::NON_EQUIVALENCE, rhsSym );

                            equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                            equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                            return std::make_shared< TPTP::BinaryLogic >(
                                resSym, Connective::EQUIVALENCE, equ );
                        } );
                    return;
                }
                else
                {
                    throw InternalException( "unimplemented '" + description() + "'" );
                    break;
                }
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
    else
    {
        switch( typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                const auto& lval = static_cast< const BooleanConstant& >( lhs ).value();
                const auto& rval = static_cast< const BooleanConstant& >( rhs ).value();

                if( lhs.defined() and rhs.defined() )
                {
                    res = BooleanConstant( lval.value() xor rval.value() );
                }
                else
                {
                    res = BooleanConstant();
                }
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& lval = static_cast< const BinaryConstant& >( lhs ).value();
                const auto& rval = static_cast< const BinaryConstant& >( rhs ).value();

                assert( type().isBinary() );
                const auto resultType = std::static_pointer_cast< BinaryType >( type().ptr_type() );

                if( lhs.defined() and rhs.defined() )
                {
                    res = BinaryConstant( resultType, lval.value() ^ rval.value() );
                }
                else
                {
                    res = BinaryConstant( resultType );
                }
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
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

void OrInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // or : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs   | A     | B     | C    | D    |
    // |------+-------+-------+-------+------+------|
    // |  lhs | or    | undef | false | true | sym  |
    // |------+-------+-------+-------+------+------|
    // |    0 | undef | undef | undef | true | sym' |
    // |    1 | false | undef | false | true | sym' |
    // |    2 | true  | true  | true  | true | true |
    // |    3 | sym   | sym'  | sym'  | true | sym' |

    if( lhs.symbolic() or rhs.symbolic() )
    {
        switch( typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                if( lhs.defined() and rhs.defined() )
                {
                    res = symbolicInstruction(
                        lhs,
                        rhs,
                        [ & ](
                            auto& env,
                            const auto& lhsSym,
                            const auto& rhsSym,
                            const auto& resSym ) {
                            using Connective = TPTP::BinaryLogic::Connective;

                            auto equ = std::make_shared< TPTP::BinaryLogic >(
                                lhsSym, Connective::DISJUNCTION, rhsSym );

                            equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                            equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                            return std::make_shared< TPTP::BinaryLogic >(
                                resSym, Connective::EQUIVALENCE, equ );
                        } );
                    return;
                }
                else
                {
                    throw InternalException( "unimplemented '" + description() + "'" );
                    break;
                }
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
    else
    {
        switch( typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                const auto& lval = static_cast< const BooleanConstant& >( lhs ).value();
                const auto& rval = static_cast< const BooleanConstant& >( rhs ).value();

                if( lhs.defined() and rhs.defined() )
                {
                    res = BooleanConstant( lval.value() or rval.value() );
                }
                else
                {
                    if( ( lhs.defined() and lval.value() ) or ( rhs.defined() and rval.value() ) )
                    {
                        res = BooleanConstant( true );
                    }
                    else
                    {
                        res = BooleanConstant();
                    }
                }
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& lval = static_cast< const BinaryConstant& >( lhs ).value();
                const auto& rval = static_cast< const BinaryConstant& >( rhs ).value();

                assert( type().isBinary() );
                const auto resultType = std::static_pointer_cast< BinaryType >( type().ptr_type() );

                if( lhs.defined() and rhs.defined() )
                {
                    res = BinaryConstant( resultType, lval.value() | rval.value() );
                }
                else
                {
                    res = BinaryConstant( resultType );
                }
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
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

void ImpInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // imp : (lhs : T) * (rhs : T) -> T
    //
    // | case | rhs   | A     | B     | C     | D     |
    // |------+-------+-------+-------+-------+-------|
    // |  lhs | imp   | undef | false | true  | sym   |
    // |------+-------+-------+-------+-------+-------|
    // |    0 | undef | undef | undef | true  | sym'  |
    // |    1 | false | true  | true  | true  | true  |
    // |    2 | true  | undef | false | true  | sym'  |
    // |    3 | sym   | sym'  | sym'  | true  | sym'  |

    if( lhs.symbolic() or rhs.symbolic() )
    {
        if( lhs.defined() and rhs.defined() )
        {
            res = symbolicInstruction(
                lhs,
                rhs,
                [ & ]( auto& env, const auto& lhsSym, const auto& rhsSym, const auto& resSym ) {
                    using Connective = TPTP::BinaryLogic::Connective;

                    auto equ = std::make_shared< TPTP::BinaryLogic >(
                        lhsSym, Connective::IMPLICATION, rhsSym );

                    equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                    equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                    return std::make_shared< TPTP::BinaryLogic >(
                        resSym, Connective::EQUIVALENCE, equ );
                } );
        }
        else
        {
            throw InternalException( "unimplemented '" + description() + "'" );
        }
    }
    else
    {
        const auto& lval = static_cast< const BooleanConstant& >( lhs ).value();
        const auto& rval = static_cast< const BooleanConstant& >( rhs ).value();

        if( lhs.defined() and rhs.defined() )
        {
            res = BooleanConstant( ( not lval.value() ) or rval.value() );
        }
        else
        {
            if( ( lhs.defined() and ( not lval.value() ) ) or ( rhs.defined() and rval.value() ) )
            {
                res = BooleanConstant( true );
            }
            else
            {
                res = BooleanConstant();
            }
        }
    }
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

void NotInstruction::execute( Constant& res, const Constant& lhs ) const
{
    // not : (lhs : T) -> T
    //
    // | case |       | A     | B     | C     | D     |
    // |------+-------+-------+-------+-------+-------|
    // | lhs  | not   | undef | false | true  | sym   |
    // |------+-------+-------+-------+-------+-------|
    // |    0 | undef | undef | true  | false | sym'  |

    // TODO: FIXME: @ppaulweber: symbolic constant and trace @moosbruggerj

    if( not lhs.defined() )
    {
        res = Constant::undef( type().ptr_type() );
        return;
    }

    switch( lhs.typeId().kind() )
    {
        case Type::Kind::BOOLEAN:
        {
            const auto& val = static_cast< const BooleanConstant& >( lhs ).value();

            res = BooleanConstant( not val.value() );
            break;
        }
        case Type::Kind::INTEGER:
        {
            const auto& val = static_cast< const IntegerConstant& >( lhs ).value();

            res = BooleanConstant( val == 0 );
            break;
        }
        case Type::Kind::BINARY:
        {
            const auto& val = static_cast< const BinaryConstant& >( lhs ).value();

            res = BinaryConstant( lhs.type().ptr_type(), ~val );
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
            break;
        }
    }
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
    return obj->id() == classid()             //
           or EquInstruction::classof( obj )  //
           or NeqInstruction::classof( obj )  //
           or LthInstruction::classof( obj )  //
           or LeqInstruction::classof( obj )  //
           or GthInstruction::classof( obj )  //
           or GeqInstruction::classof( obj );
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
    assert( type->result().isBoolean() );
}

void EquInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void EquInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // equ : (lhs : T) * (rhs : T) -> Boolean
    //
    // | case | rhs      | A     | B          | C     |
    // |------+----------+-------+------------+-------|
    // |  lhs | equ (=)  | undef | rhs        | sym   |
    // |------+----------+-------+------------+-------|
    // |    0 | undef    | true  | false      | sym'  |
    // |    1 | lhs      | false | lhs == rhs | sym'  |
    // |    2 | sym      | sym'  | sym'       | sym'  |

    if( lhs.symbolic() or rhs.symbolic() )
    {
        res = symbolicInstruction(
            lhs,
            rhs,
            [ & ]( auto& env, const auto& lhsSym, const auto& rhsSym, const auto& resSym ) {
                using Connective = TPTP::BinaryLogic::Connective;

                auto equ = std::make_shared< TPTP::InfixLogic >(
                    lhsSym, TPTP::InfixLogic::Connective::EQUALITY, rhsSym );

                equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                return std::make_shared< TPTP::BinaryLogic >(
                    resSym, Connective::EQUIVALENCE, equ );
            },
            Type::fromID( Type::Kind::BOOLEAN ) );
    }
    else
    {
        if( lhs.defined() and rhs.defined() )
        {
            res = BooleanConstant( lhs == rhs );
        }
        else if( lhs.defined() or rhs.defined() )
        {
            res = BooleanConstant( false );
        }
        else
        {
            res = BooleanConstant( true );
        }
    }
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
    assert( type->result().isBoolean() );
}

void NeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void NeqInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // neq : (lhs : T) * (rhs : T) -> Boolean
    //
    // | case | rhs      | A     | B          | C     |
    // |------+----------+-------+------------+-------|
    // |  lhs | neq (!=) | undef | rhs        | sym   |
    // |------+----------+-------+------------+-------|
    // |    0 | undef    | false | true       | sym'  |
    // |    1 | lhs      | true  | lhs != rhs | sym'  |
    // |    2 | sym      | sym'  | sym'       | sym'  |

    if( lhs.symbolic() or rhs.symbolic() )
    {
        res = symbolicInstruction(
            lhs,
            rhs,
            [ & ]( auto& env, const auto& lhsSym, const auto& rhsSym, const auto& resSym ) {
                using Connective = TPTP::InfixLogic::Connective;

                auto equ =
                    std::make_shared< TPTP::InfixLogic >( lhsSym, Connective::INEQUALITY, rhsSym );

                equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                return std::make_shared< TPTP::BinaryLogic >(
                    resSym, TPTP::BinaryLogic::Connective::EQUIVALENCE, equ );
            },
            Type::fromID( Type::Kind::BOOLEAN ) );
    }
    else
    {
        if( lhs.defined() and rhs.defined() )
        {
            res = BooleanConstant( lhs != rhs );
        }
        else if( lhs.defined() or rhs.defined() )
        {
            res = BooleanConstant( true );
        }
        else
        {
            res = BooleanConstant( false );
        }
    }
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
    assert( type->result().isBoolean() );
}

void LthInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // lth : (lhs : T) * (rhs : T) -> Boolean
    //
    // | case | rhs     | A     | B         | C     |
    // |------+---------+-------+-----------+-------|
    // |  lhs | lth (<) | undef | rhs       | sym   |
    // |------+---------+-------+-----------+-------|
    // |    0 | undef   | undef | undef     | undef |
    // |    1 | lhs     | undef | lhs < rhs | sym'  |
    // |    2 | sym     | undef | sym'      | sym'  |

    assert( lhs.type() == rhs.type() );

    if( not lhs.defined() or not rhs.defined() )
    {
        res = BooleanConstant();
        return;
    }

    if( lhs.symbolic() or rhs.symbolic() )
    {
        switch( lhs.typeId().kind() )
        {
            case Type::Kind::INTEGER:  // [fallthrough]
            case Type::Kind::DECIMAL:  // [fallthrough]
            case Type::Kind::RATIONAL:
            {
                res = symbolicInstruction(
                    lhs,
                    rhs,
                    [ & ]( auto& env, const auto& lhsSym, const auto& rhsSym, const auto& resSym ) {
                        auto equ = TPTP::FunctorAtom::less( lhsSym, rhsSym );

                        equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                        equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                        return std::make_shared< TPTP::BinaryLogic >(
                            resSym, TPTP::BinaryLogic::Connective::EQUIVALENCE, equ );
                    },
                    Type::fromID( Type::Kind::BOOLEAN ) );
                return;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
        return;
    }
    else
    {
        switch( lhs.typeId().kind() )
        {
            case Type::Kind::INTEGER:
            {
                const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
                const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

                res = BooleanConstant( lval < rval );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
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
    assert( type->result().isBoolean() );
}

void LeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void LeqInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // leq : (lhs : T) * (rhs : T) -> Boolean
    //
    // | case | rhs      | A     | B          | C     |
    // |------+----------+-------+------------+-------|
    // |  lhs | leq (<=) | undef | rhs        | sym   |
    // |------+----------+-------+------------+-------|
    // |    0 | undef    | true  | undef      | undef |
    // |    1 | lhs      | undef | lhs <= rhs | sym'  |
    // |    2 | sym      | undef | sym'       | sym'  |

    assert( lhs.type() == rhs.type() );

    if( lhs.defined() and rhs.defined() )
    {
        if( lhs.symbolic() or rhs.symbolic() )
        {
            switch( lhs.typeId().kind() )
            {
                case Type::Kind::INTEGER:  // [fallthrough]
                case Type::Kind::DECIMAL:  // [fallthrough]
                case Type::Kind::RATIONAL:
                {
                    res = symbolicInstruction(
                        lhs,
                        rhs,
                        [ & ](
                            auto& env,
                            const auto& lhsSym,
                            const auto& rhsSym,
                            const auto& resSym ) {
                            auto equ = TPTP::FunctorAtom::less_eq( lhsSym, rhsSym );

                            equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                            equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                            return std::make_shared< TPTP::BinaryLogic >(
                                resSym, TPTP::BinaryLogic::Connective::EQUIVALENCE, equ );
                        },
                        Type::fromID( Type::Kind::BOOLEAN ) );
                    return;
                }
                default:
                {
                    throw InternalException( "unimplemented '" + description() + "'" );
                    break;
                }
            }
        }
        else
        {
            switch( lhs.typeId().kind() )
            {
                case Type::Kind::INTEGER:
                {
                    const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
                    const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

                    res = BooleanConstant( lval <= rval );
                    break;
                }
                default:
                {
                    throw InternalException( "unimplemented '" + description() + "'" );
                    break;
                }
            }
        }
    }
    else if( not lhs.defined() and not rhs.defined() )
    {
        res = BooleanConstant( true );
    }
    else
    {
        res = BooleanConstant();
    }
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
    assert( type->result().isBoolean() );
}

void GthInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void GthInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // gth : (lhs : T) * (rhs : T) -> Boolean
    //
    // | case | rhs     | A     | B         | C     |
    // |------+---------+-------+-----------+-------|
    // |  lhs | gth (>) | undef | rhs       | sym   |
    // |------+---------+-------+-----------+-------|
    // |    0 | undef   | undef | undef     | undef |
    // |    1 | lhs     | undef | lhs > rhs | sym'  |
    // |    2 | sym     | undef | sym'      | sym'  |

    assert( lhs.type() == rhs.type() );

    if( not lhs.defined() or not rhs.defined() )
    {
        res = BooleanConstant();
        return;
    }

    if( lhs.symbolic() or rhs.symbolic() )
    {
        switch( lhs.typeId().kind() )
        {
            case Type::Kind::INTEGER:  // [fallthrough]
            case Type::Kind::DECIMAL:  // [fallthrough]
            case Type::Kind::RATIONAL:
            {
                res = symbolicInstruction(
                    lhs,
                    rhs,
                    [ & ]( auto& env, const auto& lhsSym, const auto& rhsSym, const auto& resSym ) {
                        auto equ = TPTP::FunctorAtom::greater( lhsSym, rhsSym );

                        equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                        equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                        return std::make_shared< TPTP::BinaryLogic >(
                            resSym, TPTP::BinaryLogic::Connective::EQUIVALENCE, equ );
                    },
                    Type::fromID( Type::Kind::BOOLEAN ) );
                return;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
    else
    {
        switch( lhs.typeId().kind() )
        {
            case Type::Kind::INTEGER:
            {
                const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
                const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

                res = BooleanConstant( lval > rval );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
                break;
            }
        }
    }
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
    assert( type->result().isBoolean() );
}

void GeqInstruction::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void GeqInstruction::execute( Constant& res, const Constant& lhs, const Constant& rhs ) const
{
    // geq : (lhs : T) * (rhs : T) -> Boolean
    //
    // | case | rhs      | A     | B          | C     |
    // |------+----------+-------+------------+-------|
    // |  lhs | geq (>=) | undef | rhs        | sym   |
    // |------+----------+-------+------------+-------|
    // |    0 | undef    | true  | undef      | undef |
    // |    1 | lhs      | undef | lhs >= rhs | sym'  |
    // |    2 | sym      | undef | sym'       | sym'  |

    assert( lhs.type() == rhs.type() );

    if( lhs.defined() and rhs.defined() )
    {
        if( lhs.symbolic() or rhs.symbolic() )
        {
            switch( lhs.typeId().kind() )
            {
                case Type::Kind::INTEGER:  // [fallthrough]
                case Type::Kind::DECIMAL:  // [fallthrough]
                case Type::Kind::RATIONAL:
                {
                    res = symbolicInstruction(
                        lhs,
                        rhs,
                        [ & ](
                            auto& env,
                            const auto& lhsSym,
                            const auto& rhsSym,
                            const auto& resSym ) {
                            auto equ = TPTP::FunctorAtom::greater_eq( lhsSym, rhsSym );

                            equ->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
                            equ->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );

                            return std::make_shared< TPTP::BinaryLogic >(
                                resSym, TPTP::BinaryLogic::Connective::EQUIVALENCE, equ );
                        },
                        Type::fromID( Type::Kind::BOOLEAN ) );
                    return;
                }
                default:
                {
                    throw InternalException( "unimplemented '" + description() + "'" );
                    break;
                }
            }
        }
        else
        {
            switch( lhs.typeId().kind() )
            {
                case Type::Kind::INTEGER:
                {
                    const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
                    const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

                    res = BooleanConstant( lval >= rval );
                    break;
                }
                default:
                {
                    throw InternalException( "unimplemented '" + description() + "'" );
                    break;
                }
            }
        }
    }
    else if( not lhs.defined() and not rhs.defined() )
    {
        res = BooleanConstant( true );
    }
    else
    {
        res = BooleanConstant();
    }
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

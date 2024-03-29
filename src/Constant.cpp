//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcasm-ir/graphs/contributors>
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

#include "Constant.h"

#include <libcasm-ir/Exception>
#include <libcasm-ir/Instruction>
#include <libstdhl/Random>
#include <libstdhl/String>

#include <cmath>

#include <libtptp/Type>

using namespace libcasm_ir;

//
//
// ConstantHandlerManager
//

void ConstantHandlerManager::registerConstantHandler(
    std::unique_ptr< ConstantHandler > constantHandler )
{
    assert( constantHandler != nullptr && "invalid constant handler" );
    m_constantHandlers.emplace_back( std::move( constantHandler ) );
}

void ConstantHandlerManager::processConstantHandlers(
    const std::function< u1( const ConstantHandler& ) >& process )
{
    for( const auto& constantHandler : m_constantHandlers )
    {
        if( process( *constantHandler ) )
        {
            break;
        }
    }
}

//
//
// Constant
//

static constexpr const char* undef_str = "undef";

static const auto VOID = libstdhl::Memory::get< VoidType >();
static const auto BOOLEAN = libstdhl::Memory::get< BooleanType >();
static const auto INTEGER = libstdhl::Memory::get< IntegerType >();
static const auto DECIMAL = libstdhl::Memory::get< DecimalType >();
static const auto RATIONAL = libstdhl::Memory::get< RationalType >();
static const auto STRING = libstdhl::Memory::get< StringType >();

Constant::Constant( const Type::Ptr& type, const libstdhl::Type::Data& data, Value::ID id )
: Value( type, id )
, m_data( data )
{
}

Constant::Constant( const Type::Ptr& type, Value::ID id )
: Value( type, id )
, m_data( nullptr )
{
}

Constant::Constant( void )
: Value( VOID, classid() )
, m_data( nullptr )
{
}

Constant::~Constant( void )
{
}

Constant::Constant( const Constant& other )
: Constant()
{
    *this = other;
}

Constant::Constant( Constant&& other ) noexcept
: Constant()
{
    *this = std::move( other );
}

Constant& Constant::operator=( const Constant& other )
{
    if( this != &other )
    {
        Value::operator=( other );
        m_data = other.m_data;
    }

    return *this;
}

Constant& Constant::operator=( Constant&& other ) noexcept
{
    if( this != &other )
    {
        Value::operator=( std::move( other ) );
        m_data = std::move( other.m_data );
    }

    return *this;
}

u1 Constant::defined( void ) const
{
    return m_data.defined();
}

u1 Constant::symbolic( void ) const
{
    return isa< SymbolicConstant >( *this );  // PPA: TODO: FIXME:
}

const libstdhl::Type::Data& Constant::data( void ) const
{
    return m_data;
}

std::string Constant::name( void ) const
{
    switch( id() )
    {
        case Value::VOID_CONSTANT:
        {
            return static_cast< const VoidConstant* >( this )->toString();
        }
        case Value::BOOLEAN_CONSTANT:
        {
            return static_cast< const BooleanConstant* >( this )->toString();
        }
        case Value::INTEGER_CONSTANT:
        {
            return static_cast< const IntegerConstant* >( this )->toString();
        }
        case Value::BINARY_CONSTANT:
        {
            return static_cast< const BinaryConstant* >( this )->toString();
        }
        case Value::STRING_CONSTANT:
        {
            return static_cast< const StringConstant* >( this )->toString();
        }
        case Value::DECIMAL_CONSTANT:
        {
            return static_cast< const DecimalConstant* >( this )->toString();
        }
        case Value::RATIONAL_CONSTANT:
        {
            return static_cast< const RationalConstant* >( this )->toString();
        }
        case Value::ENUMERATION_CONSTANT:
        {
            return static_cast< const EnumerationConstant* >( this )->toString();
        }
        case Value::RANGE_CONSTANT:
        {
            return static_cast< const RangeConstant* >( this )->toString();
        }
        case Value::TUPLE_CONSTANT:
        {
            return static_cast< const TupleConstant* >( this )->toString();
        }
        case Value::LIST_CONSTANT:
        {
            return static_cast< const ListConstant* >( this )->toString();
        }
        case Value::DOMAIN_CONSTANT:
        {
            return static_cast< const DomainConstant* >( this )->toString();
        }
        case Value::SYMBOLIC_CONSTANT:
        {
            return static_cast< const SymbolicConstant* >( this )->toString();
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            return static_cast< const RuleReferenceConstant* >( this )->toString();
        }
        default:
        {
            std::string result;
            ConstantHandlerManager::instance().processConstantHandlers(
                [this, &result]( const ConstantHandler& constantHandler ) -> u1 {
                    return constantHandler.name( *this, result );
                } );
            return result;
        }
    }
}

void Constant::accept( Visitor& visitor )
{
    switch( id() )
    {
        case Value::VOID_CONSTANT:
        {
            static_cast< VoidConstant* >( this )->accept( visitor );
            break;
        }
        case Value::BOOLEAN_CONSTANT:
        {
            static_cast< BooleanConstant* >( this )->accept( visitor );
            break;
        }
        case Value::INTEGER_CONSTANT:
        {
            static_cast< IntegerConstant* >( this )->accept( visitor );
            break;
        }
        case Value::BINARY_CONSTANT:
        {
            static_cast< BinaryConstant* >( this )->accept( visitor );
            break;
        }
        case Value::STRING_CONSTANT:
        {
            static_cast< StringConstant* >( this )->accept( visitor );
            break;
        }
        case Value::DECIMAL_CONSTANT:
        {
            static_cast< DecimalConstant* >( this )->accept( visitor );
            break;
        }
        case Value::RATIONAL_CONSTANT:
        {
            static_cast< RationalConstant* >( this )->accept( visitor );
            break;
        }
        case Value::ENUMERATION_CONSTANT:
        {
            static_cast< EnumerationConstant* >( this )->accept( visitor );
            break;
        }
        case Value::RANGE_CONSTANT:
        {
            static_cast< RangeConstant* >( this )->accept( visitor );
            break;
        }
        case Value::LIST_CONSTANT:
        {
            static_cast< ListConstant* >( this )->accept( visitor );
            break;
        }
        case Value::DOMAIN_CONSTANT:
        {
            static_cast< DomainConstant* >( this )->accept( visitor );
            break;
        }
        case Value::SYMBOLIC_CONSTANT:
        {
            static_cast< SymbolicConstant* >( this )->accept( visitor );
            break;
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            static_cast< RuleReferenceConstant* >( this )->accept( visitor );
            break;
        }
        default:
        {
            assert( !" invalid constant to dispatch 'accept' found! " );
        }
    }
}

void Constant::foreach( const std::function< void( const Constant& constant ) >& callback ) const
{
    assert( this->defined() );  // PPA: foreach only valid if a constant is
                                // defined, please ensure the correct invocation,
                                // a universe shall not be undefined

    switch( id() )
    {
        case Value::VOID_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::BOOLEAN_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::INTEGER_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::BINARY_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::STRING_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::DECIMAL_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::RATIONAL_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::ENUMERATION_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::RANGE_CONSTANT:
        {
            static_cast< const RangeConstant* >( this )->foreach( callback );
            break;
        }
        case Value::LIST_CONSTANT:
        {
            static_cast< const ListConstant* >( this )->foreach( callback );
            break;
        }
        case Value::DOMAIN_CONSTANT:
        {
            static_cast< const DomainConstant* >( this )->foreach( callback );
            break;
        }
        case Value::SYMBOLIC_CONSTANT:
        {
            callback( *this );
            break;
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            callback( *this );
            break;
        }
        default:
        {
            ConstantHandlerManager::instance().processConstantHandlers(
                [this, &callback]( const ConstantHandler& constantHandler ) -> u1 {
                    return constantHandler.foreach( *this, callback );
                } );
        }
    }
}

Constant Constant::choose( void ) const
{
    assert( this->defined() );  // PPA: foreach only valid if a constant is
                                // defined, please ensure the correct invocation,
                                // a universe shall not be undefined

    switch( id() )
    {
        case Value::VOID_CONSTANT:
        {
            return *this;
        }
        case Value::BOOLEAN_CONSTANT:
        {
            return *this;
        }
        case Value::INTEGER_CONSTANT:
        {
            return *this;
        }
        case Value::BINARY_CONSTANT:
        {
            return *this;
        }
        case Value::STRING_CONSTANT:
        {
            return *this;
        }
        case Value::DECIMAL_CONSTANT:
        {
            return *this;
        }
        case Value::RATIONAL_CONSTANT:
        {
            return *this;
        }
        case Value::ENUMERATION_CONSTANT:
        {
            return *this;
        }
        case Value::RANGE_CONSTANT:
        {
            return static_cast< const RangeConstant* >( this )->choose();
        }
        case Value::LIST_CONSTANT:
        {
            return static_cast< const ListConstant* >( this )->choose();
        }
        case Value::DOMAIN_CONSTANT:
        {
            return static_cast< const DomainConstant* >( this )->choose();
        }
        case Value::SYMBOLIC_CONSTANT:
        {
            return *this;
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            return *this;
        }
        default:
        {
            Constant result;
            ConstantHandlerManager::instance().processConstantHandlers(
                [this, &result]( const ConstantHandler& constantHandler ) -> u1 {
                    return constantHandler.choose( *this, result );
                } );
            return result;
        }
    }
}

std::size_t Constant::hash( void ) const
{
    switch( id() )
    {
        case Value::VOID_CONSTANT:
        {
            return static_cast< const VoidConstant* >( this )->hash();
        }
        case Value::BOOLEAN_CONSTANT:
        {
            return static_cast< const BooleanConstant* >( this )->hash();
        }
        case Value::INTEGER_CONSTANT:
        {
            return static_cast< const IntegerConstant* >( this )->hash();
        }
        case Value::BINARY_CONSTANT:
        {
            return static_cast< const BinaryConstant* >( this )->hash();
        }
        case Value::STRING_CONSTANT:
        {
            return static_cast< const StringConstant* >( this )->hash();
        }
        case Value::DECIMAL_CONSTANT:
        {
            return static_cast< const DecimalConstant* >( this )->hash();
        }
        case Value::RATIONAL_CONSTANT:
        {
            return static_cast< const RationalConstant* >( this )->hash();
        }
        case Value::ENUMERATION_CONSTANT:
        {
            return static_cast< const EnumerationConstant* >( this )->hash();
        }
        case Value::RANGE_CONSTANT:
        {
            return static_cast< const RangeConstant* >( this )->hash();
        }
        case Value::LIST_CONSTANT:
        {
            return static_cast< const ListConstant* >( this )->hash();
        }
        case Value::DOMAIN_CONSTANT:
        {
            return static_cast< const DomainConstant* >( this )->hash();
        }
        case Value::SYMBOLIC_CONSTANT:
        {
            return static_cast< const SymbolicConstant* >( this )->hash();
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            return static_cast< const RuleReferenceConstant* >( this )->hash();
        }
        default:
        {
            std::size_t result = 0;
            ConstantHandlerManager::instance().processConstantHandlers(
                [this, &result]( const ConstantHandler& constantHandler ) -> u1 {
                    return constantHandler.hash( *this, result );
                } );
            return result;
        }
    }
}

u1 Constant::operator==( const Value& rhs ) const
{
    switch( id() )
    {
        case Value::VOID_CONSTANT:
        {
            return static_cast< const VoidConstant* >( this )->operator==( rhs );
        }
        case Value::BOOLEAN_CONSTANT:
        {
            return static_cast< const BooleanConstant* >( this )->operator==( rhs );
        }
        case Value::INTEGER_CONSTANT:
        {
            return static_cast< const IntegerConstant* >( this )->operator==( rhs );
        }
        case Value::BINARY_CONSTANT:
        {
            return static_cast< const BinaryConstant* >( this )->operator==( rhs );
        }
        case Value::STRING_CONSTANT:
        {
            return static_cast< const StringConstant* >( this )->operator==( rhs );
        }
        case Value::DECIMAL_CONSTANT:
        {
            return static_cast< const DecimalConstant* >( this )->operator==( rhs );
        }
        case Value::RATIONAL_CONSTANT:
        {
            return static_cast< const RationalConstant* >( this )->operator==( rhs );
        }
        case Value::ENUMERATION_CONSTANT:
        {
            return static_cast< const EnumerationConstant* >( this )->operator==( rhs );
        }
        case Value::RANGE_CONSTANT:
        {
            return static_cast< const RangeConstant* >( this )->operator==( rhs );
        }
        case Value::LIST_CONSTANT:
        {
            return static_cast< const ListConstant* >( this )->operator==( rhs );
        }
        case Value::DOMAIN_CONSTANT:
        {
            return static_cast< const DomainConstant* >( this )->operator==( rhs );
        }
        case Value::SYMBOLIC_CONSTANT:
        {
            return static_cast< const SymbolicConstant* >( this )->operator==( rhs );
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            return static_cast< const RuleReferenceConstant* >( this )->operator==( rhs );
        }
        default:
        {
            u1 result = false;
            ConstantHandlerManager::instance().processConstantHandlers(
                [this, &rhs, &result]( const ConstantHandler& constantHandler ) -> u1 {
                    return constantHandler.compare( *this, rhs, result );
                } );
            return result;
        }
    }
}

u1 Constant::classof( Value const* obj )
{
    return obj->id() == classid() or VoidConstant::classof( obj ) or
           BooleanConstant::classof( obj ) or IntegerConstant::classof( obj ) or
           BinaryConstant::classof( obj ) or StringConstant::classof( obj ) or
           DecimalConstant::classof( obj ) or RationalConstant::classof( obj ) or
           EnumerationConstant::classof( obj ) or DomainConstant::classof( obj ) or
           SymbolicConstant::classof( obj ) or RuleReferenceConstant::classof( obj ) or
           Identifier::classof( obj );
}

Constant Constant::undef( const Type::Ptr& type )
{
    switch( type->kind() )
    {
        case Type::Kind::LABEL:     // [[fallthrough]]
        case Type::Kind::LOCATION:  // [[fallthrough]]
        case Type::Kind::RELATION:
        {
            break;
        }
        case Type::Kind::VOID:
        {
            return VoidConstant();
        }
        case Type::Kind::BOOLEAN:
        {
            return BooleanConstant();
        }
        case Type::Kind::INTEGER:
        {
            return IntegerConstant();
        }
        case Type::Kind::BINARY:
        {
            return BinaryConstant( std::static_pointer_cast< BinaryType >( type ) );
        }
        case Type::Kind::STRING:
        {
            return StringConstant();
        }
        case Type::Kind::DECIMAL:
        {
            return DecimalConstant();
        }
        case Type::Kind::RATIONAL:
        {
            return RationalConstant();
        }
        case Type::Kind::ENUMERATION:
        {
            return EnumerationConstant( std::static_pointer_cast< EnumerationType >( type ) );
        }
        case Type::Kind::RANGE:
        {
            return RangeConstant( std::static_pointer_cast< RangeType >( type ) );
        }
        case Type::Kind::LIST:
        {
            return ListConstant( std::static_pointer_cast< ListType >( type ) );
        }
        case Type::Kind::TUPLE:
        {
            return TupleConstant( std::static_pointer_cast< TupleType >( type ) );
        }
        case Type::Kind::RECORD:
        {
            return RecordConstant( std::static_pointer_cast< RecordType >( type ) );
        }
        case Type::Kind::OBJECT:
        {
            return DomainConstant( std::static_pointer_cast< ObjectType >( type ) );
        }
        case Type::Kind::RULE_REFERENCE:
        {
            return RuleReferenceConstant( type );
        }
        case Type::Kind::FUNCTION_REFERENCE:
        {
            return FunctionReferenceConstant( type );
        }
        case Type::Kind::FILE:
        {
            break;
        }
        case Type::Kind::PORT:
        {
            break;
        }
        case libcasm_ir::Type::Kind::_SIZE_:
        {
            assert( !" internal error!" );
            break;
        }
    }

    throw std::domain_error(
        "unimplemented type '" + type->description() + "' to create a 'undef' constant" );
    return VoidConstant();
}

//
//
// Void Constant
//

VoidConstant::VoidConstant( void )
: Constant( VOID, libstdhl::Type::Data( 0, false ), classid() )
{
}

std::string VoidConstant::toString( void ) const
{
    return ( defined() ? "void" : undef_str );
}

void VoidConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t VoidConstant::hash( void ) const
{
    const auto h = ( (std::size_t)classid() ) << 1;
    return h;
}

u1 VoidConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const VoidConstant& >( rhs );
    return ( this->defined() == other.defined() );
}

u1 VoidConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Boolean Constant
//

BooleanConstant::BooleanConstant( const std::string& value )
: Constant( BOOLEAN, libstdhl::Type::createBoolean( value ), classid() )
{
}

BooleanConstant::BooleanConstant( const libstdhl::Type::Boolean& value )
: Constant( BOOLEAN, value, classid() )
{
}

BooleanConstant::BooleanConstant( const u1 value )
: Constant( BOOLEAN, libstdhl::Type::createBoolean( value ), classid() )
{
}

BooleanConstant::BooleanConstant( void )
: Constant( BOOLEAN, classid() )
{
}

const libstdhl::Type::Boolean& BooleanConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::Boolean& >( m_data );
}

std::string BooleanConstant::toString( void ) const
{
    return ( defined() ? ( value() == true ? "true" : "false" ) : undef_str );
}

void BooleanConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t BooleanConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, value().value() );
}

u1 BooleanConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const BooleanConstant& >( rhs );
    return data() == other.data();
}

u1 BooleanConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Integer Constant
//

IntegerConstant::IntegerConstant( const std::string& value, const libstdhl::Type::Radix radix )
: Constant( INTEGER, libstdhl::Type::createInteger( value, radix ), classid() )
{
    // TODO: PPA: force CASM integer string digit separator usage as
    // group of
    // three
}

IntegerConstant::IntegerConstant( const BinaryConstant& value )
: Constant( INTEGER, value.value(), classid() )
{
}

IntegerConstant::IntegerConstant( const libstdhl::Type::Integer& value )
: Constant( INTEGER, value, classid() )
{
}

IntegerConstant::IntegerConstant( const libstdhl::Type::Natural& value, const u1 sign = false )
: Constant( INTEGER, libstdhl::Type::createInteger( value, sign ), classid() )
{
}

IntegerConstant::IntegerConstant( const i64 value )
: Constant( INTEGER, libstdhl::Type::createInteger( value ), classid() )
{
}

IntegerConstant::IntegerConstant( void )
: Constant( INTEGER, classid() )
{
}

i64 IntegerConstant::value_i64( void ) const
{
    if( m_data.trivial() )
    {
        return m_data.value() * ( m_data.sign() ? -1 : 1 );
    }
    else
    {
        assert( !" internal error! " );
        return 0;
    }
}

const libstdhl::Type::Integer& IntegerConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::Integer& >( m_data );
}

std::string IntegerConstant::toString( void ) const
{
    return ( defined() ? ( m_data.to< libstdhl::Type::DECIMAL >() ) : undef_str );
}

void IntegerConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t IntegerConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( value() ) );
}

u1 IntegerConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const IntegerConstant& >( rhs );
    return value() == other.value();
    // return ( this->defined() == other.defined() )
    //        and ( this->value() == other.value() );
}

u1 IntegerConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Binary Constant
//

BinaryConstant::BinaryConstant( const std::string& value, const libstdhl::Type::Radix radix )
: Constant(
      libstdhl::Memory::get< BinaryType >( value, radix ),
      libstdhl::Type::createNatural( value, radix ),
      classid() )
{
    assert( this->type().isBinary() );
    const auto& t = static_cast< const BinaryType& >( this->type() );
    if( t.bitsize() > BinaryType::SizeMax )
    {
        throw std::domain_error(
            "invalid bit size '" + std::to_string( t.bitsize() ) + "' to create BinaryConstant" );
    }
}

BinaryConstant::BinaryConstant( const Type::Ptr& type, const libstdhl::Type::Natural& value )
: Constant( type, value, classid() )
{
    assert( this->type().isBinary() );
    const auto& t = static_cast< const BinaryType& >( this->type() );

    if( t.bitsize() > BinaryType::SizeMax )
    {
        throw std::domain_error(
            "invalid bit size '" + std::to_string( t.bitsize() ) + "' to create BinaryConstant" );
    }

    assert( value.trivial() and " TODO: PPA: FIXME:" );
    const u64 bitsize =
        static_cast< u64 >( std::log2( (double)( value.value() > 1 ? value.value() - 1 : 1 ) ) );

    if( bitsize > t.bitsize() )
    {
        throw std::invalid_argument(
            "value bit-size '" + std::to_string( bitsize ) + "' does not fit into bit-size '" +
            std::to_string( t.bitsize() ) + "'" );
    }
}

BinaryConstant::BinaryConstant( const BinaryType::Ptr& type, u64 value )
: BinaryConstant( type, libstdhl::Type::createNatural( value ) )
{
}

BinaryConstant::BinaryConstant( const BinaryType::Ptr& type )
: Constant( type, classid() )
{
}

BinaryConstant::BinaryConstant( const u16 bitsize, const u64 value )
: BinaryConstant( libstdhl::Memory::get< BinaryType >( bitsize ), value )
{
}

BinaryConstant::BinaryConstant( const u16 bitsize )
: BinaryConstant( libstdhl::Memory::get< BinaryType >( bitsize ) )
{
}

u64 BinaryConstant::value_u64( void ) const
{
    return m_data.value();
}

const libstdhl::Type::Natural& BinaryConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::Natural& >( m_data );
}

std::string BinaryConstant::toString( void ) const
{
    return ( defined() ? ( m_data.to< libstdhl::Type::DECIMAL >() ) : undef_str );
}

void BinaryConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t BinaryConstant::hash( void ) const
{
    const auto h =
        libstdhl::Hash::combine( ( (std::size_t)classid() << 1 ) | defined(), type().hash() );
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( value() ) );
}

u1 BinaryConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const BinaryConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 BinaryConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// String Constant
//

StringConstant::StringConstant( const libstdhl::Type::String& value )
: Constant( STRING, value, classid() )
{
}

StringConstant::StringConstant( const std::string& value )
: Constant( STRING, libstdhl::Type::createString( value ), classid() )
{
}

StringConstant::StringConstant( void )
: Constant( STRING, classid() )
{
}

const libstdhl::Type::String& StringConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::String& >( m_data );
}

std::string StringConstant::toString( void ) const
{
    if( defined() )
    {
        return value().toString();
    }
    else
    {
        return undef_str;
    }
}

void StringConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t StringConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( value() ) );
}

u1 StringConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const StringConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 StringConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Decimal Constant
//

DecimalConstant::DecimalConstant( const libstdhl::Type::Decimal& value )
: Constant( DECIMAL, value, classid() )
{
}

DecimalConstant::DecimalConstant( const libstdhl::Type::Integer& value )
: Constant( DECIMAL, libstdhl::Type::createDecimal( value ), classid() )
{
}

DecimalConstant::DecimalConstant( const libstdhl::Type::Natural& value )
: Constant( DECIMAL, libstdhl::Type::createDecimal( value ), classid() )
{
}

DecimalConstant::DecimalConstant( const std::string& value )
: Constant( DECIMAL, libstdhl::Type::createDecimal( value ), classid() )
{
}

DecimalConstant::DecimalConstant( const double value )
: Constant( DECIMAL, libstdhl::Type::createDecimal( value ), classid() )
{
}

DecimalConstant::DecimalConstant( void )
: Constant( DECIMAL, classid() )
{
}

const libstdhl::Type::Decimal& DecimalConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::Decimal& >( m_data );
}

std::string DecimalConstant::toString( void ) const
{
    // return ( defined() ? ( "TODO" ) : undef_str );
    return ( defined() ? ( std::to_string( (double)value().value() ) ) : undef_str );
    // TODO: PPA: use literal function from libstdhl::Type::Data
}

void DecimalConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t DecimalConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( value() ) );
}

u1 DecimalConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const DecimalConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 DecimalConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rational Constant
//

RationalConstant::RationalConstant( const std::string& value )
: Constant( RATIONAL, libstdhl::Type::createRational( value ), classid() )
{
}

RationalConstant::RationalConstant( const libstdhl::Type::Rational& value )
: Constant( RATIONAL, value, classid() )
{
}

RationalConstant::RationalConstant( void )
: Constant( RATIONAL, classid() )
{
}

const libstdhl::Type::Rational& RationalConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::Rational& >( m_data );
}

std::string RationalConstant::toString( void ) const
{
    return ( defined() ? ( "TODO" ) : undef_str );
    // TODO: PPA: use literal function from libstdhl::Type::Data
}

void RationalConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t RationalConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( value() ) );
}

u1 RationalConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const RationalConstant& >( rhs );
    return value() == other.value();
}

u1 RationalConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Enumeration Constant
//

EnumerationConstant::EnumerationConstant(
    const EnumerationType::Ptr& type, const std::string& value )
: Constant( type, libstdhl::Type::createNatural( type->kind().encode( value ) ), classid() )
{
}

EnumerationConstant::EnumerationConstant( const Enumeration::Ptr& kind, const std::string& value )
: Constant(
      libstdhl::Memory::get< EnumerationType >( kind ),
      libstdhl::Type::createNatural( kind->encode( value ) ),
      classid() )
{
}

EnumerationConstant::EnumerationConstant( const EnumerationType::Ptr& type )
: Constant( type, classid() )
{
}

EnumerationConstant::EnumerationConstant( const Enumeration::Ptr& kind )
: Constant( libstdhl::Memory::get< EnumerationType >( kind ), classid() )
{
}

const libstdhl::Type::Natural& EnumerationConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::Natural& >( m_data );
}

std::string EnumerationConstant::toString( void ) const
{
    return static_cast< const EnumerationType& >( type() ).kind().decode( m_data.value() );
}

void EnumerationConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t EnumerationConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( value() ) );
}

u1 EnumerationConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const EnumerationConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 EnumerationConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Range Constant
//

RangeConstant::RangeConstant( const RangeType::Ptr& type, const Range::Ptr& value )
: Constant( type, libstdhl::Type::Data( 0, false ), classid() )
{
    assert( type );
    type->setRange( value );
}

RangeConstant::RangeConstant( const RangeType::Ptr& type )
: Constant( type, classid() )
{
}

RangeConstant::RangeConstant( const Type::Ptr& type, const Constant& from, const Constant& to )
: Constant( type, libstdhl::Type::Data( 0, false ), classid() )
{
    assert( type );
    assert( type->isRange() );
    static_cast< RangeType& >( *type ).setRange(
        libstdhl::Memory::make_unique< Range >( from, to ) );
}

Range::Ptr RangeConstant::value( void ) const
{
    return static_cast< const RangeType& >( type() ).ptr_range();
}

std::string RangeConstant::toString( void ) const
{
    return type().name();
}

void RangeConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void RangeConstant::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    type().foreach( callback );
}

Constant RangeConstant::choose( void ) const
{
    return type().choose();
}

std::size_t RangeConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, value()->hash() );
}

u1 RangeConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const RangeConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 RangeConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
//
// Tuple Constant (Layout)
//

TupleConstant::TupleLayout::TupleLayout( const std::vector< Constant >& elements )
: m_elements( elements )
{
}

const std::vector< Constant >& TupleConstant::TupleLayout::elements( void ) const
{
    return m_elements;
}

std::size_t TupleConstant::TupleLayout::hash( void ) const
{
    std::size_t hashSum = 0;

    for( const auto& element : elements() )
    {
        hashSum = libstdhl::Hash::combine( hashSum, element.hash() );
    }

    return hashSum;
}

libstdhl::Type::Layout* TupleConstant::TupleLayout::clone( void ) const
{
    return new TupleLayout( elements() );
}

//
//
// Tuple Constant
//

TupleConstant::TupleConstant( const TupleType::Ptr& type, const std::vector< Constant >& elements )
: Constant( type, libstdhl::Type::Data( new TupleLayout( elements ) ), classid() )
{
    assert( type );
}

TupleConstant::TupleConstant( const TupleType::Ptr& type )
: Constant( type, classid() )
{
    assert( type );
}

TupleConstant::TupleConstant(
    const RecordType::Ptr& type, const std::unordered_map< std::string, Constant >& elements )
: Constant( type, classid() )
{
    assert( type );
    assert( type->arguments().size() >= elements.size() and elements.size() > 0 );
    assert( not type->elements().empty() );

    std::vector< Constant > tupleElements;
    tupleElements.reserve( elements.size() );

    for( std::size_t index = 0; index < type->arguments().size(); index++ )
    {
        const auto& subTypeName = type->identifiers()[ index ];

        const auto it = elements.find( subTypeName );
        if( it != elements.cend() )
        {
            tupleElements.emplace_back( it->second );
        }
        else
        {
            tupleElements.emplace_back( Constant::undef( type->arguments()[ index ] ) );
        }
    }

    m_data = libstdhl::Type::Data( new TupleLayout( tupleElements ) );
}

TupleConstant::TupleConstant( const RecordType::Ptr& type )
: Constant( type, classid() )
{
    assert( type );
}

const Constant& TupleConstant::value( const std::size_t position ) const
{
    assert( position >= 1 and position <= cardinality() );
    return value()->elements()[ position - 1 ];
}

std::size_t TupleConstant::cardinality( void ) const
{
    if( not defined() )
    {
        return 0;
    }

    return value()->elements().size();
}

std::string TupleConstant::toString( void ) const
{
    if( not defined() )
    {
        return undef_str;
    }

    std::stringstream stream;
    stream << "(";

    for( const auto& element : value()->elements() )
    {
        stream << element.name() << ", ";
    }

    stream.seekp( -1, stream.cur );
    stream << ")";

    return stream.str();
}

void TupleConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void TupleConstant::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // not supported
}

Constant TupleConstant::choose( void ) const
{
    // not supported
    return VoidConstant();
}

std::size_t TupleConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, value()->hash() );
}

const TupleConstant::TupleLayout* TupleConstant::value( void ) const
{
    return static_cast< TupleLayout* >( m_data.ptr() );
}

u1 TupleConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const TupleConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 TupleConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// List Constant
//

ListConstant::ListConstant( const ListType::Ptr& type, const List::Ptr& value )
: Constant( type, libstdhl::Type::Data( (u64)value.get(), false ), classid() )
{
    assert( type );
    type->setList( value );
}

ListConstant::ListConstant( const ListType::Ptr& type )
: Constant( type, classid() )
{
}

const List* ListConstant::value( void ) const
{
    return (List*)m_data.value();
}

std::string ListConstant::toString( void ) const
{
    return type().name();
}

void ListConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void ListConstant::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    for( auto element : value()->elements() )
    {
        if( isa< Constant >( element ) )
        {
            const auto& c = static_cast< const Constant& >( *element );
            callback( c );
        }
    }
}

Constant ListConstant::choose( void ) const
{
    assert( value() != nullptr );

    Constant result = VoidConstant();
    std::size_t index = -1;
    while( true )
    {
        index = libstdhl::Random::uniform< std::size_t >( 0, value()->elements().size() - 1 );

        const auto element = value()->elements()[ index ];

        if( isa< Constant >( element ) )
        {
            result = static_cast< const Constant& >( *element );
            break;
        }
    }

    return result;
}

std::size_t ListConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, value()->hash() );
}

u1 ListConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const ListConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 ListConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
//
// Domain Constant
//

DomainConstant::DomainConstant( const Type::Ptr& type )
: Constant( type, libstdhl::Type::createNatural( 0 ), classid() )
{
}

std::string DomainConstant::toString( void ) const
{
    return type().description();
}

void DomainConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

void DomainConstant::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    type().foreach( callback );
}

Constant DomainConstant::choose( void ) const
{
    return type().choose();
}

std::size_t DomainConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, type().hash() );
}

u1 DomainConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const DomainConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->type() == other.type() );
}

u1 DomainConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rule Reference Constant
//

RuleReferenceConstant::RuleReferenceConstant( const Rule::Ptr& value )
: ReferenceConstant(
      std::static_pointer_cast< Type >( libstdhl::Memory::make< RuleReferenceType >(
          std::static_pointer_cast< RelationType >( value->type().ptr_type() ) ) ),
      value.get(),
      classid() )
{
}

RuleReferenceConstant::RuleReferenceConstant( const Type::Ptr& type )
: ReferenceConstant( type, classid() )
{
    assert( type->isRuleReference() );
}

std::string RuleReferenceConstant::toString( void ) const
{
    return ( defined() ? value()->name() : undef_str );
}

void RuleReferenceConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t RuleReferenceConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, value()->hash() );
}

u1 RuleReferenceConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const RuleReferenceConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 RuleReferenceConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Function Reference Constant
//

FunctionReferenceConstant::FunctionReferenceConstant( const Function::Ptr& value )
: ReferenceConstant(
      std::static_pointer_cast< Type >( libstdhl::Memory::make< FunctionReferenceType >(
          std::static_pointer_cast< RelationType >( value->type().ptr_type() ) ) ),
      value.get(),
      classid() )
{
}

FunctionReferenceConstant::FunctionReferenceConstant( const Derived::Ptr& value )
: ReferenceConstant(
      std::static_pointer_cast< Type >( libstdhl::Memory::make< FunctionReferenceType >(
          std::static_pointer_cast< RelationType >( value->type().ptr_type() ) ) ),
      value.get(),
      classid() )
{
}

FunctionReferenceConstant::FunctionReferenceConstant( const Builtin::Ptr& value )
: ReferenceConstant(
      std::static_pointer_cast< Type >( libstdhl::Memory::make< FunctionReferenceType >(
          std::static_pointer_cast< RelationType >( value->type().ptr_type() ) ) ),
      value.get(),
      classid() )
{
}

FunctionReferenceConstant::FunctionReferenceConstant( const Type::Ptr& type )
: ReferenceConstant( type, classid() )
{
    assert( type->isFunctionReference() );
}

std::string FunctionReferenceConstant::toString( void ) const
{
    return ( defined() ? value()->name() : undef_str );
}

void FunctionReferenceConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t FunctionReferenceConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, value()->hash() );
}

u1 FunctionReferenceConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const FunctionReferenceConstant& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->value() == other.value() );
}

u1 FunctionReferenceConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
//
// Symbolic Constant
//

SymbolicConstant::SymbolicLayout::SymbolicLayout(
    const std::string& name, SymbolicExecutionEnvironment& environment )
: m_name( name )
, m_environment( environment )
{
}

std::size_t SymbolicConstant::SymbolicLayout::hash( void ) const
{
    return libstdhl::String::value( name() );
}

libstdhl::Type::Layout* SymbolicConstant::SymbolicLayout::clone( void ) const
{
    return new SymbolicLayout( name(), environment() );
}

const std::string& SymbolicConstant::SymbolicLayout::name() const
{
    return m_name;
}

SymbolicExecutionEnvironment& SymbolicConstant::SymbolicLayout::environment( void ) const
{
    return m_environment;
}

SymbolicConstant::SymbolicConstant(
    const Type::Ptr& type, const std::string& name, SymbolicExecutionEnvironment& environment )
: Constant( type, libstdhl::Type::Data( new SymbolicLayout( name, environment ) ), classid() )
{
    environment.addSymbolDefinition( definition() );
}

SymbolicConstant::SymbolicConstant( const Type::Ptr& type )
: Constant( type, classid() )
{
    // TODO: @moosbruggerj undef symbolic var
}

std::string SymbolicConstant::toString( void ) const
{
    return ( defined() ? value()->name() : undef_str );
}

void SymbolicConstant::accept( Visitor& visitor )
{
    // TODO: @moosbruggerj fix me
    // visitor.visit( *this );
}

std::size_t SymbolicConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, value()->hash() );
}

u1 SymbolicConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }
    const auto& other = static_cast< const SymbolicConstant& >( rhs );
    return ( this->defined() == other.defined() ) and
           ( this->value()->name() == other.value()->name() );
}

u1 SymbolicConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

SymbolicExecutionEnvironment& SymbolicConstant::environment( void ) const
{
    if( !defined() )
    {
        throw UndefinedConstantException(
            "unable to get environment from undefined symbol constant." );
    }
    return value()->environment();
}

TPTP::Logic::Ptr SymbolicConstant::definition( void ) const
{
    if( !defined() )
    {
        throw UndefinedConstantException(
            "unable to get tptp symbol definition from undefined symbol constant." );
    }
    TPTP::Type::Ptr TPTPtype = getTPTPType( *this );
    return std::make_shared< TPTP::TypeAtom >(
        std::make_shared< TPTP::Identifier >( name() ), TPTPtype );
}

const SymbolicConstant::SymbolicLayout* SymbolicConstant::value( void ) const
{
    return static_cast< SymbolicLayout* >( m_data.ptr() );
}

const TPTP::Type::Ptr SymbolicConstant::getTPTPType( const Constant& constant ) const
{
    return environment().getTPTPType( constant.type() );
}

//
// Identifier
//

Identifier::Identifier( const Type::Ptr& type, const std::string& value )
: Constant( type, libstdhl::Type::createString( value ), classid() )
{
}

std::string Identifier::toString( void ) const
{
    return *( (std::string*)m_data.ptr() );
}

void Identifier::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t Identifier::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( name() ) );
}

u1 Identifier::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const Identifier& >( rhs );
    return ( this->defined() == other.defined() ) and ( this->name() == other.name() );
}

u1 Identifier::classof( Value const* obj )
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

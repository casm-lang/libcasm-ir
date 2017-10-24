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

#include <cmath>

using namespace libcasm_ir;

static constexpr const char* undef_str = "undef";

static const auto VOID = libstdhl::Memory::get< VoidType >();
static const auto BOOLEAN = libstdhl::Memory::get< BooleanType >();
static const auto INTEGER = libstdhl::Memory::get< IntegerType >();
static const auto FLOATING = libstdhl::Memory::get< FloatingType >();
static const auto RATIONAL = libstdhl::Memory::get< RationalType >();
static const auto STRING = libstdhl::Memory::get< StringType >();

Constant::Constant(
    const Type::Ptr& type, const libstdhl::Type::Data& data, Value::ID id )
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
    return false; // PPA: TODO: FIXME:
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
            return static_cast< const VoidConstant* >( this )->name();
        }
        case Value::BOOLEAN_CONSTANT:
        {
            return static_cast< const BooleanConstant* >( this )->name();
        }
        case Value::INTEGER_CONSTANT:
        {
            return static_cast< const IntegerConstant* >( this )->name();
        }
        case Value::BIT_CONSTANT:
        {
            return static_cast< const BitConstant* >( this )->name();
        }
        case Value::STRING_CONSTANT:
        {
            return static_cast< const StringConstant* >( this )->name();
        }
        case Value::FLOATING_CONSTANT:
        {
            return static_cast< const FloatingConstant* >( this )->name();
        }
        case Value::RATIONAL_CONSTANT:
        {
            return static_cast< const RationalConstant* >( this )->name();
        }
        case Value::ENUMERATION_CONSTANT:
        {
            return static_cast< const EnumerationConstant* >( this )->name();
        }
        case Value::RANGE_CONSTANT:
        {
            return static_cast< const RangeConstant* >( this )->name();
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            return static_cast< const RuleReferenceConstant* >( this )->name();
        }
        default:
        {
            return Constant::name( *this );
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
        case Value::BIT_CONSTANT:
        {
            static_cast< BitConstant* >( this )->accept( visitor );
            break;
        }
        case Value::STRING_CONSTANT:
        {
            static_cast< StringConstant* >( this )->accept( visitor );
            break;
        }
        case Value::FLOATING_CONSTANT:
        {
            static_cast< FloatingConstant* >( this )->accept( visitor );
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

void Constant::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    assert( this->defined() ); // PPA: foreach only valid if a constant is
                               // defined, please ensure the correct invocation,
                               // a universe shall not be undefined

    if( id() == Value::RANGE_CONSTANT )
    {
        static_cast< const RangeConstant* >( this )->foreach( callback );
    }
    else
    {
        callback( *this );
    }
}

Constant Constant::choose( void ) const
{
    assert( this->defined() ); // PPA: foreach only valid if a constant is
                               // defined, please ensure the correct invocation,
                               // a universe shall not be undefined

    if( id() == Value::RANGE_CONSTANT )
    {
        return static_cast< const RangeConstant* >( this )->choose();
    }
    else
    {
        return *this;
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
        case Value::BIT_CONSTANT:
        {
            return static_cast< const BitConstant* >( this )->hash();
        }
        case Value::STRING_CONSTANT:
        {
            return static_cast< const StringConstant* >( this )->hash();
        }
        case Value::FLOATING_CONSTANT:
        {
            return static_cast< const FloatingConstant* >( this )->hash();
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
        case Value::RULE_REFERENCE_CONSTANT:
        {
            return static_cast< const RuleReferenceConstant* >( this )->hash();
        }
        default:
        {
            break;
        }
    }

    assert( !" invalid constant to dispatch 'hash' found! " );
    return 0;
}

u1 Constant::operator==( const Value& rhs ) const
{
    switch( id() )
    {
        case Value::VOID_CONSTANT:
        {
            return static_cast< const VoidConstant* >( this )->operator==(
                rhs );
        }
        case Value::BOOLEAN_CONSTANT:
        {
            return static_cast< const BooleanConstant* >( this )->operator==(
                rhs );
        }
        case Value::INTEGER_CONSTANT:
        {
            return static_cast< const IntegerConstant* >( this )->operator==(
                rhs );
        }
        case Value::BIT_CONSTANT:
        {
            return static_cast< const BitConstant* >( this )->operator==( rhs );
        }
        case Value::STRING_CONSTANT:
        {
            return static_cast< const StringConstant* >( this )->operator==(
                rhs );
        }
        case Value::FLOATING_CONSTANT:
        {
            return static_cast< const FloatingConstant* >( this )->operator==(
                rhs );
        }
        case Value::RATIONAL_CONSTANT:
        {
            return static_cast< const RationalConstant* >( this )->operator==(
                rhs );
        }
        case Value::ENUMERATION_CONSTANT:
        {
            return static_cast< const EnumerationConstant* >( this )->
            operator==( rhs );
        }
        case Value::RANGE_CONSTANT:
        {
            return static_cast< const RangeConstant* >( this )->operator==(
                rhs );
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            return static_cast< const RuleReferenceConstant* >( this )->
            operator==( rhs );
        }
        default:
        {
            break;
        }
    }

    assert( !" invalid constant to dispatch 'operator==' found! " );
    return false;
}

u1 Constant::classof( Value const* obj )
{
    return obj->id() == classid() or VoidConstant::classof( obj )
           or BooleanConstant::classof( obj ) or IntegerConstant::classof( obj )
           or BitConstant::classof( obj ) or StringConstant::classof( obj )
           or FloatingConstant::classof( obj )
           or RationalConstant::classof( obj )
           or EnumerationConstant::classof( obj )
           or RuleReferenceConstant::classof( obj )
           or Identifier::classof( obj );
}

Constant Constant::undef( const Type::Ptr& type )
{
    switch( type->kind() )
    {
        case Type::Kind::LABEL:    // [[fallthrough]]
        case Type::Kind::LOCATION: // [[fallthrough]]
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
        case Type::Kind::BIT:
        {
            return BitConstant( std::static_pointer_cast< BitType >( type ) );
        }
        case Type::Kind::STRING:
        {
            return StringConstant();
        }
        case Type::Kind::FLOATING:
        {
            return FloatingConstant();
        }
        case Type::Kind::RATIONAL:
        {
            return RationalConstant();
        }
        case Type::Kind::ENUMERATION:
        {
            return EnumerationConstant(
                std::static_pointer_cast< EnumerationType >( type ) );
        }
        case Type::Kind::RANGE:
        {
            break;
        }
        case Type::Kind::TUPLE:
        {
            break;
        }
        case Type::Kind::LIST:
        {
            break;
        }
        case Type::Kind::RULE_REFERENCE:
        {
            return RuleReferenceConstant( type );
        }
        case Type::Kind::FUNCTION_REFERENCE:
        {
            break;
        }
        case Type::Kind::FILE:
        {
            break;
        }
        case Type::Kind::PORT:
        {
            break;
        }
        case Type::Kind::_TOP_:
        {
            break;
        }
    }

    throw std::domain_error( "unimplemented type '" + type->description()
                             + "' to create a 'undef' constant" );
    return VoidConstant();
}

//
//
// Constants
//

VoidConstant::VoidConstant( void )
: Constant( VOID, libstdhl::Type::Data( 0, false ), classid() )
{
}

std::string VoidConstant::name( void ) const
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

std::string BooleanConstant::name( void ) const
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

IntegerConstant::IntegerConstant(
    const std::string& value, const libstdhl::Type::Radix radix )
: Constant( INTEGER, libstdhl::Type::createInteger( value, radix ), classid() )
{
    // TODO: PPA: force CASM integer string digit separator usage as
    // group of
    // three
}

IntegerConstant::IntegerConstant( const BitConstant& value )
: Constant( INTEGER, value.value(), classid() )
{
}

IntegerConstant::IntegerConstant( const libstdhl::Type::Integer& value )
: Constant( INTEGER, value, classid() )
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

std::string IntegerConstant::name( void ) const
{
    return (
        defined() ? ( m_data.to< libstdhl::Type::DECIMAL >() ) : undef_str );
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
// Bit Constant
//

BitConstant::BitConstant(
    const std::string& value, const libstdhl::Type::Radix radix )
: Constant( libstdhl::Memory::get< BitType >( value, radix ),
      libstdhl::Type::createNatural( value, radix ), classid() )
{
    assert( this->type().isBit() );
    const auto& t = static_cast< const BitType& >( this->type() );
    if( t.bitsize() > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( t.bitsize() )
                                 + "' to create BitConstant" );
    }
}

BitConstant::BitConstant(
    const Type::Ptr& type, const libstdhl::Type::Natural& value )
: Constant( type, value, classid() )
{
    assert( this->type().isBit() );
    const auto& t = static_cast< const BitType& >( this->type() );

    if( t.bitsize() > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( t.bitsize() )
                                 + "' to create BitConstant" );
    }

    assert( value.trivial() and " TODO: PPA: FIXME:" );
    const u64 bitsize
        = static_cast< u64 >( std::log2(
              (double)( value.value() > 1 ? value.value() - 1 : 1 ) ) )
          + 1;

    if( bitsize > t.bitsize() )
    {
        throw std::invalid_argument( "value bit-size '"
                                     + std::to_string( bitsize )
                                     + "' does not fit into bit-size '"
                                     + std::to_string( t.bitsize() ) + "'" );
    }
}

BitConstant::BitConstant( const BitType::Ptr& type, u64 value )
: BitConstant( type, libstdhl::Type::createNatural( value ) )
{
}

BitConstant::BitConstant( const BitType::Ptr& type )
: Constant( type, classid() )
{
}

BitConstant::BitConstant( const u16 bitsize, const u64 value )
: BitConstant( libstdhl::Memory::get< BitType >( bitsize ), value )
{
}

BitConstant::BitConstant( const u16 bitsize )
: BitConstant( libstdhl::Memory::get< BitType >( bitsize ) )
{
}

u64 BitConstant::value_u64( void ) const
{
    return m_data.value();
}

const libstdhl::Type::Natural& BitConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::Natural& >( m_data );
}

std::string BitConstant::name( void ) const
{
    return (
        defined() ? ( m_data.to< libstdhl::Type::DECIMAL >() ) : undef_str );
}

void BitConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t BitConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( value() ) );
}

u1 BitConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const BitConstant& >( rhs );
    return ( this->defined() == other.defined() )
           and ( this->value() == other.value() );
}

u1 BitConstant::classof( Value const* obj )
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

std::string StringConstant::name( void ) const
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
    return ( this->defined() == other.defined() )
           and ( this->value() == other.value() );
}

u1 StringConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Floating Constant
//

FloatingConstant::FloatingConstant( const std::string& value )
: Constant( FLOATING, libstdhl::Type::createFloating( value ), classid() )
{
}

FloatingConstant::FloatingConstant( const double value )
: Constant( FLOATING, libstdhl::Type::createFloating( value ), classid() )
{
}

FloatingConstant::FloatingConstant( const libstdhl::Type::Floating& value )
: Constant( FLOATING, value, classid() )
{
}

FloatingConstant::FloatingConstant( void )
: Constant( FLOATING, classid() )
{
}

const libstdhl::Type::Floating& FloatingConstant::value( void ) const
{
    return static_cast< const libstdhl::Type::Floating& >( m_data );
}

std::string FloatingConstant::name( void ) const
{
    return ( defined() ? ( "TODO" ) : undef_str );
    // TODO: PPA: use literal function from libstdhl::Type::Data
}

void FloatingConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t FloatingConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, libstdhl::Hash::value( value() ) );
}

u1 FloatingConstant::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const FloatingConstant& >( rhs );
    return ( this->defined() == other.defined() )
           and ( this->value() == other.value() );
}

u1 FloatingConstant::classof( Value const* obj )
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

std::string RationalConstant::name( void ) const
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
: Constant( type, libstdhl::Type::createNatural( type->kind().encode( value ) ),
      classid() )
{
}

EnumerationConstant::EnumerationConstant(
    const Enumeration::Ptr& kind, const std::string& value )
: Constant( libstdhl::Memory::get< EnumerationType >( kind ),
      libstdhl::Type::createNatural( kind->encode( value ) ), classid() )
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

std::string EnumerationConstant::name( void ) const
{
    return static_cast< const EnumerationType& >( type() ).kind().decode(
        m_data.value() );
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
    return ( this->defined() == other.defined() )
           and ( this->value() == other.value() );
}

u1 EnumerationConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Range Constant
//

RangeConstant::RangeConstant(
    const RangeType::Ptr& type, const Range::Ptr& value )
: Constant( type, libstdhl::Type::Data( 0, false ), classid() )
{
    static_cast< RangeType& >( *type ).setRange( value );
}

RangeConstant::RangeConstant( const RangeType::Ptr& type )
: Constant( type, classid() )
{
}

RangeConstant::RangeConstant(
    const Type::Ptr& type, const Constant& from, const Constant& to )
: Constant( type, libstdhl::Type::Data( 0, false ), classid() )
{
    assert( type->isRange() );
    static_cast< RangeType& >( *type ).setRange(
        libstdhl::Memory::make_unique< Range >( from, to ) );
}

Range::Ptr RangeConstant::value( void ) const
{
    return static_cast< const RangeType& >( type() ).ptr_range();
}

std::string RangeConstant::name( void ) const
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
    return ( this->defined() == other.defined() )
           and ( this->value() == other.value() );
}

u1 RangeConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rule Reference Constant
//

RuleReferenceConstant::RuleReferenceConstant( const Rule::Ptr& value )
: ReferenceConstant( std::static_pointer_cast< Type >(
                         libstdhl::Memory::make< RuleReferenceType >(
                             std::static_pointer_cast< RelationType >(
                                 value->type().ptr_type() ) ) ),
      value.get(), classid() )
{
}

RuleReferenceConstant::RuleReferenceConstant( const Type::Ptr& type )
: ReferenceConstant( type, classid() )
{
    assert( type->isRuleReference() );
}

std::string RuleReferenceConstant::name( void ) const
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
    return ( this->defined() == other.defined() )
           and ( this->value() == other.value() );
}

u1 RuleReferenceConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Identifier
//

Identifier::Identifier( const Type::Ptr& type, const std::string& value )
: Constant( type, libstdhl::Type::createString( value ), classid() )
{
}

std::string Identifier::name( void ) const
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
    return ( this->defined() == other.defined() )
           and ( this->name() == other.name() );
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

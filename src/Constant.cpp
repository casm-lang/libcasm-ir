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

#include "Constant.h"

#include "../stdhl/cpp/FloatingPoint.h"
#include "../stdhl/cpp/Math.h"
#include "../stdhl/cpp/Rational.h"

using namespace libcasm_ir;

static constexpr const char* undef_str = "undef";
static const std::string EMPTY = "";

static const auto VOID = libstdhl::get< VoidType >();
static const auto BOOLEAN = libstdhl::get< BooleanType >();
static const auto INTEGER = libstdhl::get< IntegerType >();
static const auto STRING = libstdhl::get< StringType >();
static const auto FLOATING = libstdhl::get< FloatingType >();
static const auto RATIONAL = libstdhl::get< RationalType >();

Constant::Constant( const std::string& name, const Type::Ptr& type,
    const libstdhl::Type& data, const Value::Ptr& value, u1 defined,
    u1 symbolic, Value::ID id )
: Value( name, type, id )
, m_data( data )
, m_value( value )
, m_defined( defined )
, m_symbolic( symbolic )
{
}

Constant::Constant( const Type::Ptr& type, const libstdhl::Type& data,
    const Value::Ptr& value, u1 defined, u1 symbolic, Value::ID id )
: Value( EMPTY, type, id )
, m_data( data )
, m_value( value )
, m_defined( defined )
, m_symbolic( symbolic )
{
}

Constant::Constant( void )
: Constant( VOID, libstdhl::Type(), nullptr, false, false, classid() )
{
}

u1 Constant::defined( void ) const
{
    return m_defined;
}

u1 Constant::symbolic( void ) const
{
    return m_symbolic;
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
            assert( !" invalid constant to dispatch found! " );
            return "";
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
        }
        case Value::BOOLEAN_CONSTANT:
        {
            static_cast< BooleanConstant* >( this )->accept( visitor );
        }
        case Value::INTEGER_CONSTANT:
        {
            static_cast< IntegerConstant* >( this )->accept( visitor );
        }
        case Value::BIT_CONSTANT:
        {
            static_cast< BitConstant* >( this )->accept( visitor );
        }
        case Value::STRING_CONSTANT:
        {
            static_cast< StringConstant* >( this )->accept( visitor );
        }
        case Value::FLOATING_CONSTANT:
        {
            static_cast< FloatingConstant* >( this )->accept( visitor );
        }
        case Value::RATIONAL_CONSTANT:
        {
            static_cast< RationalConstant* >( this )->accept( visitor );
        }
        case Value::ENUMERATION_CONSTANT:
        {
            static_cast< EnumerationConstant* >( this )->accept( visitor );
        }
        case Value::RANGE_CONSTANT:
        {
            static_cast< RangeConstant* >( this )->accept( visitor );
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            static_cast< RuleReferenceConstant* >( this )->accept( visitor );
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
    switch( type->id() )
    {
        case Type::ID::_BOTTOM_: // [[fallthrough]]
        case Type::ID::LABEL:    // [[fallthrough]]
        case Type::ID::LOCATION: // [[fallthrough]]
        case Type::ID::RELATION:
        {
            break;
        }
        case Type::ID::VOID:
        {
            return VoidConstant();
        }
        case Type::ID::BOOLEAN:
        {
            return BooleanConstant();
        }
        case Type::ID::INTEGER:
        {
            return IntegerConstant();
        }
        case Type::ID::BIT:
        {
            return BitConstant( std::static_pointer_cast< BitType >( type ) );
        }
        case Type::ID::STRING:
        {
            return StringConstant();
        }
        case Type::ID::FLOATING:
        {
            return FloatingConstant();
        }
        case Type::ID::RATIONAL:
        {
            return RationalConstant();
        }
        case Type::ID::ENUMERATION:
        {
            return EnumerationConstant(
                std::static_pointer_cast< EnumerationType >( type ) );
        }
        case Type::ID::RANGE:
        {
            break;
        }
        case Type::ID::TUPLE:
        {
            break;
        }
        case Type::ID::LIST:
        {
            break;
        }
        case Type::ID::RULE_REFERENCE:
        {
            return RuleReferenceConstant( type );
        }
        case Type::ID::FUNCTION_REFERENCE:
        {
            break;
        }
        case Type::ID::FILE:
        {
            break;
        }
        case Type::ID::PORT:
        {
            break;
        }
        case Type::ID::_TOP_:
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
: Constant( VOID, libstdhl::Type(), nullptr, true, false, classid() )
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

u1 VoidConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Boolean Constant
//

BooleanConstant::BooleanConstant( u1 value, u1 defined, u1 symbolic )
: Constant(
      BOOLEAN, libstdhl::Type( value ), nullptr, defined, symbolic, classid() )
{
}

BooleanConstant::BooleanConstant( u1 value )
: BooleanConstant( value, true, false )
{
}

BooleanConstant::BooleanConstant( void )
: BooleanConstant( false, false, false )
{
}

u1 BooleanConstant::value( void ) const
{
    return static_cast< u1 >( m_data.word( 0 ) );
}

std::string BooleanConstant::name( void ) const
{
    return ( defined() ? ( value() ? "true" : "false" ) : undef_str );
}

void BooleanConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

std::size_t BooleanConstant::hash( void ) const
{
    const auto h = ( ( (std::size_t)classid() ) << 1 ) | defined();
    return libstdhl::Hash::combine( h, value() );
}

u1 BooleanConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Integer Constant
//

IntegerConstant::IntegerConstant(
    const libstdhl::Type& value, u1 defined, u1 symbolic )
: Constant( INTEGER, value, nullptr, defined, symbolic, classid() )
{
}

IntegerConstant::IntegerConstant(
    const std::string& value, const libstdhl::Type::Radix radix )
: IntegerConstant( libstdhl::Integer( value, radix ), true, false )
{
    // TODO: PPA: force CASM integer string digit separator usage as
    // group of
    // three
}

IntegerConstant::IntegerConstant( const BitConstant& value )
: IntegerConstant( value.value(), true, false )
{
}

IntegerConstant::IntegerConstant( const libstdhl::Integer& value )
: IntegerConstant( value, true, false )
{
}

IntegerConstant::IntegerConstant( i64 value )
: IntegerConstant( libstdhl::Integer( value ), true, false )
{
}

IntegerConstant::IntegerConstant( void )
: IntegerConstant( libstdhl::Type(), false, false )
{
}

i64 IntegerConstant::value_i64( void ) const
{
    i64 v = ( i64 )( m_data.word( 0 ) );

    if( m_data.sign() )
    {
        return v * -1;
    }
    else
    {
        return v;
    }
}

const libstdhl::Integer& IntegerConstant::value( void ) const
{
    return static_cast< const libstdhl::Integer& >( m_data );
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

u1 IntegerConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Bit Constant
//

BitConstant::BitConstant(
    const BitType::Ptr& type, u64 value, u1 defined, u1 symbolic )
: Constant(
      type, libstdhl::Type( value ), nullptr, defined, symbolic, classid() )
{
    if( type->bitsize() > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( type->bitsize() )
                                 + "' to create BitConstant" );
    }
}

BitConstant::BitConstant( const BitType::Ptr& type, const std::string& value,
    const libstdhl::Type::Radix radix )
: Constant( type, libstdhl::Type( value, false, radix ), nullptr, true, false,
      classid() )
{
    if( type->bitsize() > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( type->bitsize() )
                                 + "' to create BitConstant" );
    }
}

BitConstant::BitConstant( const Type::Ptr& type, const libstdhl::Type& value )
: Constant( type, value, nullptr, true, false, classid() )
{
    assert( type->isBit() );
    const auto& t = static_cast< const BitType& >( *type );
    if( t.bitsize() > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( t.bitsize() )
                                 + "' to create BitConstant" );
    }
}

BitConstant::BitConstant( const BitType::Ptr& type, u64 value )
: BitConstant( type, value, true, false )
{
}

BitConstant::BitConstant( const BitType::Ptr& type )
: BitConstant( type, 0, false, false )
{
}

BitConstant::BitConstant(
    const std::string& value, const libstdhl::Type::Radix radix )
: BitConstant( libstdhl::get< BitType >( value, radix ), value, radix )
{
}

BitConstant::BitConstant( u16 bitsize, u64 value )
: BitConstant( libstdhl::get< BitType >( bitsize ), value )
{
}

BitConstant::BitConstant( u16 bitsize )
: BitConstant( libstdhl::get< BitType >( bitsize ) )
{
}

u64 BitConstant::value_u64( void ) const
{
    return m_data.word( 0 );
}

const libstdhl::Type& BitConstant::value( void ) const
{
    return m_data;
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

u1 BitConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// String Constant
//

StringConstant::StringConstant(
    const std::string& value, u1 defined, u1 symbolic )
: Constant(
      value, STRING, libstdhl::Type(), nullptr, defined, symbolic, classid() )
{
}

StringConstant::StringConstant( const std::string& value )
: StringConstant( value, true, false )
{
}

StringConstant::StringConstant( void )
: StringConstant( undef_str, false, false )
{
}

std::string StringConstant::value( void ) const
{
    return name();
}

std::string StringConstant::name( void ) const
{
    return _name();
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

u1 StringConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Floating Constant
//

FloatingConstant::FloatingConstant(
    const libstdhl::Type& value, u1 defined, u1 symbolic )
: Constant( FLOATING, value, nullptr, defined, symbolic, classid() )
{
}

FloatingConstant::FloatingConstant( const libstdhl::Type& value )
: FloatingConstant( value, true, false )
{
}

FloatingConstant::FloatingConstant( const std::string& value )
: FloatingConstant( libstdhl::FloatingPoint( value ), true, false )
{
}

FloatingConstant::FloatingConstant( const double value )
: FloatingConstant( libstdhl::FloatingPoint( value ), true, false )
{
}

FloatingConstant::FloatingConstant( void )
: FloatingConstant( libstdhl::Type(), false, false )
{
}

const libstdhl::FloatingPoint& FloatingConstant::value( void ) const
{
    return static_cast< const libstdhl::FloatingPoint& >( m_data );
}

std::string FloatingConstant::name( void ) const
{
    return ( defined() ? ( "TODO" ) : undef_str );
    // TODO: PPA: use literal function from libstdhl::Type
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

u1 FloatingConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rational Constant
//

RationalConstant::RationalConstant(
    const libstdhl::Type& value, u1 defined, u1 symbolic )
: Constant( RATIONAL, value, nullptr, defined, symbolic, classid() )
{
}

RationalConstant::RationalConstant( const libstdhl::Type& value )
: RationalConstant( value, true, false )
{
}

RationalConstant::RationalConstant( const std::string& value )
: RationalConstant( libstdhl::Rational( value ), true, false )
{
}

RationalConstant::RationalConstant( void )
: RationalConstant( libstdhl::Type(), false, false )
{
}

const libstdhl::Rational& RationalConstant::value( void ) const
{
    return static_cast< const libstdhl::Rational& >( m_data );
}

std::string RationalConstant::name( void ) const
{
    return ( defined() ? ( "TODO" ) : undef_str );
    // TODO: PPA: use literal function from libstdhl::Type
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

u1 RationalConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Enumeration Constant
//

EnumerationConstant::EnumerationConstant( const EnumerationType::Ptr& type,
    const std::string& value, u1 defined, u1 symbolic, Value::ID id )
: Constant( value, type,
      defined ? libstdhl::Type( type->kind().encode( value ) )
              : libstdhl::Type(),
      nullptr, defined, symbolic, id )
{
}

EnumerationConstant::EnumerationConstant(
    const EnumerationType::Ptr& type, const std::string& value )
: EnumerationConstant( type, value, true, false )
{
}

EnumerationConstant::EnumerationConstant( const EnumerationType::Ptr& type )
: EnumerationConstant( type, "undef", false, false )
{
}

EnumerationConstant::EnumerationConstant(
    const Enumeration::Ptr& kind, const std::string& value )
: EnumerationConstant( libstdhl::get< EnumerationType >( kind ), value )
{
}

EnumerationConstant::EnumerationConstant( const Enumeration::Ptr& kind )
: EnumerationConstant( libstdhl::get< EnumerationType >( kind ) )
{
}

const libstdhl::Type& EnumerationConstant::value( void ) const
{
    return m_data;
}

std::string EnumerationConstant::name( void ) const
{
    return _name();
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

u1 EnumerationConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Range Constant
//

RangeConstant::RangeConstant( const Type::Ptr& type, const Range::Ptr& value,
    u1 defined, u1 symbolic, Value::ID id )
: Constant( type, libstdhl::Type(), value, defined, symbolic, id )
{
}

RangeConstant::RangeConstant(
    const RangeType::Ptr& type, const Range::Ptr& value )
: RangeConstant( type, value, true, false )
{
}

RangeConstant::RangeConstant( const RangeType::Ptr& type )
: RangeConstant( type, type->ptr_range() ? type->ptr_range() : nullptr,
      type->ptr_range() ? true : false, false )
{
}

RangeConstant::RangeConstant(
    const Type::Ptr& type, const Constant& from, const Constant& to )
: RangeConstant( type, libstdhl::make_unique< Range >( from, to ), true, false )
{
    assert( type->isRange() );
    static_cast< RangeType& >( *type ).setRange(
        std::static_pointer_cast< Range >( m_value ) );
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

u1 RangeConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rule Reference Constant
//

RuleReferenceConstant::RuleReferenceConstant(
    const Type::Ptr& type, const Rule::Ptr& value, u1 defined, u1 symbolic )
: ReferenceConstant< Rule >( type, value, defined, symbolic, classid() )
{
}

RuleReferenceConstant::RuleReferenceConstant( const Rule::Ptr& value )
: RuleReferenceConstant( value->ptr_type(), value, true, false )
{
}

RuleReferenceConstant::RuleReferenceConstant( const Type::Ptr& type )
: RuleReferenceConstant( type, nullptr, false, false )
{
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

u1 RuleReferenceConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Identifier
//

Identifier::Identifier( const std::string& value, const Type::Ptr& type )
: Constant( value, type, libstdhl::Type(), nullptr, true, false, classid() )
{
}

std::string Identifier::name( void ) const
{
    return _name();
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

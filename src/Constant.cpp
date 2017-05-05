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

Constant::Constant( void )
: Constant( "", libstdhl::get< VoidType >(), libstdhl::Type(), nullptr, false,
      false, classid() )
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
        case Value::RULE_REFERENCE_CONSTANT:
        {
            static_cast< RuleReferenceConstant* >( this )->accept( visitor );
        }
        default:
        {
            assert( !" invalid constant to dispatch found! " );
        }
    }
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
        case Type::ID::RULE_REFERENCE:
        {
            return RuleReferenceConstant( type );
        }

        case Type::ID::_BOTTOM_: // [[fallthrough]]
        case Type::ID::_TOP_:    // [[fallthrough]]
        case Type::ID::RANGE:    // [[fallthrough]]
        case Type::ID::LABEL:    // [[fallthrough]]
        case Type::ID::LOCATION: // [[fallthrough]]
        case Type::ID::RELATION: // [[fallthrough]]
        case Type::ID::FUNCTION_REFERENCE:
        {
            throw std::domain_error( "unimplemented type '"
                                     + type->description()
                                     + "' to create a 'undef' constant" );
            break;
        }
    }

    return VoidConstant();
}

//
//
// Constants
//

VoidConstant::VoidConstant( void )
: Constant( "", libstdhl::get< VoidType >(), libstdhl::Type(), nullptr, true,
      false, classid() )
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

u1 VoidConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Boolean Constant
//

BooleanConstant::BooleanConstant( u1 value, u1 defined, u1 symbolic )
: Constant( "", libstdhl::get< BooleanType >(), libstdhl::Type( value, 1 ),
      nullptr, defined, symbolic, classid() )
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

u1 BooleanConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Integer Constant
//

IntegerConstant::IntegerConstant( i64 value, u1 defined, u1 symbolic )
: Constant( "", libstdhl::get< IntegerType >(), libstdhl::Integer( value ),
      nullptr, defined, symbolic, classid() )
{
}

IntegerConstant::IntegerConstant(
    const std::string& value, const libstdhl::Type::Radix radix )
: Constant( "", libstdhl::get< IntegerType >(),
      libstdhl::Integer( value, radix ), nullptr, true, false, classid() )
{
    // TODO: PPA: force CASM integer string digit separator usage as
    // group of
    // three
}

IntegerConstant::IntegerConstant( const BitConstant& value )
: Constant( "", libstdhl::get< IntegerType >(), value.value(), nullptr, true,
      false, classid() )
{
}

IntegerConstant::IntegerConstant( const libstdhl::Integer& value )
: Constant( "", libstdhl::get< IntegerType >(), value, nullptr, true, false,
      classid() )
{
}

IntegerConstant::IntegerConstant( i64 value )
: IntegerConstant( value, true, false )
{
}

IntegerConstant::IntegerConstant( void )
: IntegerConstant( 0, false, false )
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

u1 IntegerConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Bit Constant
//

BitConstant::BitConstant(
    const BitType::Ptr& type, u64 value, u1 defined, u1 symbolic )
: Constant( "", type, libstdhl::Type( value, type->bitsize() ), nullptr,
      defined, symbolic, classid() )
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
: Constant( "", type, libstdhl::Type( value, type->bitsize(), false, radix ),
      nullptr, true, false, classid() )
{
    if( type->bitsize() > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( type->bitsize() )
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

u1 BitConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// String Constant
//

StringConstant::StringConstant(
    const std::string& value, u1 defined, u1 symbolic )
: Constant( value, libstdhl::get< StringType >(), libstdhl::Type(), nullptr,
      defined, symbolic, classid() )
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

u1 StringConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Floating Constant
//

FloatingConstant::FloatingConstant(
    const double value, u1 defined, u1 symbolic )
: Constant( "", libstdhl::get< FloatingType >(),
      libstdhl::FloatingPoint( value ), nullptr, defined, symbolic, classid() )
{
}

FloatingConstant::FloatingConstant( const std::string& value )
: Constant( "", libstdhl::get< FloatingType >(),
      libstdhl::FloatingPoint( value ), nullptr, true, false, classid() )
{
}

FloatingConstant::FloatingConstant( const double value )
: FloatingConstant( value, true, false )
{
}

FloatingConstant::FloatingConstant( void )
: FloatingConstant( 0.0, false, false )
{
}

double FloatingConstant::value( void ) const
{
    std::cerr << __FILE__ << ":" << __LINE__ << ": TODO: FIXME\n";
    return 0.0;
}

std::string FloatingConstant::name( void ) const
{
    return ( defined() ? ( std::to_string( value() ) ) : undef_str );
    // TODO: PPA: use literal function from libstdhl::Type
}

void FloatingConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 FloatingConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rational Constant
//

RationalConstant::RationalConstant(
    const std::string& value, u1 defined, u1 symbolic )
: Constant( "", libstdhl::get< RationalType >(), libstdhl::Rational( value ),
      nullptr, defined, symbolic, classid() )
{
}

RationalConstant::RationalConstant( const std::string& value )
: RationalConstant( value, true, false )
{
}

RationalConstant::RationalConstant( const libstdhl::Rational& value )
: Constant( "", libstdhl::get< RationalType >(), value, nullptr, true, false,
      classid() )
{
}

RationalConstant::RationalConstant( void )
: RationalConstant( nullptr, false, false )
{
}

std::string RationalConstant::value( void ) const
{
    return "";
    // TODO: PPA: use literal function from libstdhl::Type
}

std::string RationalConstant::name( void ) const
{
    return ( defined() ? ( value() ) : undef_str );
    // TODO: PPA: use literal function from libstdhl::Type
}

void RationalConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
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
      defined ? libstdhl::Type( type->kind().encode( value ), 64 )
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

u64 EnumerationConstant::value( void ) const
{
    return m_data.word( 0 );
}

std::string EnumerationConstant::name( void ) const
{
    return (
        defined()
            ? ( static_cast< const EnumerationType& >( type() ).kind().decode(
                  value() ) )
            : undef_str );
}

void EnumerationConstant::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 EnumerationConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rule Reference Constant
//

RuleReferenceConstant::RuleReferenceConstant(
    const Type::Ptr& type, const Rule::Ptr& value, u1 defined, u1 symbolic )
: ReferenceConstant< Rule >( "", type, value, defined, symbolic, classid() )
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

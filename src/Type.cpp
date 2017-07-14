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

#include "Type.h"

#include "Constant.h"
#include "Enumeration.h"
#include "Exception.h"
#include "Range.h"

#include "../stdhl/cpp/Random.h"

using namespace libcasm_ir;

static const auto VOID_TYPE = libstdhl::get< VoidType >();

Type::Type( Type::ID id )
: m_id( id )
{
}

Type::ID Type::id( void ) const
{
    return m_id;
}

const Type& Type::result( void ) const
{
    if( isRelation() or isRange() )
    {
        return *m_result.get();
    }

    return *this;
}

Type::Ptr Type::ptr_result( void ) const
{
    if( isRelation() or isRange() )
    {
        return m_result;
    }

    return ptr_this< Type >();
}

Type::Ptr Type::ptr_type( void ) const
{
    return ptr_this< Type >();
}

const Types& Type::arguments( void ) const
{
    return m_arguments;
}

std::string Type::make_hash( void ) const
{
    return "t:" + std::to_string( id() ) + ":" + description();
}

u1 Type::isSynthetic( void ) const
{
    return isVoid() or isLabel() or isLocation() or isRelation();
}

u1 Type::isVoid( void ) const
{
    return id() == Type::VOID;
}

u1 Type::isLabel( void ) const
{
    return id() == Type::LABEL;
}

u1 Type::isLocation( void ) const
{
    return id() == Type::LOCATION;
}

u1 Type::isRelation( void ) const
{
    return id() == Type::RELATION;
}

u1 Type::isPrimitive( void ) const
{
    return isBoolean() or isInteger() or isBit() or isString() or isFloating()
           or isRational();
}

u1 Type::isBoolean( void ) const
{
    return id() == Type::BOOLEAN;
}

u1 Type::isInteger( void ) const
{
    return id() == Type::INTEGER;
}

u1 Type::isBit( void ) const
{
    return id() == Type::BIT;
}

u1 Type::isString( void ) const
{
    return id() == Type::STRING;
}

u1 Type::isFloating( void ) const
{
    return id() == Type::FLOATING;
}

u1 Type::isRational( void ) const
{
    return id() == Type::RATIONAL;
}

u1 Type::isComposed( void ) const
{
    return isEnumeration() or isRange() or isTuple() or isList();
}

u1 Type::isEnumeration( void ) const
{
    return id() == Type::ENUMERATION;
}

u1 Type::isRange( void ) const
{
    return id() == Type::RANGE;
}

u1 Type::isTuple( void ) const
{
    return id() == Type::TUPLE;
}

u1 Type::isList( void ) const
{
    return id() == Type::LIST;
}

u1 Type::isReference( void ) const
{
    return isRuleReference() or isFunctionReference();
}

u1 Type::isRuleReference( void ) const
{
    return id() == Type::RULE_REFERENCE;
}

u1 Type::isFunctionReference( void ) const
{
    return id() == Type::FUNCTION_REFERENCE;
}

u1 Type::isAbstraction( void ) const
{
    return isFile() or isPort();
}

u1 Type::isFile( void ) const
{
    return id() == Type::FILE;
}

u1 Type::isPort( void ) const
{
    return id() == Type::PORT;
}

std::string Type::token( const Type::ID id )
{
    switch( id )
    {
        case _BOTTOM_:
        {
            return "_BOTTOM_";
        }
        // synthetic
        case VOID:
        {
            return "Void";
        }
        case LABEL:
        {
            return "Label";
        }
        case LOCATION:
        {
            return "Location";
        }
        case RELATION:
        {
            return "Relation";
        }
        // primitive
        case BOOLEAN:
        {
            return "Boolean";
        }
        case INTEGER:
        {
            return "Integer";
        }
        case BIT:
        {
            return "Bit";
        }
        case STRING:
        {
            return "String";
        }
        case FLOATING:
        {
            return "Floating";
        }
        case RATIONAL:
        {
            return "Rational";
        }
        // composed
        case ENUMERATION:
        {
            return "Enumeration";
        }
        case RANGE:
        {
            return "Range";
        }
        case TUPLE:
        {
            return "Tuple";
        }
        case LIST:
        {
            return "List";
        }
        // reference
        case RULE_REFERENCE:
        {
            return "RuleRef";
        }
        case FUNCTION_REFERENCE:
        {
            return "FuncRef";
        }
        // abstraction
        case FILE:
        {
            return "File";
        }
        case PORT:
        {
            return "Port";
        }
        case _TOP_:
        {
            return "_TOP_";
        }
    }

    assert( !" internal error " );
    return "";
}

//
//
// Synthetic Type
//

SyntheticType::SyntheticType( Type::ID id )
: Type( id )
{
}

//
// Void Type
//

VoidType::VoidType( void )
: SyntheticType( Type::VOID )
{
}

std::string VoidType::name( void ) const
{
    return "v";
}

std::string VoidType::description( void ) const
{
    return token( id() );
}

void VoidType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has no range to process
}

Constant VoidType::choose( void ) const
{
    return VoidConstant();
}

void VoidType::validate( const Constant& constant ) const
{
    assert( isa< VoidConstant >( constant ) );
}

std::size_t VoidType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
// Label Type
//

LabelType::LabelType( void )
: SyntheticType( Type::LABEL )
{
}

std::string LabelType::name( void ) const
{
    return "label";
}

std::string LabelType::description( void ) const
{
    return token( id() );
}

void LabelType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has no range to process
}

Constant LabelType::choose( void ) const
{
    return VoidConstant();
}

void LabelType::validate( const Constant& constant ) const
{
    // TODO: enable line if there will be a value LabelConstant
    // assert( isa< LabelConstant >( constant ) );
}

std::size_t LabelType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
// Location Type
//

LocationType::LocationType( void )
: SyntheticType( Type::LOCATION )
{
}

std::string LocationType::name( void ) const
{
    return "loc";
}

std::string LocationType::description( void ) const
{
    return token( id() );
}

void LocationType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has no range to process
}

Constant LocationType::choose( void ) const
{
    return VoidConstant();
}

void LocationType::validate( const Constant& constant ) const
{
    // TODO: assert( isa< LocationConstant >( constant ) );
}

std::size_t LocationType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Relation Type
//

RelationType::RelationType( const Type::Ptr& result, const Types& arguments )
: SyntheticType( Type::RELATION )
{
    m_result = result;
    m_arguments = arguments;

#ifndef NDEBUG
    assert( result );
    for( auto argument : arguments )
    {
        assert( argument );
    }
#endif
}

std::string RelationType::name( void ) const
{
    std::string tmp = "<";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        if( not first )
        {
            tmp += ", ";
        }

        tmp += argument->name();

        first = false;
    }

    tmp += " -> " + m_result->name() + ">";

    return tmp;
}

std::string RelationType::description( void ) const
{
    std::string tmp = "< ";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        if( not first )
        {
            tmp += " * ";
        }

        tmp += argument->description();

        first = false;
    }

    tmp += " -> " + m_result->description() + " >";

    return tmp;
}

void RelationType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant RelationType::choose( void ) const
{
    return VoidConstant();
}

void RelationType::validate( const Constant& constant ) const
{
    // relation constants are not possible, nothing to validate here!
}

std::size_t RelationType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Primitive Type
//

PrimitiveType::PrimitiveType( Type::ID id )
: Type( id )
{
}

//
// Boolean Type
//

BooleanType::BooleanType( void )
: PrimitiveType( Type::BOOLEAN )
{
}

std::string BooleanType::name( void ) const
{
    return "b";
}

std::string BooleanType::description( void ) const
{
    return token( id() );
}

void BooleanType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    callback( BooleanConstant( false ) );
    callback( BooleanConstant( true ) );
}

Constant BooleanType::choose( void ) const
{
    auto const value = ( u1 )( libstdhl::Random::uniform< u64 >() % 2 );
    return BooleanConstant( value );
}

void BooleanType::validate( const Constant& constant ) const
{
    assert( isa< BooleanConstant >( constant ) );
}

std::size_t BooleanType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
// Integer Type
//

IntegerType::IntegerType( void )
: PrimitiveType( Type::INTEGER )
, m_range( nullptr )
{
}

IntegerType::IntegerType( const RangeType::Ptr& range )
: PrimitiveType( Type::INTEGER )
, m_range( range )
{
    assert( range );

    if( not range->type().isInteger() )
    {
        throw std::domain_error(
            "range '" + range->name() + "' has to be of type 'Integer'" );
    }

    // if( TODO )
    // {
    //     throw std::domain_error(
    //         "range '"
    //         + range->name()
    //         + "' violates monotonically nondecreasing property of 'Integer'
    //         type" );
    // }
}

u1 IntegerType::constrained( void ) const
{
    return m_range ? true : false;
}

RangeType::Ptr IntegerType::range( void ) const
{
    return m_range;
}

std::string IntegerType::name( void ) const
{
    return "i" + ( m_range ? m_range->name() : "" );
}

std::string IntegerType::description( void ) const
{
    if( not m_range )
    {
        return token( id() );
    }
    else
    {
        return token( id() ) + "'" + m_range->name();
    }
}

void IntegerType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    if( constrained() )
    {
        m_range->foreach( callback );
    }
    else
    {
        // this type has an infinite range to process, therefore omitted (for
        // now)
    }
}

Constant IntegerType::choose( void ) const
{
    if( constrained() )
    {
        return m_range->choose();
    }
    else
    {
        return IntegerConstant( libstdhl::Random::uniform< i64 >() );
        // LIMITATION: currently we only address the 64-bit range for
        // this choosing value range, can be extended later even to address
        // bigger
        // randomized values
    }
}

void IntegerType::validate( const Constant& constant ) const
{
    assert( isa< IntegerConstant >( constant ) );

    if( constrained() )
    {
        return m_range->validate( constant );
    }
}

std::size_t IntegerType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
// Bit Type
//

BitType::BitType( u16 bitsize )
: PrimitiveType( Type::BIT )
, m_bitsize( bitsize )
{
    if( m_bitsize < 1 or m_bitsize > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
}

BitType::BitType( const IntegerConstant::Ptr& bitsize )
: PrimitiveType( Type::BIT )
{
    assert( bitsize );
    if( bitsize->value().value() > BitType::SizeMax or bitsize->value().sign() )
    {
        throw std::domain_error(
            "invalid bit size '" + bitsize->name() + "' for 'Bit' type" );
    }

    m_bitsize = bitsize->value_i64();

    if( m_bitsize < 1 )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
}

BitType::BitType( const std::string& value, const libstdhl::Type::Radix radix )
: PrimitiveType( Type::BIT )
{
    std::string tmp = value;
    tmp.erase( std::remove( tmp.begin(), tmp.end(), '\'' ), tmp.end() );

    m_bitsize = (u16)tmp.size() * std::log2( (double)radix );

    if( m_bitsize < 1 or m_bitsize > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
}

u16 BitType::bitsize( void ) const
{
    return m_bitsize;
}

std::string BitType::name( void ) const
{
    return "u" + std::to_string( m_bitsize );
}

std::string BitType::description( void ) const
{
    return token( id() ) + "'" + std::to_string( m_bitsize );
}

void BitType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an (depending on the current bit-size) infinite range to
    // process, therefore omitted (for now)
}

Constant BitType::choose( void ) const
{
    return BitConstant( m_bitsize,
        libstdhl::Random::uniform< u64 >()
            % m_bitsize ); // TODO: FIXME: PPA: fix the randomized value modulo
                           // mapping to full range not only the bitsize
}

void BitType::validate( const Constant& constant ) const
{
    assert( isa< BitConstant >( constant ) );

    const auto& c = static_cast< const BitConstant& >( constant );
    assert( c.type().isBit() );
    const auto& t = static_cast< const BitType& >( c.type() );

    if( m_bitsize < t.bitsize() )
    {
        throw ValidationException( " type " + t.description()
                                   + " of constant is invalid for type "
                                   + this->description() );
    }
}

std::size_t BitType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
// String Type
//

StringType::StringType( void )
: PrimitiveType( Type::STRING )
{
}

std::string StringType::name( void ) const
{
    return "s";
}

std::string StringType::description( void ) const
{
    return token( id() );
}

void StringType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant StringType::choose( void ) const
{
    // this is undefined for now
    return StringConstant();
}

void StringType::validate( const Constant& constant ) const
{
    assert( isa< StringConstant >( constant ) );
}

std::size_t StringType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
// Flaoting Type
//

FloatingType::FloatingType( void )
: PrimitiveType( Type::FLOATING )
{
}

std::string FloatingType::name( void ) const
{
    return "z";
}

std::string FloatingType::description( void ) const
{
    return token( id() );
}

void FloatingType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant FloatingType::choose( void ) const
{
    // this is undefined for now
    return FloatingConstant();
}

void FloatingType::validate( const Constant& constant ) const
{
    assert( isa< FloatingConstant >( constant ) );
}

std::size_t FloatingType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
// Rational Type
//

RationalType::RationalType( void )
: PrimitiveType( Type::RATIONAL )
{
}

std::string RationalType::name( void ) const
{
    return "q";
}

std::string RationalType::description( void ) const
{
    return token( id() );
}

void RationalType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant RationalType::choose( void ) const
{
    const auto n
        = libstdhl::Type::Integer( libstdhl::Random::uniform< i64 >() );
    const auto d
        = libstdhl::Type::Integer( libstdhl::Random::uniform< i64 >() + 1 );
    // d = randomvalue + 1 to avoid that the denominator is zero!

    return RationalConstant( libstdhl::Type::Rational( n, d ) );
}

void RationalType::validate( const Constant& constant ) const
{
    assert( isa< RationalConstant >( constant ) );
}

std::size_t RationalType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Composed Type
//

ComposedType::ComposedType( Type::ID id )
: Type( id )
{
}

//
// Enumeration Type
//

EnumerationType::EnumerationType( const Enumeration::Ptr& kind )
: ComposedType( Type::ENUMERATION )
, m_kind( kind )
{
}

Enumeration& EnumerationType::kind( void ) const
{
    return *m_kind.get();
}

Enumeration::Ptr EnumerationType::ptr_kind( void ) const
{
    return m_kind;
}

std::string EnumerationType::name( void ) const
{
    return m_kind->name();
}

std::string EnumerationType::description( void ) const
{
    return m_kind->name();
}

void EnumerationType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    for( auto element : m_kind->elements() )
    {
        callback( EnumerationConstant( m_kind, element ) );
    }
}

Constant EnumerationType::choose( void ) const
{
    const auto e = libstdhl::Random::uniform< std::size_t >(
        0, m_kind->elements().size() - 1 );

    return EnumerationConstant( m_kind, m_kind->elements()[ e ] );
}

void EnumerationType::validate( const Constant& constant ) const
{
    assert( isa< EnumerationConstant >( constant ) );

    const auto& c = static_cast< const EnumerationConstant& >( constant );
    if( *this == c.type() )
    {
        try
        {
            m_kind->encode( c.name() );
        }
        catch( const std::domain_error& e )
        {
            throw ValidationException( "value '" + c.name()
                                       + "' of constant is invalid for type "
                                       + this->description() );
        }
    }
    else
    {
        throw ValidationException( "type " + c.type().description()
                                   + " of constant is invalid for type "
                                   + this->description() );
    }
}

std::size_t EnumerationType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Range Type
//

RangeType::RangeType( const Range::Ptr& range )
: ComposedType( Type::RANGE )
, m_range( range )
{
    m_result = range->type().ptr_type();
}

RangeType::RangeType( const Type::Ptr& type )
: ComposedType( Type::RANGE )
, m_range( nullptr )
{
    m_result = type;
}

Range& RangeType::range( void ) const
{
    assert( m_range );
    return *m_range.get();
}

Range::Ptr RangeType::ptr_range( void ) const
{
    return m_range;
}

void RangeType::setRange( const Range::Ptr& range )
{
    assert( range->type() == type() );
    m_range = range;
}

Type& RangeType::type( void ) const
{
    return *m_result;
}

Type::Ptr RangeType::ptr_type( void ) const
{
    return m_result;
}

std::string RangeType::name( void ) const
{
    if( m_range )
    {
        return m_range->name();
    }
    else
    {
        return "[" + type().description() + ".." + type().description() + "]";
    }
}

std::string RangeType::description( void ) const
{
    return name();
}

void RangeType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    assert( m_range );
    if( type().isInteger() )
    {
        const auto& a
            = static_cast< IntegerConstant& >( *range().from() ).value_i64();
        const auto& b
            = static_cast< IntegerConstant& >( *range().to() ).value_i64();

        for( i64 i = a; i <= b; i++ )
        {
            callback( IntegerConstant( i ) );
        }
    }
    else
    {
        throw std::domain_error(
            "unimplemented 'foreach' of range type '" + name() + "'" );
    }
}

Constant RangeType::choose( void ) const
{
    assert( m_range );
    if( type().isInteger() )
    {
        const auto& a
            = static_cast< IntegerConstant& >( *range().from() ).value_i64();
        const auto& b
            = static_cast< IntegerConstant& >( *range().to() ).value_i64();

        return IntegerConstant( libstdhl::Random::uniform< i64 >( a, b ) );
    }
    else
    {
        throw std::domain_error(
            "unimplemented 'foreach' of range type '" + name() + "'" );
        return VoidConstant();
    }
}

void RangeType::validate( const Constant& constant ) const
{
    assert( m_range );

    if( type().isInteger() )
    {
        assert( isa< IntegerConstant >( constant ) );

        const auto& a = static_cast< IntegerConstant& >( *range().from() );
        const auto& b = static_cast< IntegerConstant& >( *range().to() );
        const auto& x = static_cast< const IntegerConstant& >( constant );

        if( ( a.value() > x.value() ) or ( x.value() > b.value() ) )
        {
            throw ValidationException( "value '" + x.name()
                                       + "' is out of type range "
                                       + this->description() );
        }
    }
    else
    {
        // TODO: unimplemented
    }
}

std::size_t RangeType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Tuple Type
//

TupleType::TupleType( const Types& types )
: ComposedType( Type::TUPLE )
{
    m_arguments = types;
}

std::string TupleType::name( void ) const
{
    std::string tmp = "t<";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        tmp += ( not first ? "," : "" );
        tmp += argument->name();
        first = false;
    }

    tmp += ">";

    return tmp;
}

std::string TupleType::description( void ) const
{
    std::string tmp = token( id() ) + "< ";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        tmp += ( not first ? ", " : "" );
        tmp += argument->description();
        first = false;
    }

    tmp += " >";

    return tmp;
}

void TupleType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // TODO
}

Constant TupleType::choose( void ) const
{
    // TODO
    return VoidConstant();
}

void TupleType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t TupleType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// List Type
//

ListType::ListType( const Type::Ptr& type )
: ComposedType( Type::LIST )
{
    m_result = type;
}

std::string ListType::name( void ) const
{
    return "l<" + m_result->name() + ">";
}

std::string ListType::description( void ) const
{
    return "List< " + m_result->description() + " >";
}

void ListType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // TODO
}

Constant ListType::choose( void ) const
{
    // TODO
    return VoidConstant();
}

void ListType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t ListType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Reference Type
//

ReferenceType::ReferenceType( const Type::ID id, const RelationType::Ptr& type )
: Type( id )
{
    m_result = type;
}

std::string ReferenceType::description( void ) const
{
    return token( id() ) + m_result->description();
}

void ReferenceType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant ReferenceType::choose( void ) const
{
    return VoidConstant();
}

//
//
// Rule Reference Type
//

RuleReferenceType::RuleReferenceType( const RelationType::Ptr& type )
: ReferenceType( Type::RULE_REFERENCE, type )
{
}

RuleReferenceType::RuleReferenceType(
    const Type::Ptr& result, const Types& arguments )
: RuleReferenceType( libstdhl::make< RelationType >( result, arguments ) )
{
}

RuleReferenceType::RuleReferenceType( void )
: RuleReferenceType(
      libstdhl::make< RelationType >( libstdhl::make< VoidType >() ) )
{
}

std::string RuleReferenceType::name( void ) const
{
    return "r" + m_result->name();
}

void RuleReferenceType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t RuleReferenceType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Function Reference Type
//

FunctionReferenceType::FunctionReferenceType( const RelationType::Ptr& type )
: ReferenceType( Type::FUNCTION_REFERENCE, type )
{
}

FunctionReferenceType::FunctionReferenceType(
    const Type::Ptr& result, const Types& arguments )
: FunctionReferenceType( libstdhl::make< RelationType >( result, arguments ) )
{
}

std::string FunctionReferenceType::name( void ) const
{
    return "f" + m_result->name();
}

void FunctionReferenceType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t FunctionReferenceType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Abstraction Type
//

AbstractionType::AbstractionType( Type::ID id )
: Type( id )
{
}

//
//
// File Type
//

FileType::FileType( void )
: AbstractionType( Type::FILE )
{
    // TODO: PPA: add file properties?
}

std::string FileType::name( void ) const
{
    return "file";
}

std::string FileType::description( void ) const
{
    return token( id() );
}

void FileType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // nothing to process
}

Constant FileType::choose( void ) const
{
    return VoidConstant();
}

void FileType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t FileType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
}

//
//
// Port Type
//

PortType::PortType( void )
: AbstractionType( Type::PORT )
{
    // TODO: PPA: add port properties?
}

std::string PortType::name( void ) const
{
    return "port";
}

std::string PortType::description( void ) const
{
    return token( id() );
}

void PortType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // nothing to process
}

Constant PortType::choose( void ) const
{
    return VoidConstant();
}

void PortType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t PortType::hash( void ) const
{
    return libstdhl::Hash::combine(
        classid(), std::hash< std::string >()( name() ) );
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

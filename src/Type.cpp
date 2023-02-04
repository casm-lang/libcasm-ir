//
//  Copyright (C) 2015-2023 CASM Organization <https://casm-lang.org>
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

#include "Type.h"

#include "Constant.h"
#include "Exception.h"

#include <libstdhl/Random>

#include <algorithm>

using namespace libcasm_ir;

static const auto TYPE_VOID = libstdhl::Memory::get< libcasm_ir::VoidType >();
static const auto TYPE_LABEL = libstdhl::Memory::get< libcasm_ir::LabelType >();
static const auto TYPE_LOCATION = libstdhl::Memory::get< libcasm_ir::LocationType >();
static const auto TYPE_BOOLEAN = libstdhl::Memory::get< libcasm_ir::BooleanType >();
static const auto TYPE_INTEGER = libstdhl::Memory::get< libcasm_ir::IntegerType >();
static const auto TYPE_RATIONAL = libstdhl::Memory::get< libcasm_ir::RationalType >();
static const auto TYPE_DECIMAL = libstdhl::Memory::get< libcasm_ir::DecimalType >();
static const auto TYPE_STRING = libstdhl::Memory::get< libcasm_ir::StringType >();

static std::array< std::vector< Type::ID >, (std::size_t)Type::Kind::_SIZE_ + 1 > s_typeIDs = {};

Type::Type( Type::Kind kind )
: m_id( kind )
{
}

Type::Kind Type::kind( void ) const
{
    return m_id.kind();
}

Type::ID Type::id( void )
{
    if( m_id.flavor() == 0 )
    {
        const auto type_hash = this->hash();
        auto result = s_registered_type_hash2ptr().emplace( type_hash, this->ptr_type() );

        if( not result.second )
        {
            // found already allocated type with set ID!
            return result.first->second->id();
        }
        else
        {
            // NOT found, registered this as new type in hash2ptr,
            // allocate new ID, set it to this type, and link it in id2hash
            const auto type_flavor = s_typeIDs[ (std::size_t)kind() ].size() + 1;
            m_id.setFlavor( type_flavor );
            s_typeIDs[ (std::size_t)kind() ].emplace_back( m_id );

            auto type_id2hash = s_registered_type_id2hash().emplace( m_id.hash(), type_hash );
            if( not type_id2hash.second )
            {
                assert( !" inconsistent state of the registered types! " );
            }
        }
    }

    return m_id;
}

const Type& Type::result( void ) const
{
    if( isRelation() or isRange() or isList() )
    {
        return *m_result.get();
    }

    return *this;
}

Type::Ptr Type::ptr_result( void ) const
{
    if( isRelation() or isRange() or isList() )
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

u1 Type::isSynthetic( void ) const
{
    return isVoid() or isLabel() or isLocation() or isRelation();
}

u1 Type::isVoid( void ) const
{
    return kind() == Type::Kind::VOID;
}

u1 Type::isLabel( void ) const
{
    return kind() == Type::Kind::LABEL;
}

u1 Type::isLocation( void ) const
{
    return kind() == Type::Kind::LOCATION;
}

u1 Type::isRelation( void ) const
{
    return kind() == Type::Kind::RELATION;
}

u1 Type::isPrimitive( void ) const
{
    return isBoolean() or isInteger() or isRational() or isBinary() or isDecimal() or isString();
}

u1 Type::isBoolean( void ) const
{
    return kind() == Type::Kind::BOOLEAN;
}

u1 Type::isInteger( void ) const
{
    return kind() == Type::Kind::INTEGER;
}

u1 Type::isRational( void ) const
{
    return kind() == Type::Kind::RATIONAL;
}

u1 Type::isBinary( void ) const
{
    return kind() == Type::Kind::BINARY;
}

u1 Type::isDecimal( void ) const
{
    return kind() == Type::Kind::DECIMAL;
}

u1 Type::isString( void ) const
{
    return kind() == Type::Kind::STRING;
}

u1 Type::isComposed( void ) const
{
    return isEnumeration() or isRange() or isTuple() or isRecord() or isList() or isObject();
}

u1 Type::isEnumeration( void ) const
{
    return kind() == Type::Kind::ENUMERATION;
}

u1 Type::isRange( void ) const
{
    return kind() == Type::Kind::RANGE;
}

u1 Type::isTuple( void ) const
{
    return kind() == Type::Kind::TUPLE;
}

u1 Type::isRecord( void ) const
{
    return kind() == Type::Kind::RECORD;
}

u1 Type::isList( void ) const
{
    return kind() == Type::Kind::LIST;
}

u1 Type::isObject( void ) const
{
    return kind() == Type::Kind::OBJECT;
}

u1 Type::isReference( void ) const
{
    return isRuleReference() or isFunctionReference();
}

u1 Type::isRuleReference( void ) const
{
    return kind() == Type::Kind::RULE_REFERENCE;
}

u1 Type::isFunctionReference( void ) const
{
    return kind() == Type::Kind::FUNCTION_REFERENCE;
}

u1 Type::isAbstraction( void ) const
{
    return isFile() or isPort();
}

u1 Type::isFile( void ) const
{
    return kind() == Type::Kind::FILE;
}

u1 Type::isPort( void ) const
{
    return kind() == Type::Kind::PORT;
}

const std::vector< Type::ID >& Type::fromKind( const Type::Kind kind )
{
    return s_typeIDs[ (std::size_t)kind ];
}

Type::Ptr Type::fromID( const Type::ID id )
{
    if( id.flavor() == 0 )
    {
        switch( id.kind() )
        {
            case libcasm_ir::Type::Kind::VOID:
            {
                return TYPE_VOID;
            }
            case libcasm_ir::Type::Kind::LABEL:
            {
                return TYPE_LABEL;
            }
            case libcasm_ir::Type::Kind::LOCATION:
            {
                return TYPE_LOCATION;
            }
            case libcasm_ir::Type::Kind::RELATION:
            {
                assert( !" invalid ID!" );
                break;
            }
            case libcasm_ir::Type::Kind::OBJECT:
            {
                assert( !" invalid ID!" );
                break;
            }
            case libcasm_ir::Type::Kind::BOOLEAN:
            {
                return TYPE_BOOLEAN;
            }
            case libcasm_ir::Type::Kind::INTEGER:
            {
                return TYPE_INTEGER;
            }
            case libcasm_ir::Type::Kind::RATIONAL:
            {
                return TYPE_RATIONAL;
            }
            case libcasm_ir::Type::Kind::BINARY:
            {
                assert( !" invalid ID!" );
                break;
            }
            case libcasm_ir::Type::Kind::DECIMAL:
            {
                return TYPE_DECIMAL;
            }
            case libcasm_ir::Type::Kind::STRING:
            {
                return TYPE_STRING;
            }
            case libcasm_ir::Type::Kind::ENUMERATION:         // [fallthrough]
            case libcasm_ir::Type::Kind::RANGE:               // [fallthrough]
            case libcasm_ir::Type::Kind::TUPLE:               // [fallthrough]
            case libcasm_ir::Type::Kind::RECORD:              // [fallthrough]
            case libcasm_ir::Type::Kind::LIST:                // [fallthrough]
            case libcasm_ir::Type::Kind::RULE_REFERENCE:      // [fallthrough]
            case libcasm_ir::Type::Kind::FUNCTION_REFERENCE:  // [fallthrough]
            case libcasm_ir::Type::Kind::FILE:                // [fallthrough]
            case libcasm_ir::Type::Kind::PORT:
            {
                assert( !" invalid ID!" );
                break;
            }
            case libcasm_ir::Type::Kind::_SIZE_:
            {
                assert( !" internal error!" );
                break;
            }
        }
    }

    auto type_id2hash = s_registered_type_id2hash().find( id.hash() );
    if( type_id2hash == s_registered_type_id2hash().end() )
    {
        throw InternalException( "type id '" + std::to_string( id ) + "' is not registered" );
    }

    auto type_cache = s_cache().find( type_id2hash->second );
    if( type_cache == s_cache().end() )
    {
        auto type_hash2ptr = s_registered_type_hash2ptr().find( type_id2hash->second );
        if( type_hash2ptr == s_registered_type_hash2ptr().end() )
        {
            assert( !" inconsistent state of the registered types! " );
        }

        return type_hash2ptr->second;
    }

    return type_cache->second;
}

std::string Type::token( const Type::Kind kind )
{
    switch( kind )
    {
        // synthetic
        case Type::Kind::VOID:
        {
            return "Void";
        }
        case Type::Kind::LABEL:
        {
            return "Label";
        }
        case Type::Kind::LOCATION:
        {
            return "Location";
        }
        case Type::Kind::RELATION:
        {
            return "Relation";
        }
        // primitive
        case Type::Kind::BOOLEAN:
        {
            return "Boolean";
        }
        case Type::Kind::INTEGER:
        {
            return "Integer";
        }
        case Type::Kind::RATIONAL:
        {
            return "Rational";
        }
        case Type::Kind::BINARY:
        {
            return "Binary";
        }
        case Type::Kind::DECIMAL:
        {
            return "Decimal";
        }
        case Type::Kind::STRING:
        {
            return "String";
        }
        // composed
        case Type::Kind::ENUMERATION:
        {
            return "Enumeration";
        }
        case Type::Kind::RANGE:
        {
            return "Range";
        }
        case Type::Kind::TUPLE:
        {
            return "Tuple";
        }
        case Type::Kind::RECORD:
        {
            return "Record";
        }
        case Type::Kind::LIST:
        {
            return "List";
        }
        case Type::Kind::OBJECT:
        {
            return "Object";
        }
        // reference
        case Type::Kind::RULE_REFERENCE:
        {
            return "RuleRef";
        }
        case Type::Kind::FUNCTION_REFERENCE:
        {
            return "FuncRef";
        }
        // abstraction
        case Type::Kind::FILE:
        {
            return "File";
        }
        case Type::Kind::PORT:
        {
            return "Port";
        }

        case Type::Kind::_SIZE_:
        {
            return "_SIZE_";
        }
    }

    assert( !" internal error " );
    return "";
}

//
//
// Synthetic Type
//

SyntheticType::SyntheticType( Type::Kind kind )
: Type( kind )
{
}

//
//
// Void Type
//

VoidType::VoidType( void )
: SyntheticType( classid() )
{
}

std::string VoidType::name( void ) const
{
    return "v";
}

std::string VoidType::description( void ) const
{
    return token( kind() );
}

void VoidType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
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
    return std::hash< std::string >()( name() );
}

//
//
// Label Type
//

LabelType::LabelType( void )
: SyntheticType( classid() )
{
}

std::string LabelType::name( void ) const
{
    return "label";
}

std::string LabelType::description( void ) const
{
    return token( kind() );
}

void LabelType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
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
    return std::hash< std::string >()( name() );
}

//
//
// Location Type
//

LocationType::LocationType( void )
: SyntheticType( classid() )
{
}

std::string LocationType::name( void ) const
{
    return "loc";
}

std::string LocationType::description( void ) const
{
    return token( kind() );
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
    return std::hash< std::string >()( name() );
}

//
//
// Relation Type
//

RelationType::RelationType( const Type::Ptr& result, const Types& arguments )
: SyntheticType( classid() )
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
    auto tmp = std::hash< std::string >()( name() );

    for( auto argument : m_arguments )
    {
        tmp = libstdhl::Hash::combine( tmp, argument->hash() );
    }

    tmp = libstdhl::Hash::combine( tmp, m_result->hash() );

    return tmp;
}

//
//
// Primitive Type
//

PrimitiveType::PrimitiveType( Type::Kind kind )
: Type( kind )
{
}

//
//
// Boolean Type
//

BooleanType::BooleanType( void )
: PrimitiveType( classid() )
{
}

std::string BooleanType::name( void ) const
{
    return "b";
}

std::string BooleanType::description( void ) const
{
    return token( kind() );
}

void BooleanType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
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
    return std::hash< std::string >()( name() );
}

//
//
// Integer Type
//

IntegerType::IntegerType( void )
: PrimitiveType( classid() )
, m_range( nullptr )
{
}

IntegerType::IntegerType( const RangeType::Ptr& range )
: PrimitiveType( classid() )
, m_range( range )
{
    assert( range );

    if( not range->type().isInteger() )
    {
        throw std::domain_error( "range '" + range->name() + "' has to be of type 'Integer'" );
    }

    const auto& a = static_cast< IntegerConstant& >( *range->range().from() ).value();
    const auto& b = static_cast< IntegerConstant& >( *range->range().to() ).value();

    if( a > b )
    {
        throw std::domain_error(
            "range '" + range->name() +
            "' violates monotonically nondecreasing property of 'Integer' type" );
    }
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
        return token( kind() );
    }
    else
    {
        return token( kind() ) + "'" + m_range->name();
    }
}

void IntegerType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
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
    auto tmp = std::hash< std::string >()( name() );

    if( constrained() )
    {
        tmp = libstdhl::Hash::combine( tmp, m_range->hash() );
    }

    return tmp;
}

//
//
// Rational Type
//

RationalType::RationalType( void )
: PrimitiveType( classid() )
{
}

std::string RationalType::name( void ) const
{
    return "q";
}

std::string RationalType::description( void ) const
{
    return token( kind() );
}

void RationalType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant RationalType::choose( void ) const
{
    const auto n = libstdhl::Type::createInteger( libstdhl::Random::uniform< i64 >() );

    const auto d = libstdhl::Type::createInteger( libstdhl::Random::uniform< i64 >() + 1 );
    // d = randomvalue + 1 to avoid that the denominator is zero!

    return RationalConstant( libstdhl::Type::createRational( n, d ) );
}

void RationalType::validate( const Constant& constant ) const
{
    assert( isa< RationalConstant >( constant ) );
}

std::size_t RationalType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
//
// Binary Type
//

BinaryType::BinaryType( u16 bitsize )
: PrimitiveType( classid() )
, m_bitsize( bitsize )
{
    if( m_bitsize < 1 or m_bitsize > BinaryType::SizeMax )
    {
        throw std::domain_error(
            "invalid bit size '" + std::to_string( m_bitsize ) + "' for 'Binary' type" );
    }
}

BinaryType::BinaryType( const IntegerConstant::Ptr& bitsize )
: PrimitiveType( classid() )
{
    assert( bitsize );
    if( bitsize->value().value() > BinaryType::SizeMax or bitsize->value().sign() )
    {
        throw std::domain_error( "invalid bit size '" + bitsize->name() + "' for 'Binary' type" );
    }

    m_bitsize = bitsize->value_i64();

    if( m_bitsize < 1 )
    {
        throw std::domain_error(
            "invalid bit size '" + std::to_string( m_bitsize ) + "' for 'Binary' type" );
    }
}

BinaryType::BinaryType( const std::string& value, const libstdhl::Type::Radix radix )
: PrimitiveType( classid() )
{
    std::string tmp = value;
    tmp.erase( std::remove( tmp.begin(), tmp.end(), '\'' ), tmp.end() );

    m_bitsize = (u16)tmp.size() * std::log2( (double)radix );

    if( m_bitsize < 1 or m_bitsize > BinaryType::SizeMax )
    {
        throw std::domain_error(
            "invalid bit size '" + std::to_string( m_bitsize ) + "' for 'Binary' type" );
    }
}

u16 BinaryType::bitsize( void ) const
{
    return m_bitsize;
}

std::string BinaryType::name( void ) const
{
    return "u" + std::to_string( m_bitsize );
}

std::string BinaryType::description( void ) const
{
    return token( kind() ) + "'" + std::to_string( m_bitsize );
}

void BinaryType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an (depending on the current bit-size) infinite range to
    // process, therefore omitted (for now)
}

Constant BinaryType::choose( void ) const
{
    return BinaryConstant(
        m_bitsize,
        libstdhl::Random::uniform< u64 >() %
            m_bitsize );  // TODO: FIXME: PPA: fix the randomized value modulo
                          // mapping to full range not only the bitsize
}

void BinaryType::validate( const Constant& constant ) const
{
    assert( isa< BinaryConstant >( constant ) or isa< IntegerConstant >( constant ) );

    std::size_t constantBitSize;
    if( isa< BinaryConstant >( constant ) )
    {
        const auto& c = static_cast< const BinaryConstant& >( constant );
        assert( c.type().isBinary() );
        const auto& t = static_cast< const BinaryType& >( c.type() );
        constantBitSize = t.bitsize();
    }
    else
    {
        const auto& c = static_cast< const IntegerConstant& >( constant );
        assert( c.type().isInteger() );

        assert( c.value().trivial() and " TODO: PPA: FIXME:" );
        constantBitSize = static_cast< u64 >(
            std::log2( (double)( c.value() > 1 ? c.value().value() - 1 : 1 ) ) );
    }

    if( m_bitsize < constantBitSize )
    {
        throw ValidationException(
            " type " + constant.type().description() + " of constant is invalid for type " +
            this->description() );
    }
}

std::size_t BinaryType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
// Flaoting Type
//

DecimalType::DecimalType( void )
: PrimitiveType( classid() )
{
}

std::string DecimalType::name( void ) const
{
    return "d";
}

std::string DecimalType::description( void ) const
{
    return token( kind() );
}

void DecimalType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant DecimalType::choose( void ) const
{
    // this is undefined for now
    return DecimalConstant();
}

void DecimalType::validate( const Constant& constant ) const
{
    assert( isa< DecimalConstant >( constant ) );
}

std::size_t DecimalType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
//
// String Type
//

StringType::StringType( void )
: PrimitiveType( classid() )
{
}

std::string StringType::name( void ) const
{
    return "s";
}

std::string StringType::description( void ) const
{
    return token( kind() );
}

void StringType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
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
    return std::hash< std::string >()( name() );
}

//
//
// Composed Type
//

ComposedType::ComposedType( Type::Kind kind )
: Type( kind )
{
}

//
// Enumeration Type
//

EnumerationType::EnumerationType( const Enumeration::Ptr& kind )
: ComposedType( classid() )
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
    const auto e = libstdhl::Random::uniform< std::size_t >( 0, m_kind->elements().size() - 1 );

    return EnumerationConstant( m_kind, m_kind->elements()[ e ] );
}

void EnumerationType::validate( const Constant& constant ) const
{
    assert( isa< EnumerationConstant >( constant ) or isa< DomainConstant >( constant ) );

    if( isa< EnumerationConstant >( constant ) )
    {
        const auto& c = static_cast< const EnumerationConstant& >( constant );
        if( *this != c.type() )
        {
            throw ValidationException(
                "type " + c.type().description() + " of constant is invalid for type " +
                this->description() );
        }

        try
        {
            m_kind->encode( c.name() );
        }
        catch( const std::domain_error& e )
        {
            throw ValidationException(
                "value '" + c.name() + "' of constant is invalid for type " + this->description() );
        }
    }
    else
    {
        const auto& c = static_cast< const DomainConstant& >( constant );
        if( *this != c.type() )
        {
            throw ValidationException(
                "type " + c.type().description() + " of constant is invalid for type " +
                this->description() );
        }
    }
}

std::size_t EnumerationType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
//
// Range Type
//

RangeType::RangeType( const Range::Ptr& range )
: ComposedType( classid() )
, m_range( range )
{
    m_result = range->type().ptr_type();
}

RangeType::RangeType( const Type::Ptr& type )
: ComposedType( classid() )
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

void RangeType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
{
    if( type().isInteger() )
    {
        const auto a = m_range ? static_cast< IntegerConstant& >( *range().from() ).value()
                               : libstdhl::Limits< libstdhl::Type::Integer >::min();

        const auto b = m_range ? static_cast< IntegerConstant& >( *range().to() ).value()
                               : libstdhl::Limits< libstdhl::Type::Integer >::max();

        for( auto i = a; i <= b; ++i )
        {
            callback( IntegerConstant( i ) );
        }
    }
    else if( type().isBoolean() )
    {
        if( m_range )
        {
            const auto& a = static_cast< BooleanConstant& >( *range().from() ).value();
            const auto& b = static_cast< BooleanConstant& >( *range().to() ).value();

            callback( BooleanConstant( a ) );

            if( a != b )
            {
                callback( BooleanConstant( b ) );
            }
        }
        else
        {
            callback( BooleanConstant( false ) );
            callback( BooleanConstant( true ) );
        }
    }
    else
    {
        throw std::domain_error( "unimplemented 'foreach' of range type '" + name() + "'" );
    }
}

Constant RangeType::choose( void ) const
{
    if( type().isInteger() )
    {
        if( m_range )
        {
            const auto& a = static_cast< IntegerConstant& >( *range().from() ).value();
            const auto& b = static_cast< IntegerConstant& >( *range().to() ).value();

            return IntegerConstant( libstdhl::Random::uniform<>( a, b ) );
        }
        else
        {
            return IntegerConstant( libstdhl::Random::uniform< libstdhl::Type::Integer >() );
        }
    }
    else if( type().isDecimal() )
    {
        if( m_range )
        {
            const auto& a = static_cast< DecimalConstant& >( *range().from() ).value();
            const auto& b = static_cast< DecimalConstant& >( *range().to() ).value();

            return DecimalConstant( libstdhl::Random::uniform<>( a, b ) );
        }
        else
        {
            return DecimalConstant( libstdhl::Random::uniform< libstdhl::Type::Decimal >() );
        }
    }
    else if( type().isBoolean() )
    {
        if( m_range )
        {
            const auto a = static_cast< BooleanConstant& >( *range().from() ).value().value();
            const auto b = static_cast< BooleanConstant& >( *range().to() ).value().value();

            return BooleanConstant( libstdhl::Random::uniform< u8 >( a, b ) );
        }
        else
        {
            return BooleanConstant( libstdhl::Random::uniform< u8 >( false, true ) );
        }
    }

    throw std::domain_error( "unimplemented 'choose' of range type '" + name() + "'" );
    return VoidConstant();
}

void RangeType::validate( const Constant& constant ) const
{
    if( type().isInteger() )
    {
        if( isa< IntegerConstant >( constant ) )
        {
            if( not m_range )
            {
                throw ValidationException(
                    "value '" + constant.name() + "' cannot be validated against type range " +
                    this->description() );
            }

            const auto& a = static_cast< IntegerConstant& >( *range().from() );
            const auto& b = static_cast< IntegerConstant& >( *range().to() );

            const auto& x = static_cast< const IntegerConstant& >( constant );

            if( a.value() > x.value() or x.value() > b.value() )
            {
                throw ValidationException(
                    "value '" + constant.name() + "' is out of type range " + this->description() );
            }
        }
        else
        {
            assert( isa< RangeConstant >( constant ) );

            if( not constant.type().result().isInteger() )
            {
                throw ValidationException(
                    "range '" + constant.name() + "' does not match type range " +
                    this->description() );
            }

            if( not m_range )
            {
                return;
            }

            const auto& a = static_cast< IntegerConstant& >( *range().from() );
            const auto& b = static_cast< IntegerConstant& >( *range().to() );
            const auto& x = static_cast< const RangeConstant& >( constant );
            const auto& t = static_cast< const RangeType& >( x.type() );
            const auto& xa = static_cast< IntegerConstant& >( *t.range().from() );
            const auto& xb = static_cast< IntegerConstant& >( *t.range().to() );

            if( a.value() > xa.value() or xb.value() > b.value() )
            {
                throw ValidationException(
                    "range '" + constant.name() + "' is not included in type range " +
                    this->description() );
            }
        }
    }
    else
    {
        // TODO: unimplemented
    }
}

std::size_t RangeType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
//
// Tuple Type
//

TupleType::TupleType( const Types& types )
: ComposedType( classid() )
{
    m_arguments = types;
}

std::string TupleType::name( void ) const
{
    std::string tmp = "t<";

    u1 first = true;
    for( const auto& argument : m_arguments )
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
    std::string tmp = "( ";

    u1 first = true;
    for( const auto& argument : m_arguments )
    {
        tmp += ( not first ? ", " : "" );
        tmp += argument->description();
        first = false;
    }
    tmp += " )";

    return tmp;
}

void TupleType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
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
    return std::hash< std::string >()( name() );
}

//
//
// Record Type
//

RecordType::RecordType( const Types& types, const std::vector< std::string >& identifiers )
: ComposedType( classid() )
, m_identifiers( identifiers )
, m_elements()
{
    m_arguments = types;

    const auto recordSize = identifiers.size();
    assert( recordSize >= 1 and recordSize == types.size() );

    for( std::size_t index = 0; index < recordSize; index++ )
    {
        m_elements.emplace( identifiers[ index ], index );
    }
}

const std::vector< std::string >& RecordType::identifiers( void ) const
{
    return m_identifiers;
}

const std::map< std::string, std::size_t >& RecordType::elements( void ) const
{
    return m_elements;
}

u1 RecordType::contains( const RecordType& other ) const
{
    if( identifiers().size() < other.identifiers().size() )
    {
        // this record type has less identifiers than the other, cannot contain other record type
        return false;
    }

    for( const auto& otherElement : other.elements() )
    {
        const auto it = m_elements.find( otherElement.first );
        if( it == m_elements.cend() )
        {
            // element identifier from 'other' is not in this record
            return false;
        }

        if( m_arguments.at( it->second ) != other.arguments().at( otherElement.second ) )
        {
            // element type of 'other' differs from this record element
            return false;
        }
    }

    return true;
}

std::string RecordType::name( void ) const
{
    std::string tmp = "r<";

    u1 first = true;
    for( const auto it : m_elements )
    {
        tmp += ( not first ? "," : "" );
        tmp += it.first;
        tmp += ":";
        tmp += m_arguments[ it.second ]->name();
        first = false;
    }
    tmp += ">";

    return tmp;
}

std::string RecordType::description( void ) const
{
    std::string tmp = "( ";

    u1 first = true;
    for( const auto it : m_elements )
    {
        tmp += ( not first ? ", " : "" );
        tmp += it.first;
        tmp += " : ";
        tmp += m_arguments[ it.second ]->description();
        first = false;
    }
    tmp += " )";

    return tmp;
}

void RecordType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
{
    // TODO
}

Constant RecordType::choose( void ) const
{
    // TODO
    return VoidConstant();
}

void RecordType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t RecordType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
//
// List Type
//

ListType::ListType( const Type::Ptr& type )
: ComposedType( classid() )
, m_list( nullptr )
{
    m_result = type;
}

List& ListType::list( void ) const
{
    assert( m_list );
    return *m_list.get();
}

List::Ptr ListType::ptr_list( void ) const
{
    return m_list;
}

void ListType::setList( const List::Ptr& list )
{
    assert( list->type().result() == type() );
    m_list = list;
}

Type& ListType::type( void ) const
{
    return *m_result;
}

Type::Ptr ListType::ptr_type( void ) const
{
    return m_result;
}

std::string ListType::name( void ) const
{
    return "l<" + m_result->name() + ">";
}

std::string ListType::description( void ) const
{
    return "List< " + m_result->description() + " >";
}

void ListType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
{
}

Constant ListType::choose( void ) const
{
    return VoidConstant();
}

void ListType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t ListType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
//
// Object Type
//

ObjectType::ObjectType( const std::string& name )
: SyntheticType( classid() )
, m_name( name )
{
}

std::string ObjectType::name( void ) const
{
    return m_name;
}

std::string ObjectType::description( void ) const
{
    return m_name;
}

void ObjectType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has no range to process
}

Constant ObjectType::choose( void ) const
{
    return DomainConstant( ptr_this< Type >() );
}

void ObjectType::validate( const Constant& constant ) const
{
    // omit, every constant can be an object
}

std::size_t ObjectType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
//
// Reference Type
//

ReferenceType::ReferenceType( const Type::Kind kind, const RelationType::Ptr& type )
: Type( kind )
{
    m_result = type;
}

RelationType::Ptr ReferenceType::dereference( void ) const
{
    return std::static_pointer_cast< RelationType >( m_result );
}

std::string ReferenceType::description( void ) const
{
    return token( kind() ) + m_result->description();
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
: ReferenceType( classid(), type )
{
}

RuleReferenceType::RuleReferenceType( const Type::Ptr& result, const Types& arguments )
: RuleReferenceType( libstdhl::Memory::make< RelationType >( result, arguments ) )
{
}

RuleReferenceType::RuleReferenceType( void )
: RuleReferenceType(
      libstdhl::Memory::make< RelationType >( libstdhl::Memory::make< VoidType >() ) )
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
    return std::hash< std::string >()( name() );
}

//
//
// Function Reference Type
//

FunctionReferenceType::FunctionReferenceType( const RelationType::Ptr& type )
: ReferenceType( classid(), type )
{
}

FunctionReferenceType::FunctionReferenceType( const Type::Ptr& result, const Types& arguments )
: FunctionReferenceType( libstdhl::Memory::make< RelationType >( result, arguments ) )
{
}

std::string FunctionReferenceType::name( void ) const
{
    return "f" + m_result->name();  // PPA: FIXME: change this to "z", when
                                    // Decimal is introduced
}

void FunctionReferenceType::validate( const Constant& constant ) const
{
    // TODO
}

std::size_t FunctionReferenceType::hash( void ) const
{
    return std::hash< std::string >()( name() );
}

//
//
// Abstraction Type
//

AbstractionType::AbstractionType( Type::Kind kind )
: Type( kind )
{
}

//
//
// File Type
//

FileType::FileType( const Type::Ptr& type )
: AbstractionType( classid() )
{
    m_result = type;
    // TODO: PPA: add file properties?
}

std::string FileType::name( void ) const
{
    return "file<" + m_result->name() +
           ">";  // PPA: FIXME: change "file" to "f", when Decimal is introduced
}

std::string FileType::description( void ) const
{
    return token( kind() ) + "< " + m_result->description() + " >";
}

void FileType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
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
    return std::hash< std::string >()( name() );
}

//
//
// Port Type
//

PortType::PortType( const Type::Ptr& type )
: AbstractionType( classid() )
{
    m_result = type;
    // TODO: PPA: add port properties?
}

std::string PortType::name( void ) const
{
    return "port<" + m_result->name() +
           ">";  // PPA: FIXME: change "port" to "p", when Decimal is introduced
}

std::string PortType::description( void ) const
{
    return token( kind() ) + "< " + m_result->description() + " >";
}

void PortType::foreach( const std::function< void( const Constant& constant ) >& callback ) const
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
    return std::hash< std::string >()( name() );
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

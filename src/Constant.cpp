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

#include "../stdhl/cpp/Log.h"
#include "Rule.h"

using namespace libcasm_ir;

bool Constant::classof( Value const* obj )
{
    return obj->getValueID() == classid() or AgentConstant::classof( obj )
           or RuleReferenceConstant::classof( obj )
           or BooleanConstant::classof( obj ) or IntegerConstant::classof( obj )
           or BitConstant::classof( obj ) or StringConstant::classof( obj )
           // TODO: PPA: add here other constant types!
           or Identifier::classof( obj );
}

Value* Constant::getUndef( Type* result )
{
    assert( result );

    std::string tmp = "";
    tmp += result->getName();
    tmp += " undef";

    auto cache = str2obj().find( tmp );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    switch( result->getID() )
    {
        case Type::AGENT:
        {
            static AgentConstant cache = AgentConstant();
            return str2obj()
                .emplace( cache.getDescription(), &cache )
                .first->second;
        }
        case Type::RULE_REFERENCE:
        {
            static RuleReferenceConstant cache = RuleReferenceConstant();
            return str2obj()
                .emplace( cache.getDescription(), &cache )
                .first->second;
        }
        case Type::BOOLEAN:
        {
            static BooleanConstant cache = BooleanConstant();
            return str2obj()
                .emplace( cache.getDescription(), &cache )
                .first->second;
        }
        case Type::INTEGER:
        {
            static IntegerConstant cache = IntegerConstant();
            return str2obj()
                .emplace( cache.getDescription(), &cache )
                .first->second;
        }
        case Type::BIT:
        {
            BitConstant c = BitConstant( result );

            auto cache = str2obj().find( c.getDescription() );
            if( cache != str2obj().end() )
            {
                return cache->second;
            }

            Value* ptr = new BitConstant( c );
            return str2obj().emplace( c.getDescription(), ptr ).first->second;
        }
        case Type::STRING:
        {
            static StringConstant cache = StringConstant();
            return str2obj()
                .emplace( cache.getDescription(), &cache )
                .first->second;
        }
        // case Type::FLOATING:
        // {
        //     static FloatingConstant cache = FloatingConstant();
        //     return str2obj().emplace( tmp, &cache ).first->second;
        // }
        // case Type::RATIONAL:
        // {
        //     static RationalConstant cache = RationalConstant();
        //     return str2obj().emplace( tmp, &cache ).first->second;
        // }
        // case Type::ENUMERATION:
        // {
        //     static EnumerationConstant cache = EnumerationConstant();
        //     return str2obj().emplace( tmp, &cache ).first->second;
        // }
        default:
        {
            libstdhl::Log::error( "unable to create a constant for type '%s'",
                result->getDescription() );
            return 0;
        }
    }
}

Value* Constant::getAgent( Type::Agent value )
{
    AgentConstant tmp = AgentConstant( value );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new AgentConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

Value* Constant::getRuleReference( const char* value )
{
    RuleReferenceConstant tmp = RuleReferenceConstant( value );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new RuleReferenceConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

Value* Constant::getBoolean( Type::Boolean value )
{
    BooleanConstant tmp = BooleanConstant( value );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new BooleanConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

Value* Constant::getInteger( Type::Integer value )
{
    IntegerConstant tmp = IntegerConstant( value );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new IntegerConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

Value* Constant::getBit( Type* result, u64 value )
{
    BitConstant tmp = BitConstant( result, value );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new BitConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

Value* Constant::getString( const char* value )
{
    StringConstant tmp = StringConstant( value );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new StringConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

//
// Constants
//

AgentConstant::AgentConstant( Type::Agent value, u1 defined )
: ConstantOf< Type::Agent >( ( defined ? "self" : "undef" ), Type::getAgent(),
      value, defined, Value::AGENT_CONSTANT )
{
}

AgentConstant::AgentConstant( Type::Agent value )
: AgentConstant( value, true )
{
}

AgentConstant::AgentConstant( void )
: AgentConstant( 0, false )
{
}

void AgentConstant::dump( void ) const
{
    printf( "[Const] %p = agent %p\n", this, getValue() );
}

bool AgentConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

RuleReferenceConstant::RuleReferenceConstant(
    Type::RuleReference value, const char* name, u1 defined )
: ConstantOf< Type::RuleReference >(
      ( defined ? libstdhl::Allocator::string( value ? value->getName() : name )
                : "undef" ),
      Type::getRuleReference(), value, defined, Value::RULE_REFERENCE_CONSTANT )
, resolve_identifier( name )
{
}

RuleReferenceConstant::RuleReferenceConstant( Type::RuleReference value )
: RuleReferenceConstant( value, 0, true )
{
}

RuleReferenceConstant::RuleReferenceConstant( const char* name )
: RuleReferenceConstant( 0, name, true )
{
}

RuleReferenceConstant::RuleReferenceConstant( void )
: RuleReferenceConstant( 0, 0, false )
{
}

// // use this ruleconstpointer for not finished resolved rule*'s
// RuleReferenceConstant* RuleReferenceConstant::create( const char* name )
// {
//     static std::unordered_map< const char*, RuleReferenceConstant* > cache;

//     auto rsym = getSymbols().find( name );
//     if( rsym != getSymbols().end() )
//     {
//         Rule* rv = 0;
//         for( auto r : rsym->second )
//         {
//             assert( not rv );
//             if( Value::isa< Rule >( r ) )
//             {
//                 rv = static_cast< Rule* >( rv );
//             }
//         }

//         return create( rv );
//     }

//     auto result = cache.find( name );
//     if( result != cache.end() )
//     {
//         assert( result->second );
//         // printf( "[Const] found %p\n", result->second );
//         return result->second;
//     }

//     RuleReferenceConstant* obj = new RuleReferenceConstant( 0, true );
//     assert( obj );
//     obj->setResolveIdentifier( name );
//     cache[ name ] = obj;
//     return obj;
// }

void RuleReferenceConstant::resolve( void )
{
    if( !resolve_identifier )
    {
        return;
    }

    auto result = getSymbols().find( resolve_identifier );
    if( result != getSymbols().end() )
    {
        assert( result->second.size() == 1 );
        Value* val = *( result->second.begin() );
        assert( Value::isa< Rule >( val ) );
        setValue( static_cast< Rule* >( val ) );
    }
}

void RuleReferenceConstant::checking( void )
{
    for( auto value : getSymbols()[ ".rulepointer" ] )
    {
        assert( Value::isa< RuleReferenceConstant >( value ) );
        ( (libcasm_ir::RuleReferenceConstant*)value )->resolve();
    }
}

void RuleReferenceConstant::dump( void ) const
{
    printf( "[Const] %p = rule %p\n", this, getValue() );
}

bool RuleReferenceConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

BooleanConstant::BooleanConstant( Type::Boolean value, u1 defined )
: ConstantOf< Type::Boolean >(
      ( defined ? ( value ? "true" : "false" ) : "undef" ), Type::getBoolean(),
      value, defined, Value::BOOLEAN_CONSTANT )
{
}

BooleanConstant::BooleanConstant( Type::Boolean value )
: BooleanConstant( value, true )
{
}

BooleanConstant::BooleanConstant( void )
: BooleanConstant( 0, false )
{
}

void BooleanConstant::dump( void ) const
{
    printf( "[Const] %p = bool %u\n", this, getValue() );
}

bool BooleanConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

IntegerConstant::IntegerConstant( Type::Integer value, u1 defined )
: ConstantOf< Type::Integer >(
      ( defined ? libstdhl::Allocator::string( std::to_string( value ) )
                : "undef" ),
      Type::getInteger(), value, defined, Value::INTEGER_CONSTANT )
{
}

IntegerConstant::IntegerConstant( Type::Integer value )
: IntegerConstant( value, true )
{
}

IntegerConstant::IntegerConstant( void )
: IntegerConstant( 0, false )
{
}

void IntegerConstant::dump( void ) const
{
    printf( "[Const] %p = integer %li\n", this, getValue() );
}

bool IntegerConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

BitConstant::BitConstant( Type* result, u64 value, u1 defined )
: ConstantOf< Type::Bit >(
      ( defined ? libstdhl::Allocator::string( std::to_string( value ) )
                : "undef" ),
      result, this->value, defined, Value::BIT_CONSTANT )
{
    this->value[ 0 ] = value;
}

BitConstant::BitConstant( Type* result, u64 value )
: BitConstant( result, value, true )
{
}

BitConstant::BitConstant( Type* result )
: BitConstant( result, 0, false )
{
}

void BitConstant::dump( void ) const
{
    printf( "[Const] %p = bit %li\n", this, getValue()[ 0 ] );
}

bool BitConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

StringConstant::StringConstant( Type::String value, u1 defined )
: ConstantOf< Type::String >(
      ( defined ? libstdhl::Allocator::string( value ) : "undef" ),
      Type::getString(), value, defined, Value::STRING_CONSTANT )
{
}

StringConstant::StringConstant( Type::String value )
: StringConstant( value, true )
{
}

StringConstant::StringConstant( const char* value )
: StringConstant( ( Type::String )( value ), true )
{
}

StringConstant::StringConstant( void )
: StringConstant( 0, false )
{
}

void StringConstant::dump( void ) const
{
    printf( "[Const] %p = string %s\n", this, getValue() );
}

bool StringConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

Identifier::Identifier( Type* type, const char* value )
: ConstantOf< const char* >( value, type, value, true, Value::IDENTIFIER )
{
    getSymbols()[ ".identifier" ].insert( this );
}

Identifier::~Identifier( void )
{
    getSymbols()[ ".identifier" ].erase( this );
}

Identifier* Identifier::create( Type* type, const char* value, Value* scope )
{
    const char* tmp = value;

    // if( scope )
    // {
    //     tmp = libstdhl::Allocator::string( string(scope->getName()) + "::" +
    //     string(tmp) );
    //     // std::string tmp;
    //     // tmp.append( scope->getName() );
    //     // tmp.append( "::" );
    //     // tmp.append( value );
    //     // tmp_scope = tmp.c_str();
    // }

    auto result = getSymbols().find( tmp );
    if( result != getSymbols().end() )
    {
        assert( result->second.size() == 1 );
        Value* x = *result->second.begin();

        assert( x->getType()->getID() == type->getID() );
        // //if( x->getType()->getID() == type->getID() )
        // //{
        // printf( "[Ident] found '%s' of type %lu @ %p\n", value,
        // type->getID(), x );
        return (Identifier*)x;
        // //}
    }

    // printf( "[Ident] creating '%s' of type %lu\n", value, type->getID() );
    return new Identifier( type, value );
}

// Identifier* Identifier::create( Type* type )
// {
//     static std::unordered_map< u64, Identifier* > cache;
//     auto result = cache.find( type->getID() );
//     Identifier* x = 0;

//     if( result != cache.end() )
//     {
//         x = result->second;
//          assert( x->getType()->getID() == type->getID() );
//         printf( "[Ident] found 'undef' of type %lu @ %p\n", type->getID(), x
//         );
//         return x;
//     }

//     x = new Identifier( type, 0, false );

//     printf( "[Ident] creating '%s' of type %lu\n", value, type->getID() );
//     return
// }

void Identifier::forgetSymbol( const char* value )
{
    // printf( "[Ident] forgetting '%s'\n", value );
    getSymbols().erase( value );
}

void Identifier::dump( void ) const
{
    printf( "[Ident] " );
    debug();
}

bool Identifier::classof( Value const* obj )
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

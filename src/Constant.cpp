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

#include "Rule.h"

#include "../stdhl/cpp/Log.h"

using namespace libcasm_ir;

static const char* undef_str = "undef";

u1 Constant::classof( Value const* obj )
{
    return obj->id() == classid() or VoidConstant::classof( obj )
           or AgentConstant::classof( obj )
           or RuleReferenceConstant::classof( obj )
           or BooleanConstant::classof( obj ) or IntegerConstant::classof( obj )
           or BitConstant::classof( obj ) or StringConstant::classof( obj )
           // TODO: PPA: add here other constant types!
           or Identifier::classof( obj );
}

Value* Constant::Undef( Type* result )
{
    assert( result );

    std::string tmp = "";
    tmp += result->name();
    tmp += " undef";

    auto cache = m_str2obj().find( tmp );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    switch( result->id() )
    {
        case Type::AGENT:
        {
            static AgentConstant cache = AgentConstant();
            return m_str2obj()
                .emplace( cache.description(), &cache )
                .first->second;
        }
        case Type::RULE_REFERENCE:
        {
            static RuleReferenceConstant cache = RuleReferenceConstant();
            return m_str2obj()
                .emplace( cache.description(), &cache )
                .first->second;
        }
        case Type::BOOLEAN:
        {
            static BooleanConstant cache = BooleanConstant();
            return m_str2obj()
                .emplace( cache.description(), &cache )
                .first->second;
        }
        case Type::INTEGER:
        {
            static IntegerConstant cache = IntegerConstant();
            return m_str2obj()
                .emplace( cache.description(), &cache )
                .first->second;
        }
        case Type::BIT:
        {
            BitConstant c = BitConstant( result );

            auto cache = m_str2obj().find( c.description() );
            if( cache != m_str2obj().end() )
            {
                return cache->second;
            }

            Value* ptr = new BitConstant( c );
            return m_str2obj().emplace( c.description(), ptr ).first->second;
        }
        case Type::STRING:
        {
            static StringConstant cache = StringConstant();
            return m_str2obj()
                .emplace( cache.description(), &cache )
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
                result->description() );
            return 0;
        }
    }
}

Value* Constant::Agent( Type::AgentTy value )
{
    AgentConstant tmp = AgentConstant( value );

    auto cache = m_str2obj().find( tmp.description() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new AgentConstant( tmp );
    return m_str2obj().emplace( tmp.description(), ptr ).first->second;
}

Value* Constant::RuleReference( Type::RuleReferenceTy value )
{
    RuleReferenceConstant tmp = RuleReferenceConstant( value );

    auto cache = m_str2obj().find( tmp.description() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new RuleReferenceConstant( tmp );
    return m_str2obj().emplace( tmp.description(), ptr ).first->second;
}

Value* Constant::RuleReference( const char* value )
{
    RuleReferenceConstant tmp = RuleReferenceConstant( value );

    auto cache = m_str2obj().find( tmp.description() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new RuleReferenceConstant( tmp );
    return m_str2obj().emplace( tmp.description(), ptr ).first->second;
}

Value* Constant::Boolean( Type::BooleanTy value )
{
    BooleanConstant tmp = BooleanConstant( value );

    auto cache = m_str2obj().find( tmp.description() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new BooleanConstant( tmp );
    return m_str2obj().emplace( tmp.description(), ptr ).first->second;
}

Value* Constant::Integer( Type::IntegerTy value )
{
    IntegerConstant tmp = IntegerConstant( value );

    auto cache = m_str2obj().find( tmp.description() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new IntegerConstant( tmp );
    return m_str2obj().emplace( tmp.description(), ptr ).first->second;
}

Value* Constant::Bit( Type* result, u64 value )
{
    BitConstant tmp = BitConstant( result, value );

    auto cache = m_str2obj().find( tmp.description() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new BitConstant( tmp );
    return m_str2obj().emplace( tmp.description(), ptr ).first->second;
}

Value* Constant::String( const char* value )
{
    StringConstant tmp = StringConstant( value );

    auto cache = m_str2obj().find( tmp.description() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new StringConstant( tmp );
    return m_str2obj().emplace( tmp.description(), ptr ).first->second;
}

//
// Constants
//

VoidConstant::VoidConstant( void )
: ConstantOf< void* >( "void", Type::Void(), 0, true, classid() )
{
}

u1 VoidConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AgentConstant
//

AgentConstant::AgentConstant( Type::AgentTy value, u1 defined )
: ConstantOf< Type::AgentTy >( ( defined ? "self" : undef_str ), Type::Agent(),
      value, defined, classid() )
{
}

AgentConstant::AgentConstant( Type::AgentTy value )
: AgentConstant( value, true )
{
}

AgentConstant::AgentConstant( void )
: AgentConstant( 0, false )
{
}

u1 AgentConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

RuleReferenceConstant::RuleReferenceConstant(
    Type::RuleReferenceTy value, const char* name, u1 defined )
: ConstantOf< Type::RuleReferenceTy >(
      ( defined ? libstdhl::Allocator::string( value ? value->name() : name )
                : undef_str ),
      Type::RuleReference(), value, defined, classid() )
, m_resolve_identifier( name )
{
}

RuleReferenceConstant::RuleReferenceConstant( Type::RuleReferenceTy value )
: RuleReferenceConstant( value, 0, true )
{
}

RuleReferenceConstant::RuleReferenceConstant( const char* name )
: RuleReferenceConstant(
      0, libstdhl::Allocator::string( "@" + std::string( name ) ), true )
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
//             if( isa< Rule >( r ) )
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
    if( not m_resolve_identifier )
    {
        return;
    }

    for( auto value : m_id2objs()[ Rule::classid() ] )
    {
        assert( isa< Rule >( value ) );

        if( strcmp( m_resolve_identifier, value->name() ) )
        {
            setValue( static_cast< Rule* >( value ) );
            m_resolve_identifier = 0;
            break;
        }
    }
}

void RuleReferenceConstant::checking( void )
{
    for( auto value : m_id2objs()[ RuleReferenceConstant::classid() ] )
    {
        assert( isa< RuleReferenceConstant >( value ) );
        RuleReferenceConstant* rrc
            = static_cast< RuleReferenceConstant* >( value );
        rrc->resolve();
    }
}

u1 RuleReferenceConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

BooleanConstant::BooleanConstant( Type::BooleanTy value, u1 defined )
: ConstantOf< Type::BooleanTy >(
      ( defined ? ( value ? "true" : "false" ) : undef_str ), Type::Boolean(),
      value, defined, classid() )
{
}

BooleanConstant::BooleanConstant( Type::BooleanTy value )
: BooleanConstant( value, true )
{
}

BooleanConstant::BooleanConstant( void )
: BooleanConstant( 0, false )
{
}

u1 BooleanConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

IntegerConstant::IntegerConstant( Type::IntegerTy value, u1 defined )
: ConstantOf< Type::IntegerTy >(
      ( defined ? libstdhl::Allocator::string( std::to_string( value ) )
                : undef_str ),
      Type::Integer(), value, defined, classid() )
{
}

IntegerConstant::IntegerConstant( Type::IntegerTy value )
: IntegerConstant( value, true )
{
}

IntegerConstant::IntegerConstant( void )
: IntegerConstant( 0, false )
{
}

u1 IntegerConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

BitConstant::BitConstant( Type* result, u64 value, u1 defined )
: ConstantOf< Type::BitTy >(
      ( defined ? libstdhl::Allocator::string( std::to_string( value ) )
                : undef_str ),
      result, value, defined, classid() )
{
}

BitConstant::BitConstant( Type* result, u64 value )
: BitConstant( result, value, true )
{
}

BitConstant::BitConstant( Type* result )
: BitConstant( result, 0, false )
{
}

BitConstant::BitConstant( u16 bitsize, u64 value )
: BitConstant( Type::Bit( bitsize ), value, true )
{
}

BitConstant::BitConstant( u16 bitsize )
: BitConstant( Type::Bit( bitsize ), 0, false )
{
}

u1 BitConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

StringConstant::StringConstant( Type::StringTy value, u1 defined )
: ConstantOf< Type::StringTy >(
      ( defined ? libstdhl::Allocator::string( value ) : undef_str ),
      Type::String(), value, defined, classid() )
{
}

StringConstant::StringConstant( Type::StringTy value )
: StringConstant( value, true )
{
}

StringConstant::StringConstant( const char* value )
: StringConstant( ( Type::StringTy )( value ), true )
{
}

StringConstant::StringConstant( void )
: StringConstant( 0, false )
{
}

u1 StringConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

Identifier::Identifier( Type* type, const char* value )
: ConstantOf< const char* >( value, type, value, true, classid() )
{
    // auto result = ident2obj().find( value );
    // if( result != ident2obj().end() )
    // {
    //     assert( !" identifier already used! " );
    // }
}

Identifier::~Identifier( void )
{
}

Identifier* Identifier::create( Type* type, const char* value, Value* scope )
{
    assert( type );
    assert( value );

    Identifier tmp = Identifier( type, value );

    auto cache = ident2obj().find( value );
    if( cache != ident2obj().end() )
    {
        Value* v = cache->second;
        assert( strcmp( v->type().name(), type->name() ) == 0 );
        return cache->second;
    }

    Identifier* ptr = new Identifier( tmp );
    ident2obj()[ std::string( value ) ] = ptr;
    return ptr;
}

void Identifier::forgetSymbol( const char* value )
{
    ident2obj().erase( std::string( value ) );
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

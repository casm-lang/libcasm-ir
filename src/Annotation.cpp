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

#include "Annotation.h"

using namespace libcasm_ir;

Annotation::Annotation( const Value::ID id, const Data& info,
    const std::function< Type::Ptr(
        const std::vector< Type::Ptr >&, const std::vector< Value::Ptr >& ) >
        inference )
: m_id( id )
, m_info( info )
, m_inference( inference )
{
    assert( m_info.size() > 0 );

    m_type_set.emplace_back( Set() );

    for( const auto& relation : m_info )
    {
        Type::Kind rt = relation.result;

        m_type_set.front().emplace( rt );

        m_map.emplace( rt, std::initializer_list< Set >{} );

        std::string key;

        m_argument_sizes.emplace( relation.argument.size() );

        for( u32 i = 0; i < relation.argument.size(); i++ )
        {
            if( m_type_set.size() <= ( i + 1 ) )
            {
                m_type_set.emplace_back( Set() );
            }

            Type::Kind at = relation.argument[ i ];
            assert( at != libcasm_ir::Type::Kind::RELATION );

            m_type_set[ i + 1 ].emplace( at );

            key += std::to_string( at ) + ";";

            if( ( i + 1 ) >= m_map[ rt ].size() )
            {
                m_map[ rt ].emplace_back( Set() );
            }

            m_map[ rt ][ i ].emplace( at );
        }

        assert( m_relation_to_type.find( key ) == m_relation_to_type.end()
                and " result type of relation already exists!" );
        m_relation_to_type[ key ] = &relation;
    }

    auto result_str = str2obj().emplace( Value::token( id ), this );
    assert( result_str.second );

    auto result_id = id2obj().emplace( id, this );
    assert( result_id.second );
}

Value::ID Annotation::id( void ) const
{
    return m_id;
}

const Annotation::Set& Annotation::resultTypes( void ) const
{
    return m_type_set[ 0 ];
}

const Annotation::Set& Annotation::argumentTypes( u8 pos ) const
{
    assert( pos < ( m_type_set.size() - 1 ) );

    return m_type_set[ pos + 1 ];
}

const std::set< std::size_t >& Annotation::argumentSizes( void ) const
{
    return m_argument_sizes;
}

const Annotation::Map& Annotation::map( void ) const
{
    return m_map;
}

libstdhl::Json::Object Annotation::json( void ) const
{
    libstdhl::Json::Object json = {};
    // = { "signature", { "result", { "arguments" } } };

    std::size_t cnt = 0;
    for( auto relation : m_info )
    {
        const auto rt = relation.result;
        const auto rs = Type::token( rt );
        const auto key = std::to_string( cnt );

        json.push_back( { { rs, {} } } );

        for( u32 i = 0; i < relation.argument.size(); i++ )
        {
            const auto at = relation.argument[ i ];
            const auto as = Type::token( at );
            json[ cnt ][ rs ].push_back( as );
        }

        cnt++;
    }

    return json;
}

std::string Annotation::dump( void ) const
{
    return json().dump( 2 );
}

Type::Ptr Annotation::inference( const std::vector< Type::Ptr >& types,
    const std::vector< Value::Ptr >& values ) const
{
    u1 first = true;
    std::size_t pos = 1;
    std::string tmp = "";
    for( auto type : types )
    {
        if( not type )
        {
            throw std::invalid_argument( "argument at position "
                                         + std::to_string( pos )
                                         + " is not defined" );
        }

        tmp += ( first ? "" : ", " );
        tmp += type->result().description();
        first = false;
        pos++;
    }

    const auto resultType = m_inference( types, values );

    if( not resultType )
    {
        throw std::domain_error(
            "no type inference defined for '" + tmp + "'" );
    }

    return resultType;
}

void Annotation::checkTypeRelation( const Type::Ptr& type ) const
{
    std::string key = "";
    for( auto argTy : type->arguments() )
    {
        const auto arg = argTy->kind();
        assert( arg != libcasm_ir::Type::Kind::RELATION );
        key += std::to_string( arg ) + ";";
    }

    auto result = m_relation_to_type.find( key );
    if( result == m_relation_to_type.end() )
    {
        throw std::domain_error( "no type relation '" + type->description()
                                 + "' defined in annotation for '"
                                 + Value::token( id() ) + "'" );
    }

    if( result->second->result != type->result().kind() )
    {
        throw std::domain_error( "return of type relation '"
                                 + type->description()
                                 + "' does not match the annotation for '"
                                 + Value::token( id() ) + "'" );
    }
}

Type::Kind Annotation::resolveTypeRelation(
    const std::vector< Value::Ptr >& operands ) const
{
    std::string key;

    for( auto argTy : operands )
    {
        const auto arg = argTy->type().kind();
        assert( arg != libcasm_ir::Type::Kind::RELATION );
        key += std::to_string( arg ) + ";";
    }

    auto result = m_relation_to_type.find( key );
    if( result == m_relation_to_type.end() )
    {
        throw std::domain_error( "no type relation found for annotation of '"
                                 + Value::token( id() ) + "'" );
    }

    return result->second->result;
}

const Annotation::Relation* Annotation::resultTypeForRelation(
    const std::vector< Type::Kind > arguments ) const
{
    std::string key;

    std::size_t pos = -1;
    std::size_t idx = 0;
    for( auto arg : arguments )
    {
        assert( arg != libcasm_ir::Type::Kind::RELATION );
        key += std::to_string( arg ) + ";";

        if( arg == libcasm_ir::Type::Kind::_TOP_ )
        {
            if( pos != -1 )
            {
                throw std::invalid_argument( "multiple unbound types defined" );
            }

            pos = idx;
        }

        idx++;
    }

    auto result = m_relation_to_type.find( key );
    if( result != m_relation_to_type.end() )
    {
        // found exact relation
        return result->second;
    }

    if( pos != -1 )
    {
        const auto argTypes = argumentTypes( pos );

        for( auto at : argTypes )
        {
            auto argTypesTemplate = arguments;
            argTypesTemplate[ pos ] = at;

            const auto result = resultTypeForRelation( argTypesTemplate );
            if( result )
            {
                return result;
            }
        }
    }

    // no result found
    return 0;
}

const Annotation& Annotation::find( const std::string& token )
{
    const auto& mapping = str2obj();
    auto result = mapping.find( token );
    if( result == mapping.end() )
    {
        throw std::domain_error( "no annotation defined for '" + token + "'" );
    }
    return *result->second;
}

const Annotation& Annotation::find( const Value::ID id )
{
    const auto& mapping = id2obj();
    auto result = mapping.find( id );
    if( result == mapping.end() )
    {
        throw std::domain_error( "no annotation defined for Value::ID '"
                                 + std::to_string( id ) + "' (aka. '"
                                 + Value::token( id ) + "')" );
    }
    return *result->second;
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

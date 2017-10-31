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

#include <libstdhl/Hash>

using namespace libcasm_ir;

static std::unordered_map< std::string, const Annotation* >& str2obj( void )
{
    static std::unordered_map< std::string, const Annotation* > obj = {};
    return obj;
}

static std::unordered_map< Value::ID, const Annotation* >& id2obj( void )
{
    static std::unordered_map< Value::ID, const Annotation* > obj = {};
    return obj;
}

Annotation::Annotation( const Value::ID valueId,
    const Annotation::Relations& relations,
    const Resolve resolve,
    const Inference inference )
: m_valueId( valueId )
, m_relations( relations )
, m_resolve( resolve )
, m_inference( inference )
{
    assert( m_relations.size() > 0 );
    m_typeSets.emplace_back( std::set< Type::ID >{} );

    for( const auto& relation : m_relations )
    {
        Type::Kind resultTypeKind = relation.result;
        m_typeSets.front().emplace( Type::ID{ resultTypeKind } );
        m_argumentSizes.emplace( relation.argument.size() );

        auto hash = libstdhl::Hash::value( relation.argument.size() );
        hash = libstdhl::Hash::combine(
            hash, std::hash< Type::Kind >()( resultTypeKind ) );

        for( std::size_t i = 0; i < relation.argument.size(); i++ )
        {
            if( m_typeSets.size() <= ( i + 1 ) )
            {
                m_typeSets.emplace_back( std::set< Type::ID >{} );
            }

            Type::Kind argumentTypeKind = relation.argument[ i ];
            assert( argumentTypeKind != libcasm_ir::Type::Kind::RELATION );
            m_typeSets[ i + 1 ].emplace( argumentTypeKind );

            hash = libstdhl::Hash::combine(
                hash, std::hash< Type::Kind >()( argumentTypeKind ) );
        }

        auto result = m_templates.emplace( hash, &relation );
        if( not result.second )
        {
            assert( !" annotation relation of return type already exists!" );
        }
    }

    auto result_str = str2obj().emplace( Value::token( valueId ), this );
    assert( result_str.second );

    auto result_id = id2obj().emplace( valueId, this );
    assert( result_id.second );
}

Value::ID Annotation::valueID( void ) const
{
    return m_valueId;
}

const Annotation::Relations& Annotation::relations( void ) const
{
    return m_relations;
}

const std::set< Type::ID >& Annotation::resultTypeIDs( void ) const
{
    return m_typeSets[ 0 ];
}

const std::set< Type::ID >& Annotation::argumentTypeIDs(
    std::size_t position ) const
{
    assert( position < ( m_typeSets.size() - 1 ) );
    return m_typeSets[ position + 1 ];
}

const std::set< std::size_t >& Annotation::argumentSizes( void ) const
{
    return m_argumentSizes;
}

libstdhl::Json::Object Annotation::json( void ) const
{
    libstdhl::Json::Object json = {};

    std::size_t cnt = 0;
    for( auto relation : m_relations )
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

void Annotation::resolve( std::vector< Type::Ptr >& argumentTypes ) const
{
    m_resolve( argumentTypes );
}

Type::ID Annotation::inference( const std::vector< Type::Ptr >& argumentTypes,
    const std::vector< Value::Ptr >& argumentValues ) const
{
    std::size_t pos = 1;
    for( auto argumentType : argumentTypes )
    {
        if( not argumentType )
        {
            throw std::invalid_argument( "argument at position "
                                         + std::to_string( pos )
                                         + " is not defined" );
        }
        pos++;
    }

    const auto inferredType = m_inference( argumentTypes, argumentValues );

    if( inferredType )
    {
        return inferredType->id();
    }
    else
    {
        return Type::ID{ Type::Kind::_SIZE_ };
    }
}

u1 Annotation::valid( const RelationType::Ptr& type ) const
{
    const auto resultTypeKind = type->result().kind();
    auto hash = libstdhl::Hash::value( type->arguments().size() );

    hash = libstdhl::Hash::combine(
        hash, std::hash< Type::Kind >()( resultTypeKind ) );

    const auto& argumentTypes = type->arguments();
    for( std::size_t i = 0; i < argumentTypes.size(); i++ )
    {
        Type::Kind argumentTypeKind = argumentTypes[ i ]->kind();

        hash = libstdhl::Hash::combine(
            hash, std::hash< Type::Kind >()( argumentTypeKind ) );
    }

    auto result = m_templates.find( hash );
    return result != m_templates.end();
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

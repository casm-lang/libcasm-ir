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

#include "Annotation.h"

using namespace libcasm_ir;

Annotation::Annotation( const Value::ID id, const Data& info )
: m_id( id )
, m_info( info )
{
    assert( m_info.size() > 0 );

    for( auto relation : m_info )
    {
        Type::ID rt = relation.result;

        m_type_set.emplace_back( Set() );
        m_type_set.back().insert( rt );

        std::string key;

        for( u32 i = 0; i < relation.argument.size(); i++ )
        {
            Type::ID at = relation.argument[ i ];
            assert( at != libcasm_ir::Type::RELATION );

            m_type_set.emplace_back( Set() );
            m_type_set.back().insert( at );
            key += std::to_string( at ) + ";";
        }

        assert( m_relation_to_type.find( key ) == m_relation_to_type.end()
                and " result type of relation already exists!" );
        m_relation_to_type[ key ] = rt;
    }
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

Type::ID Annotation::resultTypeForRelation(
    const std::vector< const Type* > arguments ) const
{
    std::string key;

    for( auto arg : arguments )
    {
        libcasm_ir::Type::ID at = arg->result().id();
        assert( at != libcasm_ir::Type::RELATION );

        key += std::to_string( at ) + ";";
    }

    auto result = m_relation_to_type.find( key );
    if( result != m_relation_to_type.end() )
    {
        return result->second;
    }

    assert( !" no result type found for requested relation! " );
    return Type::_BOTTOM_;
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

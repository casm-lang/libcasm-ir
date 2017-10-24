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

/**
   @brief    TODO

   TODO
*/

#ifndef _LIBCASM_IR_ANNOTATION_H_
#define _LIBCASM_IR_ANNOTATION_H_

#include <libcasm-ir/CasmIR>

#include <libcasm-ir/Type>
#include <libcasm-ir/Value>

#include <libstdhl/Json>

#include <set>
#include <unordered_map>

namespace libcasm_ir
{
    /**
       @extends CasmIR
     */
    class Annotation
    {
      public:
        // stores a list of type relations for possible type unification etc.
        // format is: < result type <-- < arg1_type, arg2_type, ... > >
        struct Relation
        {
            Type::Kind result;
            std::vector< Type::Kind > argument;
        };

        using Data = std::vector< Relation >;
        using Set = std::set< Type::Kind >;
        using Map = std::unordered_map< Type::Kind, std::vector< Set > >;

        Annotation( const Value::ID id, const Data& info,
            const std::function< Type::Ptr( const std::vector< Type::Ptr >&,
                const std::vector< Value::Ptr >& ) >
                inference
            = []( const std::vector< Type::Ptr >&,
                  const std::vector< Value::Ptr >& ) -> Type::Ptr {
                return nullptr;
            } );

        Value::ID id( void ) const;

        void checkTypeRelation( const Type::Ptr& type ) const;

        Type::Kind resolveTypeRelation(
            const std::vector< Value::Ptr >& operands ) const;

        const Relation* resultTypeForRelation(
            const std::vector< Type::Kind > arguments ) const;

        const Set& resultTypes( void ) const;

        const Set& argumentTypes( u8 pos ) const;

        const std::set< std::size_t >& argumentSizes( void ) const;

        const Map& map( void ) const;

        libstdhl::Json::Object json( void ) const;

        std::string dump( void ) const;

        Type::Ptr inference( const std::vector< Type::Ptr >& types,
            const std::vector< Value::Ptr >& values ) const;

      private:
        Value::ID m_id;

        const Data m_info;

        const std::function< Type::Ptr( const std::vector< Type::Ptr >&,
            const std::vector< Value::Ptr >& ) >
            m_inference;

        std::vector< Set > m_type_set;

        std::set< std::size_t > m_argument_sizes;

        std::unordered_map< std::string, const Relation* > m_relation_to_type;

        Map m_map;

        static std::unordered_map< std::string, const Annotation* >& str2obj(
            void )
        {
            static std::unordered_map< std::string, const Annotation* > cache;
            return cache;
        }

        static std::unordered_map< u8, const Annotation* >& id2obj( void )
        {
            static std::unordered_map< u8, const Annotation* > cache;
            return cache;
        }

      public:
        static const Annotation& find( const std::string& token );

        static const Annotation& find( const Value::ID id );

        template < class T >
        static const Annotation& find( void )
        {
            return T::info;
        }
    };
}

#endif // _LIBCASM_IR_ANNOTATION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

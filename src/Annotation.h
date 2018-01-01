//
//  Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
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

#include <libcasm-ir/Property>

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
        /*
          stores a list of type relations for possible type unification etc.
          format is: < resultType <-- < argumentType1, argumentType2, ... > >
          based on Type::Kind values
        */
        struct Relation
        {
            Type::Kind result;
            std::vector< Type::Kind > argument;
        };

        using Relations = std::vector< Relation >;

        using Resolve = std::function< void( std::vector< Type::Ptr >& ) >;

        using Inference = std::function< Type::Ptr(
            const std::vector< Type::Ptr >&, const std::vector< Value::Ptr >& ) >;

        using Validate = std::function< u1( const RelationType& ) >;

        Annotation(
            const Value::ID id,
            const Properties properties,
            const Relations& relations,
            const Resolve resolve,
            const Inference inference,
            const Validate validate );

        Value::ID valueID( void ) const;

        const Properties& properties( void ) const;

        const Relations& relations( void ) const;

        const std::set< Type::ID >& resultTypeIDs( void ) const;

        const std::set< Type::ID >& argumentTypeIDs( std::size_t position ) const;

        libstdhl::Json::Object json( void ) const;

        std::string dump( void ) const;

        void resolve( std::vector< Type::Ptr >& argumentTypes ) const;

        Type::ID inference(
            const std::vector< Type::Ptr >& argumentTypes,
            const std::vector< Value::Ptr >& values ) const;

        u1 valid( const RelationType& type ) const;

      private:
        Value::ID m_valueId;

        Properties m_properties;

        const Relations m_relations;

        std::unordered_map< std::vector< Type::Kind >, const Relation* > m_templates;

        std::vector< std::set< Type::ID > > m_typeSets;

        const Resolve m_resolve;

        const Inference m_inference;

        const Validate m_validate;

      public:
        static const Annotation& find( const std::string& token );

        static const Annotation& find( const Value::ID id );

        template < class T >
        static const Annotation& find( void )
        {
            return T::annotation;
        }
    };
}

#endif  // _LIBCASM_IR_ANNOTATION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

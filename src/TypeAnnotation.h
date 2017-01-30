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

/**
   @brief    TODO

   TODO
*/

#ifndef _LIB_CASMIR_TYPEANNOTATION_H_
#define _LIB_CASMIR_TYPEANNOTATION_H_

#include "CasmIR.h"
#include "Type.h"

namespace libcasm_ir
{
    class TypeAnnotation : public CasmIR
    {
      public:
        // stores a list of type relations for possible type unification etc.
        // format is: < result type <-- < arg1_type, arg2_type, ... > >
        struct Relation
        {
            Type::ID result;
            std::vector< Type::ID > argument;
        };

        using Data = std::vector< Relation >;
        using Set = std::set< Type::ID >;

      private:
        const Data m_info;

        std::vector< Set > m_type_set;

        std::unordered_map< std::string, Type::ID > m_relation_to_type;

      public:
        TypeAnnotation( const Data& info );

        Type::ID resultTypeForRelation(
            const std::vector< Type* > arguments ) const;

        const Set& resultTypes( void ) const;
        const Set& argumentTypes( u8 pos ) const;

        template < class T >
        static const Set& ResultTypes( void )
        {
            return T::info.resultTypes();
        }

        template < class T >
        static const Set& ArgumentTypes( u8 pos )
        {
            return T::info.argumentTypes( pos );
        }
    };
}

#endif /* _LIB_CASMIR_TYPEANNOTATION_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

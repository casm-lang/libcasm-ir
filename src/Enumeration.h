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

#ifndef _LIB_CASMIR_ENUMERATION_H_
#define _LIB_CASMIR_ENUMERATION_H_

#include "Value.h"

namespace libcasm_ir
{
    class EnumerationType;

    class Enumeration : public Value
    {
      public:
        using Ptr = std::shared_ptr< Enumeration >;

        Enumeration( const std::string& name,
            const std::vector< std::string >& values,
            Value::ID id = classid() );

        ~Enumeration( void ) = default;

        const std::vector< std::string >& elements( void ) const;

        u64 encode( const std::string& value ) const;

        std::string decode( const u64 value ) const;

        void accept( Visitor& visitor ) override final;

        static inline Value::ID classid( void )
        {
            return Value::ENUMERATION;
        }

        static u1 classof( Value const* obj );

      private:
        std::vector< std::string > m_values;

        std::unordered_map< std::string, u64 > m_value2uid;
    };
}

#endif // _LIB_CASMIR_ENUMERATION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

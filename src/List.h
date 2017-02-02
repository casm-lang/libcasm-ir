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

#ifndef _LIB_CASMIR_LIST_H_
#define _LIB_CASMIR_LIST_H_

#include "CasmIR.h"

namespace libcasm_ir
{
    template < typename T >
    class List : public CasmIR
    {
      public:
        using Ptr = std::shared_ptr< List >;

        using iterator = typename std::vector< typename T::Ptr >::iterator;

        using const_iterator =
            typename std::vector< typename T::Ptr >::const_iterator;

        List()
        {
        }

        std::size_t size() const
        {
            return m_elements.size();
        }

        void add( const typename T::Ptr& node )
        {
            m_elements.push_back( node );
        }

        void remove( const iterator& it )
        {
            m_elements.erase( it );
        }

        iterator begin()
        {
            return m_elements.begin();
        }

        iterator end()
        {
            return m_elements.end();
        }

        const_iterator cbegin() const
        {
            return m_elements.cbegin();
        }

        const_iterator cend() const
        {
            return m_elements.cend();
        }

      private:
        std::vector< typename T::Ptr > m_elements;
    };
}

#endif // _LIB_CASMIR_LIST_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

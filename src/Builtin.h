//
//  Copyright (c) 2015-2016 CASM Organization
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

#ifndef _LIB_CASMIR_BUILTIN_H_
#define _LIB_CASMIR_BUILTIN_H_

#include "Constant.h"
#include "Type.h"
#include "User.h"

namespace libcasm_ir
{
    class Identifier;

    class Builtin : public User
    {
      private:
        const Type::ID ret_type;
        const std::vector< std::vector< Type::ID > >& arg_type;

        static std::unordered_map< Value::ID, Builtin* >& id2obj( void )
        {
            static std::unordered_map< Value::ID, Builtin* > cache;
            return cache;
        };

        static std::unordered_map< std::string, Builtin* >& str2obj( void )
        {
            static std::unordered_map< std::string, Builtin* > cache;
            return cache;
        };

      public:
        Builtin( const char* name, Type* result, Type::ID ret_type,
            std::vector< std::vector< Type::ID > > arg_type,
            Value::ID id = Value::BUILTIN );

        ~Builtin( void );

        const Type::ID getTypeIDsOfResult( void ) const;
        const std::vector< std::vector< Type::ID > >& getTypeIDsOfArguments(
            void ) const;

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BUILTIN;
        };
        static bool classof( Value const* obj );
    };

    class CastingBuiltin : public Builtin
    {
      public:
        CastingBuiltin( const char* name, Type* result, Type::ID ret_type,
            std::vector< std::vector< Type::ID > > arg_typ,
            Value::ID id = Value::CASTING_BUILTIN );

        static inline Value::ID classid( void )
        {
            return Value::CASTING_BUILTIN;
        };
        static bool classof( Value const* obj );
    };

    class AsBooleanBuiltin : public CastingBuiltin
    {
      public:
        AsBooleanBuiltin( void );

        static inline Value::ID classid( void )
        {
            return Value::AS_BOOLEAN_BUILTIN;
        };
        static bool classof( Value const* obj );
    };

    class AsIntegerBuiltin : public CastingBuiltin
    {
      public:
        AsIntegerBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_INTEGER_BUILTIN;
        };
        static bool classof( Value const* obj );
    };

    class AsBitBuiltin : public CastingBuiltin
    {
      public:
        AsBitBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_BIT_BUILTIN;
        };
        static bool classof( Value const* obj );
    };

    class AsEnumerationBuiltin : public CastingBuiltin
    {
      public:
        AsEnumerationBuiltin( Type* result, const char* token );

        static inline Value::ID classid( void )
        {
            return Value::AS_ENUM_BUILTIN;
        };
        static bool classof( Value const* obj );
    };

    class AsStringBuiltin : public CastingBuiltin
    {
      public:
        AsStringBuiltin( void );

        static inline Value::ID classid( void )
        {
            return Value::AS_STRING_BUILTIN;
        };
        static bool classof( Value const* obj );
    };

    class AsFloatingBuiltin : public CastingBuiltin
    {
      public:
        AsFloatingBuiltin( void );

        static inline Value::ID classid( void )
        {
            return Value::AS_FLOATING_BUILTIN;
        };
        static bool classof( Value const* obj );
    };
}

#endif /* _LIB_CASMIR_BUILTIN_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

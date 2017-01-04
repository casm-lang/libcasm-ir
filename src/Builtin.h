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

#ifndef _LIB_CASMIR_BUILTIN_H_
#define _LIB_CASMIR_BUILTIN_H_

#include "Type.h"
#include "TypeAnnotation.h"
#include "User.h"

namespace libcasm_ir
{
    class Identifier;

    class Builtin : public User, public TypeAnnotation
    {
      private:
        static std::unordered_map< std::string, Builtin* >& str2obj( void )
        {
            static std::unordered_map< std::string, Builtin* > cache;
            return cache;
        };

        const char* description;

      public:
        Builtin( const char* name, Type* result, const TypeAnnotation& info,
            Value::ID id = Value::BUILTIN );

        ~Builtin( void );

        void dump( void ) const;

        const char* getDescription( void );

        static inline Value::ID classid( void )
        {
            return Value::BUILTIN;
        };
        static bool classof( Value const* obj );

        static Builtin* get( const char* name, Type* result );

        static Builtin* getAs( Type* result );

        static Builtin* getAsBoolean( Type* result );
        static Builtin* getAsInteger( Type* result );
        static Builtin* getAsBit( Type* result );
        static Builtin* getAsString( Type* result );
        static Builtin* getAsFloating( Type* result );
        static Builtin* getAsRational( Type* result );
        static Builtin* getAsEnumeration( Type* result );
    };

    class CastingBuiltin : public Builtin
    {
      public:
        CastingBuiltin( const char* name, Type* result,
            const TypeAnnotation& info, Value::ID id = Value::CASTING_BUILTIN );

        static inline Value::ID classid( void )
        {
            return Value::CASTING_BUILTIN;
        };
        static bool classof( Value const* obj );
    };

    class AsBooleanBuiltin : public CastingBuiltin
    {
      public:
        AsBooleanBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_BOOLEAN_BUILTIN;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
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

        static const TypeAnnotation info;
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

        static const TypeAnnotation info;
    };

    class AsStringBuiltin : public CastingBuiltin
    {
      public:
        AsStringBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_STRING_BUILTIN;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsFloatingBuiltin : public CastingBuiltin
    {
      public:
        AsFloatingBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_FLOATING_BUILTIN;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsRationalBuiltin : public CastingBuiltin
    {
      public:
        AsRationalBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_RATIONAL_BUILTIN;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
    };

    class AsEnumerationBuiltin : public CastingBuiltin
    {
      public:
        AsEnumerationBuiltin( Type* result );

        static inline Value::ID classid( void )
        {
            return Value::AS_ENUMERATION_BUILTIN;
        };
        static bool classof( Value const* obj );

        static const TypeAnnotation info;
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

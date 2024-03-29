//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcasm-ir/graphs/contributors>
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

#include "Builtin.h"

#include <libcasm-ir/Constant>
#include <libcasm-ir/Exception>
#include <libcasm-ir/Instruction>

#include <cassert>

using namespace libcasm_ir;

static const auto VOID = libstdhl::Memory::get< VoidType >();
static const auto BOOLEAN = libstdhl::Memory::get< BooleanType >();
static const auto INTEGER = libstdhl::Memory::get< IntegerType >();
static const auto DECIMAL = libstdhl::Memory::get< DecimalType >();
static const auto RATIONAL = libstdhl::Memory::get< RationalType >();
static const auto STRING = libstdhl::Memory::get< StringType >();

Builtin::Builtin( const Type::Ptr& type, const Value::ID id )
: User( type, id )
{
}

std::string Builtin::name( void ) const
{
    return Value::token( id() );
}

std::size_t Builtin::hash( void ) const
{
    return libstdhl::Hash::combine( classid(), std::hash< std::string >()( name() ) );
}

u1 Builtin::operator==( const Value& rhs ) const
{
    if( this == &rhs )
    {
        return true;
    }

    if( not Value::operator==( rhs ) )
    {
        return false;
    }

    const auto& other = static_cast< const Builtin& >( rhs );
    return ( this->name() == other.name() );
}

void Builtin::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 Builtin::classof( Value const* obj )
{
    return obj->id() == classid() or GeneralBuiltin::classof( obj ) or
           CastingBuiltin::classof( obj ) or
           StringifyBuiltin::classof( obj )
           // or MathBuiltin::classof( obj )
           or OperatorBuiltin::classof( obj ) or BinaryBuiltin::classof( obj );
}

u1 Builtin::available( const std::string& token )
{
    try
    {
        const auto& annotation = Annotation::find( token );

        const auto id = annotation.valueID();

        if( id == CastingBuiltin::classid() or id == AsBooleanBuiltin::classid() or
            id == AsIntegerBuiltin::classid() or id == AsBinaryBuiltin::classid() or
            id == AsStringBuiltin::classid() or id == AsDecimalBuiltin::classid() or
            id == AsRationalBuiltin::classid() or id == AsEnumerationBuiltin::classid() )
        {
            return false;
        }
    }
    catch( const std::domain_error& e )
    {
        return false;
    }
    catch( const std::exception& e )
    {
        throw e;
    }

    return true;
}

Builtin::Ptr Builtin::create( const Value::ID id, const Type::Ptr& type )
{
    switch( id )
    {
        case Value::VALUE:                        // [fallthrough]
        case Value::VALUE_LIST:                   // [fallthrough]
        case Value::USER:                         // [fallthrough]
        case Value::SPECIFICATION:                // [fallthrough]
        case Value::AGENT:                        // [fallthrough]
        case Value::RULE:                         // [fallthrough]
        case Value::DERIVED:                      // [fallthrough]
        case Value::FUNCTION:                     // [fallthrough]
        case Value::ENUMERATION:                  // [fallthrough]
        case Value::RANGE:                        // [fallthrough]
        case Value::LIST:                         // [fallthrough]
        case Value::BLOCK:                        // [fallthrough]
        case Value::EXECUTION_SEMANTICS_BLOCK:    // [fallthrough]
        case Value::PARALLEL_BLOCK:               // [fallthrough]
        case Value::SEQUENTIAL_BLOCK:             // [fallthrough]
        case Value::STATEMENT:                    // [fallthrough]
        case Value::TRIVIAL_STATEMENT:            // [fallthrough]
        case Value::BRANCH_STATEMENT:             // [fallthrough]
        case Value::CONSTANT:                     // [fallthrough]
        case Value::VOID_CONSTANT:                // [fallthrough]
        case Value::RULE_REFERENCE_CONSTANT:      // [fallthrough]
        case Value::FUNCTION_REFERENCE_CONSTANT:  // [fallthrough]
        case Value::BOOLEAN_CONSTANT:             // [fallthrough]
        case Value::INTEGER_CONSTANT:             // [fallthrough]
        case Value::BINARY_CONSTANT:              // [fallthrough]
        case Value::STRING_CONSTANT:              // [fallthrough]
        case Value::DECIMAL_CONSTANT:             // [fallthrough]
        case Value::RATIONAL_CONSTANT:            // [fallthrough]
        case Value::SYMBOLIC_CONSTANT:            // [fallthrough]
        case Value::ENUMERATION_CONSTANT:         // [fallthrough]
        case Value::RANGE_CONSTANT:               // [fallthrough]
        case Value::TUPLE_CONSTANT:               // [fallthrough]
        case Value::LIST_CONSTANT:                // [fallthrough]
        case Value::DOMAIN_CONSTANT:              // [fallthrough]
        case Value::IDENTIFIER:                   // [fallthrough]
        case Value::INSTRUCTION:                  // [fallthrough]
        case Value::UNARY_INSTRUCTION:            // [fallthrough]
        case Value::BINARY_INSTRUCTION:           // [fallthrough]
        case Value::SELECT_INSTRUCTION:           // [fallthrough]
        case Value::SKIP_INSTRUCTION:             // [fallthrough]
        case Value::FORK_INSTRUCTION:             // [fallthrough]
        case Value::MERGE_INSTRUCTION:            // [fallthrough]
        case Value::LOOKUP_INSTRUCTION:           // [fallthrough]
        case Value::UPDATE_INSTRUCTION:           // [fallthrough]
        case Value::LOCATION_INSTRUCTION:         // [fallthrough]
        case Value::CALL_INSTRUCTION:             // [fallthrough]
        case Value::LOCAL_INSTRUCTION:            // [fallthrough]
        case Value::SELF_INSTRUCTION:             // [fallthrough]
        case Value::OPERATOR_INSTRUCTION:         // [fallthrough]
        case Value::ARITHMETIC_INSTRUCTION:       // [fallthrough]
        case Value::INV_INSTRUCTION:              // [fallthrough]
        case Value::ADD_INSTRUCTION:              // [fallthrough]
        case Value::SUB_INSTRUCTION:              // [fallthrough]
        case Value::MUL_INSTRUCTION:              // [fallthrough]
        case Value::DIV_INSTRUCTION:              // [fallthrough]
        case Value::POW_INSTRUCTION:              // [fallthrough]
        case Value::MOD_INSTRUCTION:              // [fallthrough]
        case Value::COMPARE_INSTRUCTION:          // [fallthrough]
        case Value::EQU_INSTRUCTION:              // [fallthrough]
        case Value::NEQ_INSTRUCTION:              // [fallthrough]
        case Value::LTH_INSTRUCTION:              // [fallthrough]
        case Value::LEQ_INSTRUCTION:              // [fallthrough]
        case Value::GTH_INSTRUCTION:              // [fallthrough]
        case Value::GEQ_INSTRUCTION:              // [fallthrough]
        case Value::LOGICAL_INSTRUCTION:          // [fallthrough]
        case Value::OR_INSTRUCTION:               // [fallthrough]
        case Value::XOR_INSTRUCTION:              // [fallthrough]
        case Value::AND_INSTRUCTION:              // [fallthrough]
        case Value::IMP_INSTRUCTION:              // [fallthrough]
        case Value::NOT_INSTRUCTION:              // [fallthrough]
        case Value::BUILTIN:                      // [fallthrough]
        case Value::GENERAL_BUILTIN:
        {
            break;
        }
        case Value::IS_SYMBOLIC_BUILTIN:
        {
            return libstdhl::Memory::make< IsSymbolicBuiltin >( type );
        }
        case Value::ABORT_BUILTIN:
        {
            return libstdhl::Memory::make< AbortBuiltin >( type );
        }
        case Value::ASSERT_BUILTIN:
        {
            return libstdhl::Memory::make< AssertBuiltin >( type );
        }
        case Value::ASSURE_BUILTIN:
        {
            return libstdhl::Memory::make< AssureBuiltin >( type );
        }
        case Value::SIZE_BUILTIN:
        {
            return libstdhl::Memory::make< SizeBuiltin >( type );
        }
        case Value::AT_BUILTIN:
        {
            return libstdhl::Memory::make< AtBuiltin >( type );
        }

        case Value::OUTPUT_BUILTIN:
        {
            break;
        }
        case Value::PRINT_BUILTIN:
        {
            return libstdhl::Memory::make< PrintBuiltin >( type );
        }
        case Value::PRINTLN_BUILTIN:
        {
            return libstdhl::Memory::make< PrintLnBuiltin >( type );
        }

        case Value::CASTING_BUILTIN:
        {
            break;
        }
        case Value::AS_BOOLEAN_BUILTIN:
        {
            return libstdhl::Memory::make< AsBooleanBuiltin >( type );
        }
        case Value::AS_INTEGER_BUILTIN:
        {
            return libstdhl::Memory::make< AsIntegerBuiltin >( type );
        }
        case Value::AS_BINARY_BUILTIN:
        {
            return libstdhl::Memory::make< AsBinaryBuiltin >( type );
        }
        case Value::AS_STRING_BUILTIN:
        {
            return libstdhl::Memory::make< AsStringBuiltin >( type );
        }
        case Value::AS_DECIMAL_BUILTIN:
        {
            return libstdhl::Memory::make< AsDecimalBuiltin >( type );
        }
        case Value::AS_RATIONAL_BUILTIN:
        {
            return libstdhl::Memory::make< AsRationalBuiltin >( type );
        }
        case Value::AS_ENUMERATION_BUILTIN:
        {
            return libstdhl::Memory::make< AsEnumerationBuiltin >( type );
        }

        case Value::STRINGIFY_BUILTIN:
        {
            break;
        }
        case Value::DEC_BUILTIN:
        {
            return libstdhl::Memory::make< DecBuiltin >( type );
        }
        case Value::HEX_BUILTIN:
        {
            return libstdhl::Memory::make< HexBuiltin >( type );
        }
        case Value::OCT_BUILTIN:
        {
            return libstdhl::Memory::make< OctBuiltin >( type );
        }
        case Value::BIN_BUILTIN:
        {
            return libstdhl::Memory::make< BinBuiltin >( type );
        }

        case Value::OPERATOR_BUILTIN:  // [fallthrough]
        case Value::ARITHMETIC_BUILTIN:
        {
            break;
        }
        case Value::ADDU_BUILTIN:
        {
            return libstdhl::Memory::make< AdduBuiltin >( type );
        }
        case Value::ADDS_BUILTIN:
        {
            return libstdhl::Memory::make< AddsBuiltin >( type );
        }
        case Value::SUBU_BUILTIN:
        {
            return libstdhl::Memory::make< SubuBuiltin >( type );
        }
        case Value::SUBS_BUILTIN:
        {
            return libstdhl::Memory::make< SubsBuiltin >( type );
        }
        case Value::MULU_BUILTIN:
        {
            return libstdhl::Memory::make< MuluBuiltin >( type );
        }
        case Value::MULS_BUILTIN:
        {
            return libstdhl::Memory::make< MulsBuiltin >( type );
        }

        case Value::COMPARE_BUILTIN:
        {
            break;
        }
        case Value::LESU_BUILTIN:
        {
            return libstdhl::Memory::make< LesuBuiltin >( type );
        }
        case Value::LESS_BUILTIN:
        {
            return libstdhl::Memory::make< LessBuiltin >( type );
        }
        case Value::LEQU_BUILTIN:
        {
            return libstdhl::Memory::make< LequBuiltin >( type );
        }
        case Value::LEQS_BUILTIN:
        {
            return libstdhl::Memory::make< LeqsBuiltin >( type );
        }
        case Value::GREU_BUILTIN:
        {
            return libstdhl::Memory::make< GreuBuiltin >( type );
        }
        case Value::GRES_BUILTIN:
        {
            return libstdhl::Memory::make< GresBuiltin >( type );
        }
        case Value::GEQU_BUILTIN:
        {
            return libstdhl::Memory::make< GequBuiltin >( type );
        }
        case Value::GEQS_BUILTIN:
        {
            return libstdhl::Memory::make< GeqsBuiltin >( type );
        }

        case Value::BINARY_BUILTIN:
        {
            break;
        }
        case Value::ZEXT_BUILTIN:
        {
            return libstdhl::Memory::make< ZextBuiltin >( type );
        }
        case Value::SEXT_BUILTIN:
        {
            return libstdhl::Memory::make< SextBuiltin >( type );
        }
        case Value::TRUNC_BUILTIN:
        {
            return libstdhl::Memory::make< TruncBuiltin >( type );
        }
        case Value::SHL_BUILTIN:
        {
            return libstdhl::Memory::make< ShlBuiltin >( type );
        }
        case Value::SHR_BUILTIN:
        {
            return libstdhl::Memory::make< ShrBuiltin >( type );
        }
        case Value::ASHR_BUILTIN:
        {
            return libstdhl::Memory::make< AshrBuiltin >( type );
        }
        case Value::CLZ_BUILTIN:
        {
            return libstdhl::Memory::make< ClzBuiltin >( type );
        }
        case Value::CLO_BUILTIN:
        {
            return libstdhl::Memory::make< CloBuiltin >( type );
        }
        case Value::CLS_BUILTIN:
        {
            return libstdhl::Memory::make< ClsBuiltin >( type );
        }

        case Value::_SIZE_:
        {
            break;
        }
    }

    assert( not " invalid ID to create IR built-in " );
    return nullptr;
}

//------------------------------------------------------------------------------

//
// GeneralBuiltin
//

GeneralBuiltin::GeneralBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 GeneralBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or IsSymbolicBuiltin::classof( obj ) or
           AbortBuiltin::classof( obj ) or AssertBuiltin::classof( obj ) or
           AssureBuiltin::classof( obj ) or SizeBuiltin::classof( obj ) or
           AtBuiltin::classof( obj ) or OutputBuiltin::classof( obj );
}

static const Properties general_builtin_properties = { Property::SIDE_EFFECT_FREE, Property::PURE };

//
// IsSymbolicBuiltin
//

IsSymbolicBuiltin::IsSymbolicBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

void IsSymbolicBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];
    res = BooleanConstant( arg.symbolic() );
}

const Annotation IsSymbolicBuiltin::annotation(
    classid(),
    general_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BINARY,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::DECIMAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::STRING,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::ENUMERATION,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return BOOLEAN;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isBoolean();
        }
        else
        {
            return false;
        }
    } );

u1 IsSymbolicBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AbortBuiltin
//

AbortBuiltin::AbortBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

void AbortBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    throw AbortException( "aborting" );
}

const Annotation AbortBuiltin::annotation(
    classid(),
    general_builtin_properties,
    Annotation::Relations{

        { Type::Kind::VOID, {} },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 0 )
        {
            throw InternalException( "types.size() != 0" );
        }
        return VOID;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 0 )
        {
            return type.result().isVoid();
        }
        else
        {
            return false;
        }
    } );

u1 AbortBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AssertBuiltin
//

AssertBuiltin::AssertBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

void AssertBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& cond = reg[ 0 ];
    if( cond.symbolic() )
    {
        // TODO: @moosbruggerj: add assert trace
        res = VoidConstant();
        return;
    }
    assert( cond.type().isBoolean() );
    const auto& c = static_cast< const BooleanConstant& >( cond ).value();

    if( not c.defined() )
    {
        throw UndefinedConstantException( "assertion on undefined value" );
    }
    else
    {
        if( not c.value() )
        {
            throw AssertionException( "assertion failed" );
        }
    }

    res = VoidConstant();
}

const Annotation AssertBuiltin::annotation(
    classid(),
    general_builtin_properties,
    Annotation::Relations{

        { Type::Kind::VOID,
          {
              Type::Kind::BOOLEAN,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }

        if( not types[ 0 ] )
        {
            types[ 0 ] = BOOLEAN;
        }
    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return VOID;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isVoid() and type.arguments()[ 0 ]->isBoolean();
        }
        else
        {
            return false;
        }
    } );

u1 AssertBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AssureBuiltin
//

AssureBuiltin::AssureBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

void AssureBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& cond = reg[ 0 ];
    assert( cond.type().isBoolean() );
    const auto& c = static_cast< const BooleanConstant& >( cond ).value();

    if( not c.defined() )
    {
        throw UndefinedConstantException( "assurance on undefined value" );
    }
    else
    {
        if( not c.value() )
        {
            throw AssuranceException( "assurance failed" );
        }
    }

    res = VoidConstant();
}

const Annotation AssureBuiltin::annotation(
    classid(),
    general_builtin_properties,
    Annotation::Relations{

        { Type::Kind::VOID,
          {
              Type::Kind::BOOLEAN,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }

        if( not types[ 0 ] )
        {
            types[ 0 ] = BOOLEAN;
        }
    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return VOID;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isVoid() and type.arguments()[ 0 ]->isBoolean();
        }
        else
        {
            return false;
        }
    } );

u1 AssureBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SizeBuiltin
//

SizeBuiltin::SizeBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

void SizeBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& object = reg[ 0 ];
    assert( object.type().isEnumeration() or object.type().isList() or object.type().isRange() );

    if( not object.defined() )
    {
        res = IntegerConstant();
        return;
    }

    if( object.type().isEnumeration() )
    {
        const auto enumeration = static_cast< const EnumerationType& >( object.type() );
        res = IntegerConstant( enumeration.kind().elements().size() );
    }
    else if( object.type().isList() )
    {
        const auto list = static_cast< const ListConstant& >( object ).value();
        res = IntegerConstant( list->elements().size() );
    }
    else
    {
        assert( object.type().isRange() );
        const auto& range = static_cast< const RangeConstant& >( object ).value();

        const auto& a = static_cast< IntegerConstant& >( *range->from() ).value();
        const auto& b = static_cast< IntegerConstant& >( *range->to() ).value();

        if( a >= b )
        {
            res = IntegerConstant( a - b + 1 );
        }
        else
        {
            res = IntegerConstant( b - a + 1 );
        }
    }
}

const Annotation SizeBuiltin::annotation(
    classid(),
    general_builtin_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::ENUMERATION,
          } },

        { Type::Kind::INTEGER,
          {
              Type::Kind::LIST,
          } },

        { Type::Kind::INTEGER,
          {
              Type::Kind::RANGE,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return INTEGER;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            const auto argument = type.arguments()[ 0 ];

            return type.result().isInteger() and
                   ( argument->isEnumeration() or argument->isList() or argument->isRange() );
        }
        else
        {
            return false;
        }
    } );

u1 SizeBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AtBuiltin
//

AtBuiltin::AtBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

void AtBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& object = reg[ 0 ];
    const auto& index = reg[ 1 ];

    assert( index.type().isInteger() );
    const auto& position = static_cast< const IntegerConstant& >( index ).value();

    if( not object.defined() or not index.defined() )
    {
        res = Constant::undef( object.type().ptr_result() );
        return;
    }

    if( object.type().isString() )
    {
        const auto& data = static_cast< const StringConstant& >( object ).value().toString();
        if( position >= 1 and position <= data.size() )
        {
            const auto value = position.value() - 1;
            const auto& character = data.at( value );
            res = StringConstant( std::string( 1, character ) );
        }
        else
        {
            res = StringConstant();
        }
        return;
    }

    assert( object.type().isTuple() );
    const auto& tuple = static_cast< const TupleConstant& >( object );

    if( position >= 1 and position <= tuple.cardinality() )
    {
        res = tuple.value( position.value() );
        assert( isa< Constant >( res ) );
        return;
    }

    res = Constant::undef( object.type().ptr_result() );
}

const Annotation AtBuiltin::annotation(
    classid(),
    { Property::SIDE_EFFECT_FREE },
    Annotation::Relations{

        { Type::Kind::_SIZE_,
          {
              Type::Kind::LIST,
              Type::Kind::INTEGER,
          } },
    },
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }
    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }

        auto object = types[ 0 ];
        if( not object->isList() )
        {
            throw InternalException( "invalid object type" );
        }

        return object->ptr_result();
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() != 2 )
        {
            throw InternalException( "type.arguments().size() != 2" );
        }

        const auto& object = type.arguments().at( 0 );
        const auto& index = type.arguments().at( 1 );

        if( not index->isInteger() )
        {
            return false;
        }

        if( object->isList() )
        {
            // List< T > * Integer -> T
            if( object->result() == type.result() )
            {
                return true;
            }
        }

        return false;
    } );

u1 AtBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// OutputBuiltin
//

OutputBuiltin::OutputBuiltin(
    const Type::Ptr& type, const std::string& channel, u1 newline, const Value::ID id )
: GeneralBuiltin( type, classid() )
, m_channel( channel )
, m_newline( newline )
{
}

std::string OutputBuiltin::channel( void ) const
{
    return m_channel;
}

u1 OutputBuiltin::newline( void ) const
{
    return m_newline;
}

u1 OutputBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or PrintBuiltin::classof( obj ) or PrintLnBuiltin::classof( obj );
}

//
// PrintBuiltin
//

PrintBuiltin::PrintBuiltin( const Type::Ptr& type )
: OutputBuiltin( type, "$stdout$", false, classid() )
{
}

void PrintBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& txt = reg[ 0 ];
    assert( txt.type().isString() );
    const auto stringConstant = static_cast< const StringConstant& >( txt );

    std::cout << stringConstant.toString();

    res = VoidConstant();
}

const Annotation PrintBuiltin::annotation(
    classid(),
    general_builtin_properties,
    Annotation::Relations{

        { Type::Kind::VOID,
          {
              Type::Kind::STRING,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }

        if( not types[ 0 ] )
        {
            types[ 0 ] = STRING;
        }
    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        const auto& argumentType = types[ 0 ];

        if( not argumentType->isString() )
        {
            throw TypeArgumentException(
                "found '" + argumentType->description() + "', but expects '" +
                    STRING->description() + "'",
                0 );
        }

        return VOID;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isVoid() and type.arguments()[ 0 ]->isString();
        }
        else
        {
            return false;
        }
    } );

u1 PrintBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// PrintLnBuiltin
//

PrintLnBuiltin::PrintLnBuiltin( const Type::Ptr& type )
: OutputBuiltin( type, "$stdout$", true, classid() )
{
}

void PrintLnBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& txt = reg[ 0 ];
    assert( txt.type().isString() );
    const auto stringConstant = static_cast< const StringConstant& >( txt );

    std::cout << stringConstant.toString() << "\n";

    res = VoidConstant();
}

const Annotation PrintLnBuiltin::annotation(
    classid(),
    general_builtin_properties,
    Annotation::Relations{

        { Type::Kind::VOID,
          {
              Type::Kind::STRING,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }

        if( not types[ 0 ] )
        {
            types[ 0 ] = STRING;
        }
    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        const auto& argumentType = types[ 0 ];

        if( not argumentType->isString() )
        {
            throw TypeArgumentException(
                "found '" + argumentType->description() + "', but expects '" +
                    STRING->description() + "'",
                0 );
        }

        return VOID;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isVoid() and type.arguments()[ 0 ]->isString();
        }
        else
        {
            return false;
        }
    } );

u1 PrintLnBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//------------------------------------------------------------------------------

//
// CastingBuiltin
//

CastingBuiltin::CastingBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 CastingBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or AsBooleanBuiltin::classof( obj ) or
           AsIntegerBuiltin::classof( obj ) or AsBinaryBuiltin::classof( obj ) or
           AsEnumerationBuiltin::classof( obj ) or AsStringBuiltin::classof( obj ) or
           AsDecimalBuiltin::classof( obj );
}

static const Properties casting_builtin_properties = { Property::SIDE_EFFECT_FREE, Property::PURE };

//
// AsBooleanBuiltin
//

AsBooleanBuiltin::AsBooleanBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

void AsBooleanBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    if( not arg.defined() )
    {
        res = BooleanConstant();
        return;
    }

    switch( arg.typeId().kind() )
    {
        case Type::Kind::BOOLEAN:
        {
            res = arg;
            break;
        }
        case Type::Kind::INTEGER:
        {
            const auto c = static_cast< const IntegerConstant& >( arg ).value();
            res = BooleanConstant( c != 0 );
            break;
        }
        case Type::Kind::BINARY:
        {
            const auto c = static_cast< const BinaryConstant& >( arg ).value();
            res = BooleanConstant( c != 0 );
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
        }
    }
}

const Annotation AsBooleanBuiltin::annotation(
    classid(),
    casting_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BOOLEAN,
          {
              Type::Kind::BINARY,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return BOOLEAN;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isBoolean();
        }
        else
        {
            return false;
        }
    } );

u1 AsBooleanBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsIntegerBuiltin
//

AsIntegerBuiltin::AsIntegerBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

void AsIntegerBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    if( not arg.defined() )
    {
        res = IntegerConstant();
        return;
    }

    switch( arg.typeId().kind() )
    {
        case Type::Kind::BOOLEAN:
        {
            const auto& c = static_cast< const BooleanConstant& >( arg ).value();
            res = IntegerConstant( c == true ? 1 : 0 );
            break;
        }
        case Type::Kind::INTEGER:
        {
            res = arg;
            break;
        }
        case Type::Kind::BINARY:
        {
            const auto& t = static_cast< const BinaryType& >( arg.type() );
            const auto& c = static_cast< const BinaryConstant& >( arg ).value();

            if( c.isSet( t.bitsize() ) )
            {
                Operation::execute< InvInstruction >( t.ptr_type(), res, arg );
                const auto& r = static_cast< const BinaryConstant& >( res ).value();
                res = IntegerConstant( r, true );
            }
            else
            {
                res = IntegerConstant( c );
            }
            break;
        }
        case Type::Kind::DECIMAL:
        {
            const auto& c = static_cast< const DecimalConstant& >( arg ).value();
            res = IntegerConstant( c.toInteger() );
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
        }
    }
}

const Annotation AsIntegerBuiltin::annotation(
    classid(),
    casting_builtin_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::INTEGER,
          {
              Type::Kind::INTEGER,
          } },

        { Type::Kind::INTEGER,
          {
              Type::Kind::BINARY,
          } },

        { Type::Kind::INTEGER,
          {
              Type::Kind::DECIMAL,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        const auto& arg = types[ 0 ];
        if( arg->isInteger() )
        {
            const auto& integerType = static_cast< const IntegerType& >( *arg );
            if( integerType.range() )
            {
                throw InternalException( "unimplemented" );
            }
        }
        return INTEGER;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isInteger();
        }
        else
        {
            return false;
        }
    } );

u1 AsIntegerBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsBinaryBuiltin
//

AsBinaryBuiltin::AsBinaryBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

void AsBinaryBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    assert( type().result().isBinary() );
    const auto resultType = std::static_pointer_cast< BinaryType >( type().result().ptr_type() );

    if( not arg.defined() )
    {
        res = BinaryConstant( resultType );
        return;
    }

    switch( arg.typeId().kind() )
    {
        case Type::Kind::BOOLEAN:
        {
            const auto& c = static_cast< const BooleanConstant& >( arg ).value();
            res = BinaryConstant( resultType, libstdhl::Type::createNatural( c == true ? 1 : 0 ) );
            break;
        }
        case Type::Kind::INTEGER:
        {
            const auto& c = static_cast< const IntegerConstant& >( arg ).value();

            try
            {
                const auto mask = Utility::createMask( resultType->bitsize() );

                libstdhl::Type::Natural nat;
                if( c >= 0 )
                {
                    nat = libstdhl::Type::createNatural( c );

                    const auto check = nat & mask;
                    if( nat != check )
                    {
                        res = BinaryConstant( resultType );
                        break;
                    }

                    if( nat.isSet( resultType->bitsize() ) )
                    {
                        res = BinaryConstant( resultType );
                        break;
                    }
                }
                else
                {
                    nat = libstdhl::Type::createNatural( -c );

                    const auto check = nat & mask;
                    if( nat != check )
                    {
                        res = BinaryConstant( resultType );
                        break;
                    }

                    nat = nat ^ mask;
                    nat += 1;
                    if( not nat.isSet( resultType->bitsize() ) )
                    {
                        res = BinaryConstant( resultType );
                        break;
                    }

                    nat = nat & mask;
                }

                res = BinaryConstant( resultType, nat );
            }
            catch( const std::domain_error& e )
            {
                res = BinaryConstant( resultType );
            }
            catch( const std::invalid_argument& e )
            {
                res = BinaryConstant( resultType );
            }
            break;
        }
        case Type::Kind::BINARY:
        {
            const auto& c = static_cast< const BinaryConstant& >( arg );
            assert( c.type().isBinary() );
            const auto& valueType = static_cast< const BinaryType& >( c.type() );

            if( resultType->bitsize() < valueType.bitsize() )
            {
                // perform a truncation of the binary value!
                const auto offset = IntegerConstant( c.value() );
                const Constant values[ 2 ] = { arg, offset };
                const auto truncType = libstdhl::Memory::get< RelationType >(
                    resultType, Types( { c.type().ptr_type(), offset.type().ptr_type() } ) );
                Operation::execute< TruncBuiltin >( truncType, res, values, 2 );
            }
            else if( resultType->bitsize() > valueType.bitsize() )
            {
                // perform a zero extension
                res = BinaryConstant( resultType, libstdhl::Type::createNatural( c.value() ) );
            }
            else
            {
                res = arg;
            }
            break;
        }
        case Type::Kind::DECIMAL:
        {
            const auto& c = static_cast< const DecimalConstant& >( arg ).value();

            try
            {
                const auto i = c.toInteger();
                if( i >= 0 )
                {
                    const auto nat = libstdhl::Type::createNatural( i );
                    res = BinaryConstant( resultType, nat );
                }
                else
                {
                    const auto mask = Utility::createMask( resultType->bitsize() );
                    auto nat = libstdhl::Type::createNatural( -i );
                    nat = nat ^ mask;
                    nat += 1;
                    res = BinaryConstant( resultType, nat );
                }
            }
            catch( const std::domain_error& e )
            {
                res = BinaryConstant( resultType );
            }
            catch( const std::invalid_argument& e )
            {
                res = BinaryConstant( resultType );
            }
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
        }
    }
}

const Annotation AsBinaryBuiltin::annotation(
    classid(),
    casting_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BINARY,
          {
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::DECIMAL,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return nullptr;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isBinary();
        }
        else
        {
            return false;
        }
    } );

u1 AsBinaryBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsStringBuiltin
//

AsStringBuiltin::AsStringBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

void AsStringBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    if( arg.defined() )
    {
        res = StringConstant( arg.name() );
    }
    else
    {
        res = StringConstant();
    }
}

const Annotation AsStringBuiltin::annotation(
    classid(),
    casting_builtin_properties,
    Annotation::Relations{

        { Type::Kind::STRING,
          {
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::INTEGER,
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::BINARY,
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::DECIMAL,
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::STRING,
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::ENUMERATION,
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::RULE_REFERENCE,
          } },

        { Type::Kind::STRING,
          {
              Type::Kind::FUNCTION_REFERENCE,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return STRING;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isString();
        }
        else
        {
            return false;
        }
    } );

u1 AsStringBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsDecimalBuiltin
//

AsDecimalBuiltin::AsDecimalBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

void AsDecimalBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    if( arg.defined() )
    {
        switch( arg.typeId().kind() )
        {
            case Type::Kind::DECIMAL:
            {
                const auto& c = static_cast< const DecimalConstant& >( arg );
                res = c;
                break;
            }
            case Type::Kind::BOOLEAN:
            {
                const auto& c = static_cast< const BooleanConstant& >( arg ).value();
                res = DecimalConstant( c == true ? 1.0 : 0.0 );
                break;
            }
            case Type::Kind::INTEGER:
            {
                const auto& c = static_cast< const IntegerConstant& >( arg ).value();
                res = DecimalConstant( c );
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& c = static_cast< const BinaryConstant& >( arg ).value();
                res = DecimalConstant( c );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
    else
    {
        res = DecimalConstant();
    }
}

const Annotation AsDecimalBuiltin::annotation(
    classid(),
    casting_builtin_properties,
    Annotation::Relations{

        { Type::Kind::DECIMAL,
          {
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::DECIMAL,
          {
              Type::Kind::INTEGER,
          } },

        { Type::Kind::DECIMAL,
          {
              Type::Kind::BINARY,
          } },

        { Type::Kind::DECIMAL,
          {
              Type::Kind::DECIMAL,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return DECIMAL;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isDecimal();
        }
        else
        {
            return false;
        }
    } );

u1 AsDecimalBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsRationalBuiltin
//

AsRationalBuiltin::AsRationalBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

void AsRationalBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    assert( type().result().isRational() );
    const auto resultType =
        std::static_pointer_cast< EnumerationType >( type().result().ptr_type() );

    if( arg.defined() )
    {
        switch( arg.typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                const auto& c = static_cast< const BooleanConstant& >( arg ).value();
                res = RationalConstant( libstdhl::Type::createRational(
                    libstdhl::Type::createInteger( ( libstdhl::u64 )( c == true ? 1 : 0 ) ) ) );
                break;
            }
            case Type::Kind::INTEGER:
            {
                const auto& c = static_cast< const IntegerConstant& >( arg ).value();
                res = RationalConstant( libstdhl::Type::createRational( c ) );
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& c = static_cast< const BinaryConstant& >( arg ).value();
                res = RationalConstant( libstdhl::Type::createRational( c ) );
                break;
            }
            case Type::Kind::RATIONAL:
            {
                res = arg;
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
    else
    {
        res = RationalConstant();
    }
}

const Annotation AsRationalBuiltin::annotation(
    classid(),
    casting_builtin_properties,
    Annotation::Relations{

        { Type::Kind::RATIONAL,
          {
              Type::Kind::BOOLEAN,
          } },

        { Type::Kind::RATIONAL,
          {
              Type::Kind::INTEGER,
          } },

        { Type::Kind::RATIONAL,
          {
              Type::Kind::RATIONAL,
          } },

        { Type::Kind::RATIONAL,
          {
              Type::Kind::BINARY,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return RATIONAL;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isRational();
        }
        else
        {
            return false;
        }
    } );

u1 AsRationalBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsEnumerationBuiltin
//

AsEnumerationBuiltin::AsEnumerationBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

void AsEnumerationBuiltin::execute(
    Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    assert( type().result().isEnumeration() );
    const auto resultType =
        std::static_pointer_cast< EnumerationType >( type().result().ptr_type() );

    if( arg.defined() )
    {
        switch( arg.typeId().kind() )
        {
            case Type::Kind::ENUMERATION:
            {
                if( arg.type() == *resultType )
                {
                    res = arg;
                }
                else
                {
                    res = EnumerationConstant( resultType );
                }
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
    else
    {
        res = EnumerationConstant( resultType );
    }
}

const Annotation AsEnumerationBuiltin::annotation(
    classid(),
    casting_builtin_properties,
    Annotation::Relations{

        { Type::Kind::ENUMERATION,
          {
              Type::Kind::ENUMERATION,
          } },

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return nullptr;  // TODO: PPA: fetch through values a enumeration kind
                         // hint and return its type!
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isEnumeration();
        }
        else
        {
            return false;
        }
    } );

u1 AsEnumerationBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// StringifyBuiltin
//

StringifyBuiltin::StringifyBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 StringifyBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or DecBuiltin::classof( obj ) or HexBuiltin::classof( obj ) or
           OctBuiltin::classof( obj ) or BinBuiltin::classof( obj );
}

static const Properties stringify_builtin_properties = { Property::SIDE_EFFECT_FREE,
                                                         Property::PURE };

static const Annotation::Relations stringify_builtin_data = {

    { Type::Kind::STRING,
      {
          Type::Kind::BOOLEAN,
      } },

    { Type::Kind::STRING,
      {
          Type::Kind::INTEGER,
      } },

    { Type::Kind::STRING,
      {
          Type::Kind::BINARY,
      } },

    { Type::Kind::STRING,
      {
          Type::Kind::DECIMAL,
      } },

    { Type::Kind::STRING,
      {
          Type::Kind::RATIONAL,
      } },

    { Type::Kind::STRING,
      {
          Type::Kind::ENUMERATION,
      } }

};

static const auto stringify_builtin_resolve = []( std::vector< Type::Ptr >& types ) {};

static const auto stringify_builtin_inference =
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
    if( types.size() != 1 )
    {
        throw InternalException( "types.size() != 1" );
    }
    return STRING;
};

static const auto stringify_builtin_validate = []( const RelationType& type ) -> u1 {
    if( type.arguments().size() == 1 )
    {
        return type.result().isString();
    }
    else
    {
        return false;
    }
};

//
// DecBuiltin
//

DecBuiltin::DecBuiltin( const Type::Ptr& type )
: StringifyBuiltin( type, classid() )
{
}

void DecBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    if( arg.defined() )
    {
        switch( arg.typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                const auto& c = static_cast< const BooleanConstant& >( arg ).value();
                res = StringConstant( c.value() ? "1" : "0" );
                break;
            }
            case Type::Kind::INTEGER:
            {
                const auto& c = static_cast< const IntegerConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::DECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& c = static_cast< const BinaryConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::DECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::DECIMAL:
            {
                const auto& c = static_cast< const DecimalConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::DECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::RATIONAL:
            {
                const auto& c = static_cast< const RationalConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::DECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::ENUMERATION:
            {
                const auto& c = static_cast< const EnumerationConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::DECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
    else
    {
        res = StringConstant();
    }
}

const Annotation DecBuiltin::annotation(
    classid(),
    stringify_builtin_properties,
    stringify_builtin_data,
    stringify_builtin_resolve,
    stringify_builtin_inference,
    stringify_builtin_validate );

u1 DecBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// HexBuiltin
//

HexBuiltin::HexBuiltin( const Type::Ptr& type )
: StringifyBuiltin( type, classid() )
{
}

void HexBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    if( arg.defined() )
    {
        switch( arg.typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                const auto& c = static_cast< const BooleanConstant& >( arg ).value();
                res = StringConstant( c.value() ? "1" : "0" );
                break;
            }
            case Type::Kind::INTEGER:
            {
                const auto& c = static_cast< const IntegerConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::HEXADECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& c = static_cast< const BinaryConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::HEXADECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::DECIMAL:
            {
                const auto& c = static_cast< const DecimalConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::HEXADECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::RATIONAL:
            {
                const auto& c = static_cast< const RationalConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::HEXADECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::ENUMERATION:
            {
                const auto& c = static_cast< const EnumerationConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::HEXADECIMAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
    else
    {
        res = StringConstant();
    }
}

const Annotation HexBuiltin::annotation(
    classid(),
    stringify_builtin_properties,
    stringify_builtin_data,
    stringify_builtin_resolve,
    stringify_builtin_inference,
    stringify_builtin_validate );

u1 HexBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// OctBuiltin
//

OctBuiltin::OctBuiltin( const Type::Ptr& type )
: StringifyBuiltin( type, classid() )
{
}

void OctBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    if( arg.defined() )
    {
        switch( arg.typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                const auto& c = static_cast< const BooleanConstant& >( arg ).value();
                res = StringConstant( c.value() ? "1" : "0" );
                break;
            }
            case Type::Kind::INTEGER:
            {
                const auto& c = static_cast< const IntegerConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::OCTAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& c = static_cast< const BinaryConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::OCTAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::DECIMAL:
            {
                const auto& c = static_cast< const DecimalConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::OCTAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::RATIONAL:
            {
                const auto& c = static_cast< const RationalConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::OCTAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::ENUMERATION:
            {
                const auto& c = static_cast< const EnumerationConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::OCTAL, libstdhl::Type::Literal::NONE >() );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
    else
    {
        res = StringConstant();
    }
}

const Annotation OctBuiltin::annotation(
    classid(),
    stringify_builtin_properties,
    stringify_builtin_data,
    stringify_builtin_resolve,
    stringify_builtin_inference,
    stringify_builtin_validate );

u1 OctBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// BinBuiltin
//

BinBuiltin::BinBuiltin( const Type::Ptr& type )
: StringifyBuiltin( type, classid() )
{
}

void BinBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& arg = reg[ 0 ];

    if( arg.defined() )
    {
        switch( arg.typeId().kind() )
        {
            case Type::Kind::BOOLEAN:
            {
                const auto& c = static_cast< const BooleanConstant& >( arg ).value();
                res = StringConstant( c.value() ? "1" : "0" );
                break;
            }
            case Type::Kind::INTEGER:
            {
                const auto& c = static_cast< const IntegerConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::BINARY, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& c = static_cast< const BinaryConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::BINARY, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::DECIMAL:
            {
                const auto& c = static_cast< const DecimalConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::BINARY, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::RATIONAL:
            {
                const auto& c = static_cast< const RationalConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::BINARY, libstdhl::Type::Literal::NONE >() );
                break;
            }
            case Type::Kind::ENUMERATION:
            {
                const auto& c = static_cast< const EnumerationConstant& >( arg ).value();
                res = StringConstant(
                    c.to< libstdhl::Type::Radix::BINARY, libstdhl::Type::Literal::NONE >() );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
    else
    {
        res = StringConstant();
    }
}

const Annotation BinBuiltin::annotation(
    classid(),
    stringify_builtin_properties,
    stringify_builtin_data,
    stringify_builtin_resolve,
    stringify_builtin_inference,
    stringify_builtin_validate );

u1 BinBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// OperatorBuiltin
//

OperatorBuiltin::OperatorBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 OperatorBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or ArithmeticBuiltin::classof( obj ) or
           CompareBuiltin::classof( obj );
}

//
// ArithmeticBuiltin
//

ArithmeticBuiltin::ArithmeticBuiltin( const Type::Ptr& type, const Value::ID id )
: OperatorBuiltin( type, id )
{
}

static const Properties arithmetic_builtin_properties = { Property::SIDE_EFFECT_FREE,
                                                          Property::PURE };

static const Annotation::Relations arithmetic_builtin_data = {

    { Type::Kind::INTEGER,
      {
          Type::Kind::INTEGER,
          Type::Kind::INTEGER,
      } },

    { Type::Kind::BINARY,
      {
          Type::Kind::BINARY,
          Type::Kind::BINARY,
      } }
};

static const auto arithmetic_builtin_resolve = []( std::vector< Type::Ptr >& types ) {
    if( types.size() != 2 )
    {
        throw InternalException( "types.size() != 2" );
    }

    if( not types[ 0 ] )
    {
        types[ 0 ] = types[ 1 ];
    }
    else if( not types[ 1 ] )
    {
        types[ 1 ] = types[ 0 ];
    }
};

static const auto arithmetic_builtin_inference =
    []( const std::vector< Type::Ptr >& types, const std::vector< Value::Ptr >& ) -> Type::Ptr {
    if( types.size() != 2 )
    {
        throw InternalException( "types.size() != 2" );
    }
    const auto& lhs = types[ 0 ];
    const auto& rhs = types[ 1 ];
    if( *lhs == *rhs )
    {
        return lhs;
    }
    else
    {
        return nullptr;
    }
};

static const auto arithmetic_builtin_validate = []( const RelationType& type ) -> u1 {
    if( type.arguments().size() == 2 )
    {
        return type.result() == *type.arguments()[ 0 ] and type.result() == *type.arguments()[ 1 ];
    }
    else
    {
        return false;
    }
};

u1 ArithmeticBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or AdduBuiltin::classof( obj ) or AddsBuiltin::classof( obj ) or
           SubuBuiltin::classof( obj ) or SubsBuiltin::classof( obj ) or
           MuluBuiltin::classof( obj ) or MulsBuiltin::classof( obj );
}

//
// AdduBuiltin
//

AdduBuiltin::AdduBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

void AdduBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& lhs = reg[ 0 ];
    const auto& rhs = reg[ 1 ];

    if( not lhs.defined() or not rhs.defined() )
    {
        res = Constant::undef( lhs.type().ptr_type() );
        return;
    }

    switch( lhs.typeId().kind() )
    {
        case Type::Kind::INTEGER:
        {
            const auto& lval = static_cast< const IntegerConstant& >( lhs ).value();
            const auto& rval = static_cast< const IntegerConstant& >( rhs ).value();

            res = IntegerConstant( lval + rval );
            break;
        }
        default:
        {
            throw InternalException( "unimplemented '" + description() + "'" );
        }
    }
}

const Annotation AdduBuiltin::annotation(
    classid(),
    arithmetic_builtin_properties,
    arithmetic_builtin_data,
    arithmetic_builtin_resolve,
    arithmetic_builtin_inference,
    arithmetic_builtin_validate );

u1 AdduBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AddsBuiltin
//

AddsBuiltin::AddsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

void AddsBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation AddsBuiltin::annotation(
    classid(),
    arithmetic_builtin_properties,
    arithmetic_builtin_data,
    arithmetic_builtin_resolve,
    arithmetic_builtin_inference,
    arithmetic_builtin_validate );

u1 AddsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SubuBuiltin
//

SubuBuiltin::SubuBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

void SubuBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation SubuBuiltin::annotation(
    classid(),
    arithmetic_builtin_properties,
    arithmetic_builtin_data,
    arithmetic_builtin_resolve,
    arithmetic_builtin_inference,
    arithmetic_builtin_validate );

u1 SubuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SubsBuiltin
//

SubsBuiltin::SubsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

void SubsBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation SubsBuiltin::annotation(
    classid(),
    arithmetic_builtin_properties,
    arithmetic_builtin_data,
    arithmetic_builtin_resolve,
    arithmetic_builtin_inference,
    arithmetic_builtin_validate );

u1 SubsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// MuluBuiltin
//

MuluBuiltin::MuluBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

void MuluBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation MuluBuiltin::annotation(
    classid(),
    arithmetic_builtin_properties,
    arithmetic_builtin_data,
    arithmetic_builtin_resolve,
    arithmetic_builtin_inference,
    arithmetic_builtin_validate );

u1 MuluBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// MulsBuiltin
//

MulsBuiltin::MulsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

void MulsBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation MulsBuiltin::annotation(
    classid(),
    arithmetic_builtin_properties,
    arithmetic_builtin_data,
    arithmetic_builtin_resolve,
    arithmetic_builtin_inference,
    arithmetic_builtin_validate );

u1 MulsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// CompareBuiltin
//

CompareBuiltin::CompareBuiltin( const Type::Ptr& type, const Value::ID id )
: OperatorBuiltin( type, id )
{
}

static const auto compare_builtin_properties = arithmetic_builtin_properties;

const Annotation::Relations compare_builtin_data = {

    { Type::Kind::BOOLEAN,
      {
          Type::Kind::INTEGER,
          Type::Kind::INTEGER,
      } },

    { Type::Kind::BOOLEAN,
      {
          Type::Kind::RATIONAL,
          Type::Kind::RATIONAL,
      } },

    { Type::Kind::BOOLEAN,
      {
          Type::Kind::BINARY,
          Type::Kind::BINARY,
      } },

    { Type::Kind::BOOLEAN,
      {
          Type::Kind::DECIMAL,
          Type::Kind::DECIMAL,
      } },

};

static const auto compare_builtin_resolve = arithmetic_builtin_resolve;

static const auto compare_builtin_inference = []( const std::vector< Type::Ptr >& types,
                                                  const std::vector< Value::Ptr >& ) -> Type::Ptr {
    if( types.size() != 2 )
    {
        throw InternalException( "types.size() != 2" );
    }
    const auto& lhs = types[ 0 ];
    const auto& rhs = types[ 1 ];
    if( *lhs == *rhs )
    {
        return BOOLEAN;
    }
    else
    {
        return nullptr;
    }
};

static const auto compare_builtin_validate = []( const RelationType& type ) -> u1 {
    if( type.arguments().size() == 2 )
    {
        return type.result().isBoolean() and *type.arguments()[ 0 ] == *type.arguments()[ 1 ];
    }
    else
    {
        return false;
    }
};

u1 CompareBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or LesuBuiltin::classof( obj ) or LessBuiltin::classof( obj ) or
           LequBuiltin::classof( obj ) or LeqsBuiltin::classof( obj ) or
           GreuBuiltin::classof( obj ) or GresBuiltin::classof( obj ) or
           GequBuiltin::classof( obj ) or GeqsBuiltin::classof( obj );
}

//
// LesuBuiltin
//

LesuBuiltin::LesuBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

void LesuBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation LesuBuiltin::annotation(
    classid(),
    compare_builtin_properties,
    compare_builtin_data,
    compare_builtin_resolve,
    compare_builtin_inference,
    compare_builtin_validate );

u1 LesuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LessBuiltin
//

LessBuiltin::LessBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

void LessBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation LessBuiltin::annotation(
    classid(),
    compare_builtin_properties,
    compare_builtin_data,
    compare_builtin_resolve,
    compare_builtin_inference,
    compare_builtin_validate );

u1 LessBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LequBuiltin
//

LequBuiltin::LequBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

void LequBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation LequBuiltin::annotation(
    classid(),
    compare_builtin_properties,
    compare_builtin_data,
    compare_builtin_resolve,
    compare_builtin_inference,
    compare_builtin_validate );

u1 LequBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LeqsBuiltin
//

LeqsBuiltin::LeqsBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

void LeqsBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation LeqsBuiltin::annotation(
    classid(),
    compare_builtin_properties,
    compare_builtin_data,
    compare_builtin_resolve,
    compare_builtin_inference,
    compare_builtin_validate );

u1 LeqsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GreuBuiltin
//

GreuBuiltin::GreuBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

void GreuBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation GreuBuiltin::annotation(
    classid(),
    compare_builtin_properties,
    compare_builtin_data,
    compare_builtin_resolve,
    compare_builtin_inference,
    compare_builtin_validate );

u1 GreuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GresBuiltin
//

GresBuiltin::GresBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

void GresBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation GresBuiltin::annotation(
    classid(),
    compare_builtin_properties,
    compare_builtin_data,
    compare_builtin_resolve,
    compare_builtin_inference,
    compare_builtin_validate );

u1 GresBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GequBuiltin
//

GequBuiltin::GequBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

void GequBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation GequBuiltin::annotation(
    classid(),
    compare_builtin_properties,
    compare_builtin_data,
    compare_builtin_resolve,
    compare_builtin_inference,
    compare_builtin_validate );

u1 GequBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GeqsBuiltin
//

GeqsBuiltin::GeqsBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

void GeqsBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation GeqsBuiltin::annotation(
    classid(),
    compare_builtin_properties,
    compare_builtin_data,
    compare_builtin_resolve,
    compare_builtin_inference,
    compare_builtin_validate );

u1 GeqsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// BinaryBuiltin
//

BinaryBuiltin::BinaryBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 BinaryBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or ZextBuiltin::classof( obj ) or SextBuiltin::classof( obj ) or
           TruncBuiltin::classof( obj ) or ShlBuiltin::classof( obj ) or
           ShrBuiltin::classof( obj ) or AshrBuiltin::classof( obj ) or
           ClzBuiltin::classof( obj ) or CloBuiltin::classof( obj ) or ClsBuiltin::classof( obj );
}

static const Properties binary_builtin_properties = { Property::SIDE_EFFECT_FREE, Property::PURE };

//
// ZextBuiltin
//

ZextBuiltin::ZextBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void ZextBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& valueConstant = reg[ 0 ];

    assert( type().result().isBinary() );
    const auto resultType = std::static_pointer_cast< BinaryType >( type().result().ptr_type() );

    if( not valueConstant.defined() )
    {
        res = BinaryConstant( resultType );
    }
    else
    {
        const auto& value = static_cast< const BinaryConstant& >( valueConstant ).value();
        const auto zextValue = libstdhl::Type::createNatural( value );
        res = BinaryConstant( resultType, zextValue );
    }
}

const Annotation ZextBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::INTEGER,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 2 or values.size() != 2 )
        {
            throw InternalException( "types.size() != 2 or values.size() != 2" );
        }

        if( isa< IntegerConstant >( values[ 1 ] ) )
        {
            auto c = std::static_pointer_cast< IntegerConstant >( values[ 1 ] );
            return libstdhl::Memory::get< BinaryType >( c );
        }
        else
        {
            return nullptr;
        }
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 2 )
        {
            if( type.result().isBinary() and type.arguments()[ 0 ]->isBinary() and
                type.arguments()[ 1 ]->isInteger() )
            {
                const auto& resType = static_cast< const BinaryType& >( type.result() );
                const auto& argType = static_cast< const BinaryType& >( *type.arguments()[ 0 ] );

                if( resType.bitsize() > argType.bitsize() )
                {
                    return true;
                }
            }
        }

        return false;
    } );

u1 ZextBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SextBuiltin
//

SextBuiltin::SextBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void SextBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& valueConstant = reg[ 0 ];

    assert( type().result().isBinary() );
    const auto resultType = std::static_pointer_cast< BinaryType >( type().result().ptr_type() );

    if( not valueConstant.defined() )
    {
        res = BinaryConstant( resultType );
    }
    else
    {
        const auto& value = static_cast< const BinaryConstant& >( valueConstant );
        assert( value.type().isBinary() );
        const auto& valueType = static_cast< const BinaryType& >( value.type() );

        const auto sign = value.value() >> ( valueType.bitsize() - 1 );

        Operation::execute< ZextBuiltin >( type().ptr_type(), res, reg, size );

        if( sign != 0 )
        {
            auto mask = Utility::createMask( resultType->bitsize() - valueType.bitsize() );
            mask <<= valueType.bitsize();

            auto tmp = BinaryConstant( resultType, mask );
            Operation::execute< OrInstruction >( resultType, res, res, tmp );
        }
    }
}

const Annotation SextBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::INTEGER,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 2 or values.size() != 2 )
        {
            throw InternalException( "types.size() != 2 or values.size() != 2" );
        }

        if( isa< IntegerConstant >( values[ 1 ] ) )
        {
            auto c = std::static_pointer_cast< IntegerConstant >( values[ 1 ] );
            return libstdhl::Memory::get< BinaryType >( c );
        }
        else
        {
            return nullptr;
        }
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 2 )
        {
            if( type.result().isBinary() and type.arguments()[ 0 ]->isBinary() and
                type.arguments()[ 1 ]->isInteger() )
            {
                const auto& resType = static_cast< const BinaryType& >( type.result() );
                const auto& argType = static_cast< const BinaryType& >( *type.arguments()[ 0 ] );

                if( resType.bitsize() > argType.bitsize() )
                {
                    return true;
                }
            }
        }

        return false;
    } );

u1 SextBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// TruncBuiltin
//

TruncBuiltin::TruncBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void TruncBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& valueConstant = reg[ 0 ];
    const auto& offsetConstant = reg[ 1 ];
    assert( offsetConstant.defined() );
    assert( offsetConstant.typeId().kind() == Type::Kind::INTEGER );

    assert( type().result().isBinary() );
    const auto resultType = std::static_pointer_cast< BinaryType >( type().result().ptr_type() );

    if( not valueConstant.defined() )
    {
        res = BinaryConstant( resultType );
    }
    else
    {
        const auto& offset = static_cast< const IntegerConstant& >( offsetConstant ).value();

        auto mask = libstdhl::Type::createNatural( 1 );
        mask <<= libstdhl::Type::createNatural( offset );
        mask -= 1;

        auto tmp = BinaryConstant( valueConstant.type().ptr_type(), mask );
        Operation::execute< AndInstruction >(
            valueConstant.type().ptr_type(), tmp, tmp, valueConstant );
        assert( tmp.defined() );
        assert( tmp.typeId().kind() == Type::Kind::BINARY );

        const auto& value = static_cast< const BinaryConstant& >( tmp ).value();
        res = BinaryConstant( resultType, value );
    }
}

const Annotation TruncBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::INTEGER,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 2 or values.size() != 2 )
        {
            throw InternalException( "types.size() != 2 or values.size() != 2" );
        }

        if( isa< IntegerConstant >( values[ 1 ] ) )
        {
            auto c = std::static_pointer_cast< IntegerConstant >( values[ 1 ] );
            return libstdhl::Memory::get< BinaryType >( c );
        }
        else
        {
            return nullptr;
        }
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 2 )
        {
            if( type.result().isBinary() and type.arguments()[ 0 ]->isBinary() and
                type.arguments()[ 1 ]->isInteger() )
            {
                const auto& resType = static_cast< const BinaryType& >( type.result() );
                const auto& argType = static_cast< const BinaryType& >( *type.arguments()[ 0 ] );

                if( resType.bitsize() < argType.bitsize() )
                {
                    return true;
                }
            }
        }

        return false;
    } );

u1 TruncBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// ShlBuiltin
//

ShlBuiltin::ShlBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void ShlBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& valueConstant = reg[ 0 ];
    const auto& offsetConstant = reg[ 1 ];

    if( not valueConstant.defined() or not offsetConstant.defined() )
    {
        // offset is undef, return the given value
        res = valueConstant;
    }
    else
    {
        assert( type().result().isBinary() );
        const auto& value = static_cast< const BinaryConstant& >( valueConstant ).value();

        switch( offsetConstant.typeId().kind() )
        {
            case Type::Kind::INTEGER:
            {
                const auto& offset =
                    static_cast< const IntegerConstant& >( offsetConstant ).value();

                const auto shiftedValue = libstdhl::Type::createNatural( value << offset );
                res = BinaryConstant( valueConstant.type().ptr_type(), shiftedValue );
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& offset = static_cast< const BinaryConstant& >( offsetConstant ).value();

                const auto shiftedValue = libstdhl::Type::createNatural( value << offset );
                res = BinaryConstant( valueConstant.type().ptr_type(), shiftedValue );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
}

const Annotation ShlBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }
        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];
        if( rhs->isBinary() )
        {
            if( *lhs != *rhs )
            {
                return nullptr;
            }
        }
        return lhs;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 2 )
        {
            if( type.arguments()[ 1 ]->isInteger() )
            {
                return type.result() == *type.arguments()[ 0 ];
            }
            else
            {
                return type.result() == *type.arguments()[ 0 ] and
                       type.result() == *type.arguments()[ 1 ];
            }
        }
        else
        {
            return false;
        }
    } );

u1 ShlBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// ShrBuiltin
//

ShrBuiltin::ShrBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void ShrBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    const auto& valueConstant = reg[ 0 ];
    const auto& offsetConstant = reg[ 1 ];

    if( not valueConstant.defined() or not offsetConstant.defined() )
    {
        // offset is undef, return the given value
        res = valueConstant;
    }
    else
    {
        assert( type().result().isBinary() );
        const auto& value = static_cast< const BinaryConstant& >( valueConstant ).value();

        switch( offsetConstant.typeId().kind() )
        {
            case Type::Kind::INTEGER:
            {
                const auto& offset =
                    static_cast< const IntegerConstant& >( offsetConstant ).value();

                const auto shiftedValue = libstdhl::Type::createNatural( value >> offset );
                res = BinaryConstant( valueConstant.type().ptr_type(), shiftedValue );
                break;
            }
            case Type::Kind::BINARY:
            {
                const auto& offset = static_cast< const BinaryConstant& >( offsetConstant ).value();

                const auto shiftedValue = libstdhl::Type::createNatural( value >> offset );
                res = BinaryConstant( valueConstant.type().ptr_type(), shiftedValue );
                break;
            }
            default:
            {
                throw InternalException( "unimplemented '" + description() + "'" );
            }
        }
    }
}

const Annotation ShrBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }
        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];
        if( rhs->isBinary() )
        {
            if( *lhs != *rhs )
            {
                return nullptr;
            }
        }
        return lhs;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 2 )
        {
            if( type.arguments()[ 1 ]->isInteger() )
            {
                return type.result() == *type.arguments()[ 0 ];
            }
            else
            {
                return type.result() == *type.arguments()[ 0 ] and
                       type.result() == *type.arguments()[ 1 ];
            }
        }
        else
        {
            return false;
        }
    } );

u1 ShrBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AshrBuiltin
//

AshrBuiltin::AshrBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void AshrBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation AshrBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::INTEGER,
          } },

        { Type::Kind::BINARY,
          {
              Type::Kind::BINARY,
              Type::Kind::BINARY,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 2 )
        {
            throw InternalException( "types.size() != 2" );
        }
        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];
        if( rhs->isBinary() )
        {
            if( *lhs != *rhs )
            {
                return nullptr;
            }
        }
        return lhs;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 2 )
        {
            if( type.arguments()[ 1 ]->isInteger() )
            {
                return type.result() == *type.arguments()[ 0 ];
            }
            else
            {
                return type.result() == *type.arguments()[ 0 ] and
                       type.result() == *type.arguments()[ 1 ];
            }
        }
        else
        {
            return false;
        }
    } );

u1 AshrBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// ClzBuiltin
//

ClzBuiltin::ClzBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void ClzBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation ClzBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::BINARY,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return INTEGER;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isInteger() and type.arguments()[ 0 ]->isBinary();
        }
        else
        {
            return false;
        }
    } );

u1 ClzBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// CloBuiltin
//

CloBuiltin::CloBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void CloBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation CloBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::BINARY,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return INTEGER;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isInteger() and type.arguments()[ 0 ]->isBinary();
        }
        else
        {
            return false;
        }
    } );

u1 CloBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// ClsBuiltin
//

ClsBuiltin::ClsBuiltin( const Type::Ptr& type )
: BinaryBuiltin( type, classid() )
{
}

void ClsBuiltin::execute( Constant& res, const Constant* reg, const std::size_t size ) const
{
    // TODO: FIXME: @ppaulweber
    throw InternalException( "unimplemented '" + description() + "'" );
}

const Annotation ClsBuiltin::annotation(
    classid(),
    binary_builtin_properties,
    Annotation::Relations{

        { Type::Kind::INTEGER,
          {
              Type::Kind::BINARY,
          } }

    },
    []( std::vector< Type::Ptr >& types ) {},
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        if( types.size() != 1 )
        {
            throw InternalException( "types.size() != 1" );
        }
        return INTEGER;
    },
    []( const RelationType& type ) -> u1 {
        if( type.arguments().size() == 1 )
        {
            return type.result().isInteger() and type.arguments()[ 0 ]->isBinary();
        }
        else
        {
            return false;
        }
    } );

u1 ClsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

// Math built-ins:

// pow  : Integer * Integer -> Integer
// //     base      exponent
// rand : Integer * Integer -> Integer
// //     start     end

// List built-ins:

//             NTH_BUILTIN,
//             CONS_BUILTIN,
//             APP_BUILTIN,
//             LEN_BUILTIN,
//             TAIL_BUILTIN,
//             PEEK_BUILTIN,

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

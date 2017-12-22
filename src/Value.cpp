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

#include "Value.h"

#include "Agent.h"
#include "Block.h"
#include "Builtin.h"
#include "Constant.h"
#include "Derived.h"
#include "Function.h"
#include "Instruction.h"
#include "Rule.h"
#include "Specification.h"
#include "Statement.h"
#include "Value.h"
#include "Visitor.h"

using namespace libcasm_ir;

Value::Value( const Type::Ptr& type, const ID id )
: m_type( type->id() )
, m_id( id )
{
}

Value::Value( const Type::ID type, const ID id )
: m_type( type )
, m_id( id )
{
    assert( type.flavor() != 0 );
}

std::string Value::description( void ) const
{
    return type().name() + " " + name();
}

const Type& Value::type( void ) const
{
    return *Type::fromID( m_type );
}

Type::ID Value::typeId( void ) const
{
    return m_type;
}

Value::ID Value::id( void ) const
{
    return m_id;
}

std::string Value::dump( void ) const
{
    std::string tmp = "[" + type().name() + "] ";

    if( not type().isVoid() )
    {
        tmp += label() + " = ";
    }

    if( isa< Constant >( this ) or isa< Builtin >( this ) or isa< Function >( this ) )
    {
        tmp += type().name() + " ";
    }

    if( not isa< Function >( this ) )
    {
        tmp += name();
    }

    if( auto instr = cast< Instruction >( this ) )
    {
        if( isa< ForkInstruction >( this ) or isa< MergeInstruction >( this ) )
        {
            tmp += " " + instr->statement()->scope()->name();
        }

        u1 first = true;
        for( auto operand : instr->operands() )
        {
            if( first )
            {
                first = false;
                tmp += " ";
            }
            else
            {
                tmp += ", ";
            }

            tmp += operand->type().name() + " " + operand->label();
        }
    }

    return tmp;
}

std::string Value::label( void ) const
{
    static std::unordered_map< u8, u64 > cnt;
    static std::unordered_map< const Value*, std::string > lbl;

    auto result = lbl.find( this );
    if( result != lbl.end() )
    {
        return result->second;
    }

    if( isa< Instruction >( this ) )
    {
        auto result = cnt.find( INSTRUCTION );
        if( result == cnt.end() )
        {
            cnt[ INSTRUCTION ] = 0;
        }

        if( this->type().result().isVoid() )
        {
            return name();
        }

        return lbl.emplace( this, "%r" + std::to_string( cnt[ INSTRUCTION ]++ ) ).first->second;
    }
    else if( isa< Block >( this ) )
    {
        auto result = cnt.find( BLOCK );
        if( result == cnt.end() )
        {
            cnt[ BLOCK ] = 0;
        }

        return lbl.emplace( this, "%lbl" + std::to_string( cnt[ BLOCK ]++ ) ).first->second;
    }
    else if( isa< Constant >( this ) )
    {
        auto result = cnt.find( CONSTANT );
        if( result == cnt.end() )
        {
            cnt[ CONSTANT ] = 0;
        }

        return lbl.emplace( this, "@c" + std::to_string( cnt[ CONSTANT ]++ ) ).first->second;
    }
    else if( isa< Builtin >( this ) )
    {
        auto result = cnt.find( BUILTIN );
        if( result == cnt.end() )
        {
            cnt[ BUILTIN ] = 0;
        }

        return lbl.emplace( this, "@b" + std::to_string( cnt[ BUILTIN ]++ ) ).first->second;
    }
    else
    {
        return "@" + name();
    }
}

u1 Value::operator==( const Value& rhs ) const
{
    return ( this->id() == rhs.id() ) and ( this->type() == rhs.type() );
}

void Value::iterate( const Traversal order, std::function< void( Value& ) > action )
{
    TraversalVisitor visitor( order, action );

    accept( visitor );
}

std::string Value::token( const Value::ID id )
{
    switch( id )
    {
        case Value::VALUE:
        {
            return "Value";
        }
        case Value::VALUE_LIST:
        {
            return "ValueList";
        }
        case Value::USER:
        {
            return "User";
        }

        case Value::SPECIFICATION:
        {
            return "Specification";
        }
        case Value::AGENT:
        {
            return "Agent";
        }
        case Value::RULE:
        {
            return "Rule";
        }
        case Value::DERIVED:
        {
            return "Derived";
        }
        case Value::FUNCTION:
        {
            return "Function";
        }
        case Value::ENUMERATION:
        {
            return "Enumeration";
        }
        case Value::RANGE:
        {
            return "Range";
        }
        case Value::TUPLE:
        {
            return "Tuple";
        }
        case Value::LIST:
        {
            return "List";
        }

        case Value::BLOCK:
        {
            return "Block";
        }
        case Value::EXECUTION_SEMANTICS_BLOCK:
        {
            return "ExecutionSemanticsBlock";
        }
        case Value::PARALLEL_BLOCK:
        {
            return "ParallelBlock";
        }
        case Value::SEQUENTIAL_BLOCK:
        {
            return "SequentialBlock";
        }
        case Value::STATEMENT:
        {
            return "Statement";
        }
        case Value::TRIVIAL_STATEMENT:
        {
            return "TrivialStatement";
        }
        case Value::BRANCH_STATEMENT:
        {
            return "BranchStatement";
        }

        case Value::CONSTANT:
        {
            return "Constant";
        }
        case Value::VOID_CONSTANT:
        {
            return "VoidConstant";
        }
        case Value::RULE_REFERENCE_CONSTANT:
        {
            return "RuleReferenceConstant";
        }
        case Value::FUNCTION_REFERENCE_CONSTANT:
        {
            return "FunctionReferenceConstant";
        }
        case Value::BOOLEAN_CONSTANT:
        {
            return "BooleanConstant";
        }
        case Value::INTEGER_CONSTANT:
        {
            return "IntegerConstant";
        }
        case Value::BINARY_CONSTANT:
        {
            return "BinaryConstant";
        }
        case Value::STRING_CONSTANT:
        {
            return "StringConstant";
        }
        case Value::DECIMAL_CONSTANT:
        {
            return "DecimalConstant";
        }
        case Value::RATIONAL_CONSTANT:
        {
            return "RationalConstant";
        }
        case Value::ENUMERATION_CONSTANT:
        {
            return "EnumerationConstant";
        }
        case Value::RANGE_CONSTANT:
        {
            return "RangeConstant";
        }
        case Value::TUPLE_CONSTANT:
        {
            return "TupleConstant";
        }
        case Value::LIST_CONSTANT:
        {
            return "ListConstant";
        }
        case Value::IDENTIFIER:
        {
            return "Identifier";
        }

        case Value::INSTRUCTION:
        {
            return "Instruction";
        }
        case Value::UNARY_INSTRUCTION:
        {
            return "UnaryInstruction";
        }
        case Value::BINARY_INSTRUCTION:
        {
            return "BinaryInstruction";
        }

        case Value::SELECT_INSTRUCTION:
        {
            return "select";
        }
        case Value::SKIP_INSTRUCTION:
        {
            return "skip";
        }
        case Value::FORK_INSTRUCTION:
        {
            return "fork";
        }
        case Value::MERGE_INSTRUCTION:
        {
            return "merge";
        }
        case Value::LOOKUP_INSTRUCTION:
        {
            return "lookup";
        }
        case Value::UPDATE_INSTRUCTION:
        {
            return "update";
        }
        case Value::LOCATION_INSTRUCTION:
        {
            return "location";
        }
        case Value::CALL_INSTRUCTION:
        {
            return "call";
        }
        case Value::LOCAL_INSTRUCTION:
        {
            return "local";
        }

        case Value::OPERATOR_INSTRUCTION:
        {
            return "OperatorInstruction";
        }

        case Value::ARITHMETIC_INSTRUCTION:
        {
            return "ArithmethicInstruction";
        }
        case Value::INV_INSTRUCTION:
        {
            return " -";
        }
        case Value::ADD_INSTRUCTION:
        {
            return "+";
        }
        case Value::SUB_INSTRUCTION:
        {
            return "-";
        }
        case Value::MUL_INSTRUCTION:
        {
            return "*";
        }
        case Value::DIV_INSTRUCTION:
        {
            return "/";
        }
        case Value::POW_INSTRUCTION:
        {
            return "^";
        }
        case Value::MOD_INSTRUCTION:
        {
            return "%";
        }

        case Value::COMPARE_INSTRUCTION:
        {
            return "CompareInstruction";
        }
        case Value::EQU_INSTRUCTION:
        {
            return "=";
        }
        case Value::NEQ_INSTRUCTION:
        {
            return "!=";
        }
        case Value::LTH_INSTRUCTION:
        {
            return "<";
        }
        case Value::LEQ_INSTRUCTION:
        {
            return "<=";
        }
        case Value::GTH_INSTRUCTION:
        {
            return ">";
        }
        case Value::GEQ_INSTRUCTION:
        {
            return ">=";
        }

        case Value::LOGICAL_INSTRUCTION:
        {
            return "LogicalInstruction";
        }
        case Value::OR_INSTRUCTION:
        {
            return "or";
        }
        case Value::XOR_INSTRUCTION:
        {
            return "xor";
        }
        case Value::AND_INSTRUCTION:
        {
            return "and";
        }
        case Value::IMP_INSTRUCTION:
        {
            return "imp";
        }
        case Value::NOT_INSTRUCTION:
        {
            return "not";
        }

        case Value::BUILTIN:
        {
            return "Builtin";
        }
        case Value::GENERAL_BUILTIN:
        {
            return "GeneralBuiltin";
        }
        case Value::IS_SYMBOLIC_BUILTIN:
        {
            return "isSymbolic";
        }
        case Value::ABORT_BUILTIN:
        {
            return "abort";
        }
        case Value::ASSERT_BUILTIN:
        {
            return "assert";
        }
        case Value::ASSURE_BUILTIN:
        {
            return "assure";
        }

        case Value::OUTPUT_BUILTIN:
        {
            return "OutputBuiltin";
        }
        case Value::PRINT_BUILTIN:
        {
            return "print";
        }
        case Value::PRINTLN_BUILTIN:
        {
            return "println";
        }

        case Value::CASTING_BUILTIN:
        {
            return "CastingBuiltin";
        }
        case Value::AS_BOOLEAN_BUILTIN:
        {
            return "asBoolean";
        }
        case Value::AS_INTEGER_BUILTIN:
        {
            return "asInteger";
        }
        case Value::AS_BINARY_BUILTIN:
        {
            return "asBinary";
        }
        case Value::AS_STRING_BUILTIN:
        {
            return "asString";
        }
        case Value::AS_DECIMAL_BUILTIN:
        {
            return "asDecimal";
        }
        case Value::AS_RATIONAL_BUILTIN:
        {
            return "asRational";
        }
        case Value::AS_ENUMERATION_BUILTIN:
        {
            return "as<Enumeration>";
        }

        case Value::STRINGIFY_BUILTIN:
        {
            return "StringifyBuiltin";
        }
        case Value::DEC_BUILTIN:
        {
            return "dec";
        }
        case Value::HEX_BUILTIN:
        {
            return "hex";
        }
        case Value::OCT_BUILTIN:
        {
            return "oct";
        }
        case Value::BIN_BUILTIN:
        {
            return "bin";
        }

        case Value::OPERATOR_BUILTIN:
        {
            return "OperatorBuiltin";
        }
        case Value::ARITHMETIC_BUILTIN:
        {
            return "ArithmethicBuiltin";
        }
        case Value::ADDU_BUILTIN:
        {
            return "addu";
        }
        case Value::ADDS_BUILTIN:
        {
            return "adds";
        }
        case Value::SUBU_BUILTIN:
        {
            return "subu";
        }
        case Value::SUBS_BUILTIN:
        {
            return "subs";
        }
        case Value::MULU_BUILTIN:
        {
            return "mulu";
        }
        case Value::MULS_BUILTIN:
        {
            return "muls";
        }

        case Value::COMPARE_BUILTIN:
        {
            return "CompareBuiltin";
        }
        case Value::LESU_BUILTIN:
        {
            return "lesu";
        }
        case Value::LESS_BUILTIN:
        {
            return "less";
        }
        case Value::LEQU_BUILTIN:
        {
            return "lequ";
        }
        case Value::LEQS_BUILTIN:
        {
            return "leqs";
        }
        case Value::GREU_BUILTIN:
        {
            return "greu";
        }
        case Value::GRES_BUILTIN:
        {
            return "gres";
        }
        case Value::GEQU_BUILTIN:
        {
            return "gequ";
        }
        case Value::GEQS_BUILTIN:
        {
            return "geqs";
        }

        case Value::BINARY_BUILTIN:
        {
            return "BinaryBuiltin";
        }
        case Value::ZEXT_BUILTIN:
        {
            return "zext";
        }
        case Value::SEXT_BUILTIN:
        {
            return "sext";
        }
        case Value::TRUNC_BUILTIN:
        {
            return "trunc";
        }
        case Value::SHL_BUILTIN:
        {
            return "shl";
        }
        case Value::SHR_BUILTIN:
        {
            return "shr";
        }
        case Value::ASHR_BUILTIN:
        {
            return "ashr";
        }
        case Value::CLZ_BUILTIN:
        {
            return "clz";
        }
        case Value::CLO_BUILTIN:
        {
            return "clo";
        }
        case Value::CLS_BUILTIN:
        {
            return "cls";
        }

            // case Value::MATH_BUILTIN:
            // {
            //     return "MathBuiltin";
            // }
            // case Value::POW_BUILTIN:
            // {
            //     return "pow";
            // }
            // case Value::RAND_BUILTIN:
            // {
            //     return "rand";
            // }

        case Value::_SIZE_:
        {
            return "_SIZE_ = " + std::to_string( Value::_SIZE_ );
        }
    }

    assert( !" internal error " );
    return "";
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

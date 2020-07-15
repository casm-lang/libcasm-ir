//
//  Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
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

#include "SymbolicExecutionEnvironment.h"

#include <initializer_list>
#include <libcasm-ir/Constant>
#include <libcasm-ir/Exception>
#include <libcasm-ir/Instruction>

#include <libtptp/Definition>
#include <libtptp/Type>

#include <memory>
#include <sstream>

using namespace libcasm_ir;

bool SymbolicExecutionEnvironment::Location::Comperator::operator()(
    const Location& lhs, const Location& rhs )
{
    return lhs.varName == rhs.varName ? this->operator()( lhs.arguments, rhs.arguments )
                                      : lhs.varName < rhs.varName;
}

bool SymbolicExecutionEnvironment::Location::Comperator::operator()(
    const Constant& lhs, const Constant& rhs )
{
    return libstdhl::Hash::value( lhs ) < libstdhl::Hash::value( rhs );
}

SymbolicExecutionEnvironment::ScopedEnvironment::ScopedEnvironment(
    SymbolicExecutionEnvironment* environment, const TPTP::Logic::Ptr& logic )
: m_environment( environment )
{
    m_environment->m_environments.push_back( logic );
}

SymbolicExecutionEnvironment::ScopedEnvironment::~ScopedEnvironment( void )
{
    if( m_environment != nullptr )
    {
        m_environment->m_environments.pop_back();
    }
}

void SymbolicExecutionEnvironment::ScopedEnvironment::invalidate( void )
{
    m_environment = nullptr;
}

SymbolicExecutionEnvironment::SymbolicExecutionEnvironment( void )
: m_symbolName( 0 )
, m_formulaName( 0 )
, m_time( 1 )
{
}

SymbolicExecutionEnvironment::~SymbolicExecutionEnvironment( void )
{
    for( auto env : m_scoped_environments )
    {
        if( auto env_ptr = env.lock() )
        {
            env_ptr->invalidate();
        }
    }
}

std::string SymbolicExecutionEnvironment::generateSymbolName( void )
{
    std::stringstream stream;
    stream << "'%" << m_symbolName++ << "'";
    return stream.str();
}

std::string SymbolicExecutionEnvironment::generateFormulaName( void )
{
    std::stringstream stream;
    stream << m_formulaName++;
    return stream.str();
}

std::string SymbolicExecutionEnvironment::generateOperatorFunction( const Value& value )
{
    std::stringstream stream;
    stream << "'#" << Value::token( value.id() ) << "#" << value.type().name() << "'";
    auto functionName = stream.str();
    if( m_functionDeclarations.find( functionName ) == m_functionDeclarations.end() )
    {
        auto args = std::make_shared< TPTP::ListTypeElements< TPTP::TokenBuilder::STAR > >();

        // TODO: @moosbruggerj remove dependency on isa conditions, use relation type arguments
        /*
        for( auto& type : value.type().arguments() )
        {
            args->add( getTPTPType( *type ) );
        }
        */
        int numArgs = 0;
        if( isa< UnaryInstruction >( value ) )
        {
            numArgs = 1;
        }
        else if( isa< BinaryInstruction >( value ) )
        {
            numArgs = 2;
        }

        for( int i = 0; i < numArgs; ++i )
        {
            args->add( getTPTPType( value.type() ) );
        }
        args->add( getTPTPType( value.type().result() ) );

        auto boolean = std::make_shared< TPTP::NamedType >( "$o" );
        auto typeArgs = std::make_shared< TPTP::RelationType >( args );
        auto mapping = std::make_shared< TPTP::BinaryType >(
            typeArgs, boolean, TPTP::BinaryType::Kind::MAPPING );
        auto decleration = std::make_shared< TPTP::TypeAtom >( functionName, mapping );
        auto formula = std::make_shared< TPTP::TypedFirstOrderFormula >( decleration );
        auto formulaName = generateFormulaName();
        auto definition = std::make_shared< TPTP::FormulaDefinition >(
            formulaName, TPTP::Role::hypothesis(), formula );
        m_functionDeclarations.emplace( functionName, definition );

        // generateFunctionDefinition( value, formulaName );
    }
    return functionName;
}

SymbolicConstant SymbolicExecutionEnvironment::get(
    const std::string& constant,
    const Type::Ptr& functionType,
    const std::vector< Constant >& arguments )
{
    auto symName = generateSymbolName();
    auto symConst = SymbolicConstant( functionType->ptr_result(), symName, *this );

    auto time_it = m_symbolSetTimes.find( { constant, functionType->ptr_result(), arguments } );
    int time;
    if( time_it == m_symbolSetTimes.end() )
    {
        m_symbolSetTimes[ { constant, functionType->ptr_result(), arguments } ] = 1;
        time = 1;
    }
    else
    {
        time = time_it->second;
    }

    setAtTime(
        constant,
        arguments,
        std::make_shared< TPTP::ConstantAtom >( symName, TPTP::Atom::Kind::PLAIN ),
        time );
    return symConst;
}

void SymbolicExecutionEnvironment::set(
    const std::string& varName,
    const Type::Ptr& functionType,
    const std::vector< Constant >& arguments,
    const std::string& symName )
{
    setAtTime(
        varName,
        arguments,
        std::make_shared< TPTP::ConstantAtom >( symName, TPTP::Atom::Kind::PLAIN ),
        m_time );
    m_symbolUpdateSet[ { varName, functionType->ptr_result(), arguments } ] = m_time;
}

void SymbolicExecutionEnvironment::set(
    const std::string& varName,
    const Type::Ptr& functionType,
    const std::vector< Constant >& arguments,
    const TPTP::Literal::Ptr& literal )
{
    setAtTime( varName, arguments, std::make_shared< TPTP::DefinedAtom >( literal ), m_time );
    m_symbolUpdateSet[ { varName, functionType->ptr_result(), arguments } ] = m_time;
}

void SymbolicExecutionEnvironment::set(
    const std::string& varName,
    const Type::Ptr& functionType,
    const std::vector< Constant >& arguments,
    const TPTP::Atom::Ptr& atom )
{
    setAtTime( varName, arguments, atom, m_time );
    m_symbolUpdateSet[ { varName, functionType->ptr_result(), arguments } ] = m_time;
}

void SymbolicExecutionEnvironment::addFormula( const TPTP::Logic::Ptr& logic )
{
    TPTP::Logic::Ptr fLogic = logic;
    for( auto it = m_environments.rbegin(); it != m_environments.rend(); ++it )
    {
        fLogic->setLeftDelimiter( TPTP::TokenBuilder::LPAREN() );
        fLogic->setRightDelimiter( TPTP::TokenBuilder::RPAREN() );
        const auto implication = std::make_shared< TPTP::BinaryLogic >(
            *it, TPTP::BinaryLogic::Connective::IMPLICATION, fLogic );
        fLogic = implication;
    }
    auto formula = std::make_shared< TPTP::TypedFirstOrderFormula >( fLogic );
    auto definition = std::make_shared< TPTP::FormulaDefinition >(
        generateFormulaName(), TPTP::Role::hypothesis(), formula );

    m_formulae.push_back( definition );
}

void SymbolicExecutionEnvironment::addFunctionDeclaration(
    const std::string& name, const Type& type )
{
    auto args = std::make_shared< TPTP::ListTypeElements< TPTP::TokenBuilder::STAR > >();
    args->add( std::make_shared< TPTP::NamedType >( "$int" ) );

    for( auto& arg : type.arguments() )
    {
        args->add( getTPTPType( *arg ) );
    }
    args->add( getTPTPType( type.result() ) );
    auto fName = storeFunctionFromName( name );
    auto functionArgs = std::make_shared< TPTP::RelationType >( args );
    auto mapping = std::make_shared< TPTP::BinaryType >(
        functionArgs,
        std::make_shared< TPTP::NamedType >( "$o" ),
        TPTP::BinaryType::Kind::MAPPING );
    auto atom = std::make_shared< TPTP::TypeAtom >( fName, mapping );

    auto formula = std::make_shared< TPTP::TypedFirstOrderFormula >( atom );
    auto definition = std::make_shared< TPTP::FormulaDefinition >(
        generateFormulaName(), TPTP::Role::hypothesis(), formula );

    m_functions.push_back( definition );
}

void SymbolicExecutionEnvironment::addSymbolDefinition( const TPTP::Logic::Ptr& logic )
{
    auto formula = std::make_shared< TPTP::TypedFirstOrderFormula >( logic );
    auto formulaDef = std::make_shared< TPTP::FormulaDefinition >(
        generateFormulaName(), TPTP::Role::type(), formula );
    m_symbolDefinitions.push_back( formulaDef );
}

TPTP::Specification::Ptr SymbolicExecutionEnvironment::finalize( void )
{
    for( auto& loc : m_symbolSetTimes )
    {
        auto sym = get( loc.first.varName, loc.first.type, loc.first.arguments );

        // set at 0
        setAtTime(
            loc.first.varName,
            loc.first.arguments,
            std::make_shared< TPTP::ConstantAtom >( sym.name(), TPTP::Atom::Kind::PLAIN ),
            0 );
    }

    m_symbolSetTimes.clear();

    auto spec = std::make_shared< TPTP::Specification >();
    for( auto& def : m_symbolDefinitions )
    {
        spec->add( def );
    }
    for( auto& def : m_functionDeclarations )
    {
        spec->add( def.second );
    }
    for( auto& def : m_functionDefinitons )
    {
        spec->add( def );
    }
    for( const auto& def : m_functions )
    {
        spec->add( def );
    }
    for( auto& def : m_formulae )
    {
        spec->add( def );
    }
    return spec;
}

void SymbolicExecutionEnvironment::incrementTime( void )
{
    for( auto& update : m_symbolUpdateSet )
    {
        m_symbolSetTimes[ update.first ] = update.second;
    }
    ++m_time;
}

SymbolicExecutionEnvironment::ScopedEnvironment::Ptr SymbolicExecutionEnvironment::makeEnvironment(
    const TPTP::Logic::Ptr& logic )
{
    const auto env = std::make_shared< ScopedEnvironment >( this, logic );
    // m_scoped_environments fills with time, if memory problems occur, clear invalid pointers
    // possible refactor: SymbolicExecutionEnvironment (this) needs to be shared_ptr ->
    // ScopedEnvironments has weak_ptr reference to SymbolicExecutionEnvironment -> no need for
    // m_scoped_environments
    m_scoped_environments.push_back( env );
    return env;
}

const TPTP::Type::Ptr SymbolicExecutionEnvironment::getTPTPType( const Type& type ) const
{
    switch( type.kind() )
    {
        case Type::Kind::VOID:
        case Type::Kind::LABEL:
        case Type::Kind::LOCATION:
        case Type::Kind::RELATION:
        {
            // assert
        }

        // primitive
        case Type::Kind::BOOLEAN:
        {
            return std::make_shared< TPTP::NamedType >( "$o" );
        }
        case Type::Kind::INTEGER:
        {
            return std::make_shared< TPTP::NamedType >( "$int" );
        }
        case Type::Kind::RATIONAL:
        {
            return std::make_shared< TPTP::NamedType >( "$rat" );
        }
        case Type::Kind::BINARY:
        {
            // TODO: @moosbruggerj fix me
            return std::make_shared< TPTP::NamedType >( "'!bin'" );
        }
        case Type::Kind::DECIMAL:
        {
            return std::make_shared< TPTP::NamedType >( "$real" );
        }
        case Type::Kind::STRING:
        {
            return std::make_shared< TPTP::NamedType >( "$i" );
        }

        // composed
        // TODO: @moosbruggerj fix me: composed types
        case Type::Kind::ENUMERATION:
        case Type::Kind::RANGE:
        case Type::Kind::TUPLE:
        case Type::Kind::RECORD:
        case Type::Kind::LIST:

        // reference
        case Type::Kind::RULE_REFERENCE:
        case Type::Kind::FUNCTION_REFERENCE:

        // abstraction
        case Type::Kind::FILE:
        case Type::Kind::PORT:
        {
            return std::make_shared< TPTP::NamedType >( "$i" );
        }

        // size of all type kinds
        case Type::Kind::_SIZE_:
        {
            return std::make_shared< TPTP::NamedType >( "$int" );
        }
    }
}

const TPTP::Literal::Ptr SymbolicExecutionEnvironment::tptpLiteralFromNumericConstant(
    const Constant& constant ) const
{
    switch( constant.typeId().kind() )
    {
        case Type::Kind::INTEGER:
        {
            auto& val = static_cast< const IntegerConstant& >( constant ).value();
            return std::make_shared< TPTP::IntegerLiteral >( val.to_string() );
        }
        case Type::Kind::BINARY:
        {
            // TODO: @moosbruggerj fix me
            throw InternalException( "unimplemented '" + constant.description() + "'" );
        }
        case Type::Kind::STRING:
        {
            // TODO: @moosbruggerj fix me
            auto& val = static_cast< const StringConstant& >( constant ).value();
            return std::make_shared< TPTP::IntegerLiteral >( val.toString() );
        }
        default:
        {
            throw InternalException( "unimplemented '" + constant.description() + "'" );
        }
    }
}

TPTP::Atom::Ptr SymbolicExecutionEnvironment::tptpAtomFromConstant( const Constant& constant ) const
{
    if( constant.symbolic() )
    {
        assert( isa< SymbolicConstant >( constant ) );
        return std::make_shared< TPTP::ConstantAtom >( constant.name(), TPTP::Atom::Kind::PLAIN );
    }
    else
    {
        auto literal = tptpLiteralFromNumericConstant( constant );
        return std::make_shared< TPTP::DefinedAtom >( literal );
    }
}

void SymbolicExecutionEnvironment::generateFunctionDefinition(
    const Value& value, const std::string& formulaName )
{
    switch( value.id() )
    {
        case Value::ID::ADD_INSTRUCTION:
        {
            auto& result = value.type().result();
            if( !( result.isDecimal() || result.isInteger() || result.isRational() ) )
            {
                throw InternalException(
                    "arguments for '" + Value::token( value.id() ) +
                    "' must be of arithmetic type." );
            }
            if( value.type().arguments().size() != 2 )
            {
                throw InternalException(
                    "'" + Value::token( value.id() ) + "' must have 2 arguments." );
            }
            auto X = std::make_shared< TPTP::VariableTerm >(
                "X", getTPTPType( *( value.type().arguments()[ 0 ] ) ) );
            auto Y = std::make_shared< TPTP::VariableTerm >(
                "Y", getTPTPType( *( value.type().arguments()[ 1 ] ) ) );
            auto Z =
                std::make_shared< TPTP::VariableTerm >( "Z", getTPTPType( value.type().result() ) );

            auto funcCall = std::make_shared< TPTP::FunctorAtom >(
                formulaName,
                std::initializer_list< TPTP::Logic::Ptr >{ X, Y, Z },
                TPTP::Atom::Kind::PLAIN );

            X = std::make_shared< TPTP::VariableTerm >(
                "X", getTPTPType( *( value.type().arguments()[ 0 ] ) ) );
            Y = std::make_shared< TPTP::VariableTerm >(
                "Y", getTPTPType( *( value.type().arguments()[ 1 ] ) ) );
            Z = std::make_shared< TPTP::VariableTerm >( "Z", getTPTPType( value.type().result() ) );

            auto sum = std::make_shared< TPTP::FunctorAtom >(
                "$sum",
                std::initializer_list< TPTP::Logic::Ptr >{ X, Y },
                TPTP::Atom::Kind::DEFINED );

            auto equals = std::make_shared< TPTP::InfixLogic >(
                sum, TPTP::InfixLogic::Connective::EQUALITY, Z );
            auto definition = std::make_shared< TPTP::InfixLogic >(
                funcCall, TPTP::InfixLogic::Connective::EQUALITY, equals );

            X = std::make_shared< TPTP::VariableTerm >(
                "X", getTPTPType( *( value.type().arguments()[ 0 ] ) ) );
            Y = std::make_shared< TPTP::VariableTerm >(
                "Y", getTPTPType( *( value.type().arguments()[ 1 ] ) ) );
            Z = std::make_shared< TPTP::VariableTerm >( "Z", getTPTPType( value.type().result() ) );

            auto quantified = std::make_shared< TPTP::QuantifiedLogic >(
                TPTP::QuantifiedLogic::Quantifier::UNIVERSAL,
                std::initializer_list< decltype( X ) >{ X, Y, Z },
                definition );

            auto formula = std::make_shared< TPTP::TypedFirstOrderFormula >( quantified );
            auto formulaDefiniton = std::make_shared< TPTP::FormulaDefinition >(
                generateFormulaName(), TPTP::Role::hypothesis(), formula );
            m_functionDefinitons.push_back( formulaDefiniton );
        }
        default:
        {
            throw InternalException(
                "behavior for '" + Value::token( value.id() ) + "' not defined.'" );
        }
    }
}
std::string SymbolicExecutionEnvironment::storeFunctionFromName( const std::string& name ) const
{
    return "'@" + name + "'";
}

void SymbolicExecutionEnvironment::setAtTime(
    const std::string& varName,
    const std::vector< Constant >& arguments,
    const TPTP::Atom::Ptr symbol,
    int time )
{
    auto fName = storeFunctionFromName( varName );
    auto timeAtom = std::make_shared< TPTP::DefinedAtom >( time );
    auto args =
        std::make_shared< TPTP::ListLogicElements >( std::initializer_list< TPTP::Logic::Ptr >{
            timeAtom,
        } );

    for( auto& arg : arguments )
    {
        args->add( tptpAtomFromConstant( arg ) );
    }
    args->add( symbol );
    auto functor = std::make_shared< TPTP::FunctorAtom >( fName, args, TPTP::Atom::Kind::PLAIN );
    addFormula( functor );
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

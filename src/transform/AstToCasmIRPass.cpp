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

#include "AstToCasmIRPass.h"


char libcasm_ir::AstToCasmIRPass::id = 0;

static libpass::PassRegistration< libcasm_ir::AstToCasmIRPass > PASS
( "AST to CASM IR"
, "internal: translates the AST to the CASM intermeditate representation"
, 0
, 0
);


extern Driver* global_driver;

static libcasm_ir::Type* getType( Type* type )
{
    assert( type && "not initialized type used" );
    
    switch( type->t )
    {
        case TypeType::SELF: // Agent!
            return new libcasm_ir::Type( libcasm_ir::Type::ID::AGENT );
        case TypeType::RULEREF:
            return new libcasm_ir::Type( libcasm_ir::Type::ID::RULE_POINTER );
        case TypeType::BOOLEAN:
            return new libcasm_ir::Type( libcasm_ir::Type::ID::BOOLEAN );
        case TypeType::INTEGER:
            return new libcasm_ir::Type( libcasm_ir::Type::ID::INTEGER );
        case TypeType::BIT:
            if( not( type->bitsize > 0
                  && type->bitsize <= 256
            ))
            {
                std::cout << std::string( __FUNCTION__ ) << ": " << type->to_str() << "\n";
            }
            assert( type->bitsize > 0
                 && type->bitsize <= 256
                 && "invalid Bit size for Bit type" );
            return new libcasm_ir::Type( libcasm_ir::Type::ID::BIT, type->bitsize );
        case TypeType::STRING:
            return new libcasm_ir::Type( libcasm_ir::Type::ID::STRING );
        default:
            std::cerr << type->to_str() << "\n";
            assert( 0 && "not implemented function atom identifier type" );
            return 0;
    }
}

bool libcasm_ir::AstToCasmIRPass::run( libpass::PassResult& pr )
{
    specification = 0;
    
    Ast* root = (Ast*)pr.getResult< libcasm_fe::TypeCheckPass >();
    
    AstWalker< AstToCasmIRPass, bool > walker( *this );
    
    walker.suppress_calls = true;
    walker.walk_specification( root );
    
    // PPA: this could be extracted to a 'update and check consistency' Value function?
    Value::SymbolTable& symbols = *Value::getSymbols();
    for( auto value : symbols[".rulepointer"] )
    {
        assert( Value::isa< RulePointerConstant >( value ) );
        ((RulePointerConstant*)value)->resolve();
    }
    
    
    //casm_frontend_destroy();
    
    
    pr.setResult< AstToCasmIRPass >( specification );
    pr.setResult< CasmIRDumpPass >( specification );
    
    return true;
    
    // std::string input = "";
    
    // while( input.compare( "q" ) != 0 )
    // {
    //     std::getline( cin, input );

    //     if( input.compare( "#" ) == 0 )
    //     {
    //         assert( 0 );
    //         return false;
    //     }
    //     else
    //     {
    //         for( auto r : (*Value::getSymbols())[ input.c_str() ] )
    //         {
    //             printf( "dumping '%s':\n", input.c_str() );
    //             r->dump();
    //         }
    //     }
    // }
    
    // return true;
}



libcasm_ir::Specification* libcasm_ir::AstToCasmIRPass::getSpecification( void ) const
{
    assert( specification );
    return specification;
}


#define VISIT
//printf( "===--- %s:%i: %s: %p: %s\n", __FILE__, __LINE__, __FUNCTION__, node, node->to_str().c_str() )

#define FIXME                                              \
    printf( "+++ FIXME +++: '%s:%i' in '%s'\n",              \
            __FILE__, __LINE__, __FUNCTION__ );              \
    fflush( stdout );                                      \
    assert( 0 );



void libcasm_ir::AstToCasmIRPass::visit_specification( SpecificationNode* node )
{
    // VISIT;
    assert( !specification );
    specification = new libcasm_ir::Specification( libstdhl::Allocator::string( node->identifier ) );
}

void libcasm_ir::AstToCasmIRPass::visit_init( InitNode* node )
{
    // VISIT;
    RulePointerConstant* ir_init = RulePointerConstant::create( node->identifier.c_str() );
    
    // single execution agent!
    Agent* ir_agent = new Agent();
    assert( ir_agent );
    ir_agent->setInitRulePointer( ir_init );    
    getSpecification()->add( ir_agent );
    
    // 'program' function!
    libcasm_ir::Type* ftype = new libcasm_ir::Type( libcasm_ir::Type::ID::RULE_POINTER );
    assert( ftype );
    ftype->addParameter( &libcasm_ir::AgentType );    
    libcasm_ir::Function* ir_function = new libcasm_ir::Function( libstdhl::Allocator::string( "program" ), ftype );
    assert( ir_function );    
    getSpecification()->add( ir_function );
    
}

void libcasm_ir::AstToCasmIRPass::visit_body_elements( AstNode* node )
{
    VISIT;
    //FIXME;TODO ASAP!!!
}

void libcasm_ir::AstToCasmIRPass::visit_function_def
( FunctionDefNode* node
, const std::vector<std::pair<T, T>>& inits
)
{
    VISIT;

    string x;
    for( auto& a : node->sym->arguments_ )
    {
        x.append( a->to_str() );
    }
    
    // printf( "%s, %lu: %s -> %s, \n"
    //         , node->sym->name.c_str()
    //         , node->sym->id
    //         , x.c_str()
    //         , node->sym->return_type_->to_str().c_str()
    //     );

    
    Type* ftype = getType( node->sym->return_type_ );
    for( auto argument : node->sym->arguments_ )
    {
        ftype->addParameter( getType( argument ) );
    }
        
    Function* ir_function =
        new Function( node->sym->name.c_str(), ftype );
    assert( ir_function );
    
    getSpecification()->add( ir_function );
}

    
void libcasm_ir::AstToCasmIRPass::visit_derived_def_pre( FunctionDefNode* node )
{
    VISIT;

    Type* ftype = getType( node->sym->return_type_ );

    std::vector< Type* > param_types;
    
    for( auto argument : node->sym->arguments_ )
    {
        param_types.push_back( getType( argument ) );
        ftype->addParameter( param_types.back() );
    }
    
    Derived* ir_derived = new Derived( node->sym->name.c_str(), ftype );
    assert( ir_derived );

    for( i32 i = 0; i < node->sym->arguments_.size(); i++ )
    {
        const char* param_ident = node->sym->parameter[i];
        // printf( "param %s\n", param_ident );
        
        ir_derived->addParameter
        ( Identifier::create
          ( param_types[i]
          , param_ident
        //, ir_derived
          )
        );
    }
    
    current_scope.push_back( ir_derived );
}

void libcasm_ir::AstToCasmIRPass::visit_derived_def( FunctionDefNode* node, T expr )
{
    VISIT;
    string x;
    for( auto& a : node->sym->arguments_ )
    {
        x.append( a->to_str() );
    }
    // printf( "%s, %lu: %s -> %s, \n", node->sym->name.c_str(), node->sym->id, x.c_str(), node->sym->return_type_->to_str().c_str() );
    
    assert( node->sym->type == Symbol::SymbolType::DERIVED );
    
    Value* ir_expr = lookup< Value >( node->sym->derived  );
    assert( ir_expr );
    
    TrivialStatement* ir_stmt = new TrivialStatement();
    assert( ir_stmt );
    ir_stmt->add( ir_expr );

    
    assert( Value::isa< Derived >( current_scope.back() ) and "invalid scope!" );
    Derived* ir_derived = (Derived*)current_scope.back();    
    ir_derived->setContext( ir_stmt );
        
    for( auto param : ir_derived->getParameters() )
    {
        Identifier::forgetSymbol( param->getName() );
    }
    
    current_scope.pop_back();    

    getSpecification()->add( ir_derived );
}

void libcasm_ir::AstToCasmIRPass::visit_skip( AstNode* node )
{
    VISIT;

    ExecutionSemanticsBlock* ir_scope =
        lookupParent< ExecutionSemanticsBlock >( node );
    assert( ir_scope );
    
    TrivialStatement* ir_stmt = new TrivialStatement( ir_scope );
    assert( ir_stmt );
    ast2casmir[ node ] = ir_stmt;
    
    SkipInstruction* ir_skip = new SkipInstruction();
    assert( ir_skip );
    
    ir_stmt->add( ir_skip );
}

void libcasm_ir::AstToCasmIRPass::visit_rule( RuleNode* node )
{
//    printf( "\n\n" );
    VISIT;
    string x;
    for( auto& a : node->arguments )
    {
        x.append( a->to_str() );
    }
//    printf( "%p -> %p\n", node, node->child_ );
    assert( node );
    
    Rule* ir_rule = new Rule( node->name.c_str() );
    assert( ir_rule );
    ast2casmir[ node ]        = ir_rule;
    ast2parent[ node->child_ ] = node;

    if( node->child_->node_type_ != NodeType::PARBLOCK )
    {
        ParallelBlock* ir_scope = new ParallelBlock();
        assert( ir_scope );
        ast2casmir[ node ] = ir_scope;

        ir_rule->setContext( ir_scope );
    }
    
    // for( i32 i = 0; i < node->sym->arguments_.size(); i++ )
    // {
    //     const char* param_ident = node->sym->parameter[i];
    //     printf( "param %s\n", param_ident );
        
    //     ir_derived->addParameter
    //     ( Identifier::create
    //       ( param_types[i]
    //       , param_ident
    //     //, ir_derived
    //       )
    //     );
    // }

    current_scope.push_back( ir_rule );
}

void libcasm_ir::AstToCasmIRPass::visit_rule_post( RuleNode* node )
{
    VISIT;

    assert( Value::isa< Rule >( current_scope.back() ) and "invalid scope!" );
    Rule* ir_rule = (Rule*)current_scope.back();
    
    getSpecification()->add( ir_rule );
    
    // PPA: CONT' HERE !!!
    // for( auto param : ir_derived->getParameters() )
    // {
    //     Identifier::forgetSymbol( param->getName() );
    // }
    
    current_scope.pop_back();
}

void libcasm_ir::AstToCasmIRPass::visit_parblock( UnaryNode* node )
{
    VISIT;
    // printf( "{ }\n" );
    // printf( "%p -> %p\n", node, node->child_ );
    
    assert( node );
    
    ParallelBlock* ir_scope = new ParallelBlock();
    assert( ir_scope );
    ast2casmir[ node ] = ir_scope;
    ast2parent[ node->child_ ] = node;
    
    Value* parent = lookupParent< Value >( node );
    assert( parent );
    
    if( Value::isa< Rule >( parent ) )
    {
        Rule* ir_rule = ((Rule*)parent);
        ir_rule->setContext( ir_scope );
        ir_scope->bind( ir_rule );
    }
    else if( Value::isa< ExecutionSemanticsBlock >( parent ) )
    {
        ((ExecutionSemanticsBlock*)parent)->add( ir_scope );
    }
    else if( Value::isa< Statement >( parent ) )
    {
        ((Statement*)parent)->getScope()->add( ir_scope );
    }
    else
    {
        assert( 0 );
    }
}

void libcasm_ir::AstToCasmIRPass::visit_seqblock( UnaryNode* node )
{    
    VISIT;
    // printf( "{| |}\n" );
    // printf( "%p -> %p\n", node, node->child_ );
    
    assert( node );
    
    SequentialBlock* ir_scope = new SequentialBlock();
    assert( ir_scope );
    ast2casmir[ node ] = ir_scope;
    ast2parent[ node->child_ ] = node;

    Value* parent = lookupParent< Value >( node );
    assert( parent );
    
    // if( Value::isa< Rule >( parent ) )
    // {
    //     ParallelBlock* ir_par = new ParallelBlock();
    //     ir_par->add( ir_scope );
    //     ((Rule*)parent)->setContext( ir_par );
    // }
    // else
    if( Value::isa< ExecutionSemanticsBlock >( parent ) )
    {
        ((ExecutionSemanticsBlock*)parent)->add( ir_scope );
    }
    else if( Value::isa< Statement >( parent ) )
    {
        ((Statement*)parent)->getScope()->add( ir_scope );
    }
    else
    {
        assert( 0 );
    }
}

void libcasm_ir::AstToCasmIRPass::visit_statements( AstListNode* node )
{
    VISIT;
    // printf( "...\n" );
    // for( AstNode *s : node->nodes )
    // {
    //     printf( "%p -> %p\n", node, s );
    // }
    
    assert( node );
    
    auto parent = ast2parent.find( node );
    assert( parent != ast2parent.end() );
    
    for( AstNode *s : node->nodes )
    {
        ast2parent[ s ] = parent->second;
    }
}
    
void libcasm_ir::AstToCasmIRPass::visit_forall_pre( AstNode* node )
{
    VISIT;
    FIXME;
}
    
void libcasm_ir::AstToCasmIRPass::visit_forall_post( AstNode* node )
{
    VISIT;
    FIXME;
}
    
void libcasm_ir::AstToCasmIRPass::visit_iterate( AstNode* node )
{
    VISIT;
    FIXME;
}

void libcasm_ir::AstToCasmIRPass::visit_update( UpdateNode* node, T func, T expr )
{
    VISIT;
    // printf( "%p -> %p\n", node, node->func );
    // printf( "%p -> %p\n", node, node->expr_ );
    
    ExecutionSemanticsBlock* ir_scope =
        lookupParent< ExecutionSemanticsBlock >( node );
    assert( ir_scope );
    
    TrivialStatement* ir_stmt = new TrivialStatement( ir_scope );
    assert( ir_stmt );
    ast2casmir[ node ]         = ir_stmt;
    ast2parent[ node->func ]   = node;
    ast2parent[ node->expr_ ]  = node;
    
    Value* ir_lhs = lookup< Value >( node->func  );
    Value* ir_rhs = lookup< Value >( node->expr_ );
    
    assert( ir_lhs );
    assert( ir_rhs );

    assert( Value::isa< LookupInstruction >( ir_lhs ) );
    
    ir_stmt->add( new UpdateInstruction(
                      ((Instruction*)ir_lhs)->getValue( 0 ), ir_rhs ) );
}

void libcasm_ir::AstToCasmIRPass::visit_update_dumps( UpdateNode* node, T func, T expr )
{
    VISIT;
    FIXME;
}
    
void libcasm_ir::AstToCasmIRPass::visit_update_subrange( UpdateNode* node, T func, T expr)
{
    VISIT;
    FIXME;
}
    
void libcasm_ir::AstToCasmIRPass::visit_call_pre( CallNode* node )
{
    //VISIT;
}
    
void libcasm_ir::AstToCasmIRPass::visit_call_pre( CallNode* node, T expr )
{
    //VISIT;
}

void libcasm_ir::AstToCasmIRPass::visit_call( CallNode* node, std::vector< T >& args )
{
    VISIT;
    
    printf( "FIXME: call: %s, %p, %p\n", node->rule_name.c_str(), node->rule, node->ruleref );
    
    // ExecutionSemanticsBlock* ir_scope =
    //     lookupParent< ExecutionSemanticsBlock >( node );
    // assert( ir_scope );
    
    // TrivialStatement* ir_stmt = new TrivialStatement( ir_scope );
    // assert( ir_stmt );
    // ast2casmir[ node ]         = ir_stmt;
    // // ast2parent[ node->func ]   = node;
    // // ast2parent[ node->expr_ ]  = node;
    
    // Value* instr = lookup< Value >( a );
    
    // CallInstruction* ir_call = new CallInstruction();
    // assert( ir_call );
    
    // for( auto a : node->atoms )
    // {
    //     Value* instr = lookup< Value >( a );
    //     ir_print->add( instr );
    // }
    // ir_print->add( StringConstant::create( (const char*)"\n" ) );
    
    // ir_stmt->add( ir_print );

    
    //FIXME;
}

void libcasm_ir::AstToCasmIRPass::visit_call_post( CallNode* node )
{
    //VISIT;
}

void libcasm_ir::AstToCasmIRPass::visit_print( PrintNode* node, T expr )
{
    VISIT;
    
    ExecutionSemanticsBlock* ir_scope =
        lookupParent< ExecutionSemanticsBlock >( node );
    assert( ir_scope );
    
    TrivialStatement* ir_stmt = new TrivialStatement( ir_scope );
    assert( ir_stmt );
    ast2casmir[ node ]         = ir_stmt;
    
    PrintInstruction* ir_print = new PrintInstruction();
    assert( ir_print );
    
    // for( auto a : node->atoms )
    // {
    //     Value* instr = lookup< Value >( a );
    //     ir_print->add( instr );
    // }
    ir_print->add( lookup< Value >( node->getAtom() ) );
    
    // INFO: PPA: commented line below, because the 'IR' itself adds this during runtime (interpreter)
    // and compilation --> this should not be part of the CASM IR!!! 
    //ir_print->add( StringConstant::create( (const char*)"\n" ) );
    
    ir_stmt->add( ir_print );
}
    
void libcasm_ir::AstToCasmIRPass::visit_diedie( DiedieNode* node, T msg )
{
    VISIT;
    FIXME;
}
    
void libcasm_ir::AstToCasmIRPass::visit_impossible( AstNode* node )
{
    VISIT;
    FIXME;
}
    
void libcasm_ir::AstToCasmIRPass::visit_assert( UnaryNode* node, T expr )
{
    VISIT;

    ExecutionSemanticsBlock* ir_scope =
        lookupParent< ExecutionSemanticsBlock >( node );
    assert( ir_scope );
    
    TrivialStatement* ir_stmt = new TrivialStatement( ir_scope );
    assert( ir_stmt );
    ast2casmir[ node ]         = ir_stmt;
    
    Value* ir_cond = lookup< Value >( node->child_ );
    assert( ir_cond );
    
    Instruction* ir_instr = new AssertInstruction( ir_cond );
    assert( ir_instr );
    
    ir_stmt->add( ir_instr );
}
    
void libcasm_ir::AstToCasmIRPass::visit_assure( UnaryNode* node, T expr )
{
    VISIT;
    FIXME;
}

void libcasm_ir::AstToCasmIRPass::visit_let( LetNode* node, T var )
{
    VISIT;
    
    ast2parent[ node->expr ] = node;
    ast2parent[ node->stmt ] = node;
    
    Value* ir_expr = lookup< Value >( node->expr );
    assert( ir_expr );
    
    Value* ir_ident =
        Identifier::create( getType( &node->type_ ), node->identifier.c_str() );
    assert( ir_ident );
    
    Value* ir_let = new LetInstruction( ir_ident, ir_expr );
    assert( ir_let );
    
    ExecutionSemanticsBlock* ir_scope = 0;
    Value* parent = lookupParent< Value >( node );
    assert( parent );
    if( Value::isa< ExecutionSemanticsBlock >( parent ) )
    {
        ir_scope = (ExecutionSemanticsBlock*)parent;
        // if( ir_scope->isParallel() )
        // {
        //     ExecutionSemanticsBlock* ir_scope_top = ir_scope;
        //     ir_scope = new SequentialBlock();
        //     assert( ir_scope );
        //     ir_scope->setParent( ir_scope_top );
        //     ir_scope_top->add( ir_scope );
        // }        
    }
    else if( Value::isa< Statement >( parent ) )
    {
        ir_scope = ((Statement*)parent)->getScope();
        //printf( "up for %p\n", parent );
    }
    else
    {
        assert( 0 );
    }
    
    BranchStatement* ir_stmt = new BranchStatement( ir_scope );
    assert( ir_stmt );
    ast2casmir[ node ] = ir_stmt;
    
    ir_stmt->add( ir_let );
    
    if( node->stmt->node_type_ != NodeType::PARBLOCK )
    {
        ParallelBlock* ir_blk = new ParallelBlock();
        assert( ir_blk );
        ast2casmir[ node ] = ir_blk;
        ir_stmt->addBlock( ir_blk );
    }

    assert( !" DEPRECATED IMPLEMENTATION, TODO: FIXME: PPA!!! " );
}

void libcasm_ir::AstToCasmIRPass::visit_let_post( LetNode* node )
{
    VISIT;    
    Identifier::forgetSymbol( node->identifier.c_str() );
}

void libcasm_ir::AstToCasmIRPass::visit_push( PushNode* node, T expr, T atom )
{
    VISIT;
    FIXME;
}
    
void libcasm_ir::AstToCasmIRPass::visit_pop( PopNode* node )
{
    VISIT;
    FIXME;
}


void libcasm_ir::AstToCasmIRPass::visit_ifthenelse( IfThenElseNode* node, T cond )
{
    VISIT;
    // printf( "%p -> %p\n",      node, node->condition_ );
    // printf( "%p -> %p | %p\n", node, node->then_, node->else_ );

    ExecutionSemanticsBlock* ir_scope =
        lookupParent< ExecutionSemanticsBlock >( node );
    assert( ir_scope );
    
    BranchStatement* ir_stmt = new BranchStatement( ir_scope );
    assert( ir_stmt );
    
    assert( node->condition_ );
    Value* ir_cond = lookup< Value >( node->condition_ );
    assert( ir_cond );
    assert
    ( ( Value::isa< Instruction     >( ir_cond ) and ir_cond->getType()->getIDKind() == Type::BOOLEAN )
    or  Value::isa< BooleanConstant >( ir_cond )
    );

    ir_stmt->add( ir_cond );
    
    assert( node->then_ );
    ExecutionSemanticsBlock* ir_case_true = new ParallelBlock( ir_scope );
    assert( ir_case_true );
    ast2casmir[ node ]             = ir_case_true;
    ast2parent[ node->then_ ]      = node;
    ir_stmt->addBlock( ir_case_true  );
    
    if( node->else_ )
    {
        assert(0);
        // ExecutionSemanticsBlock* ir_case_false = new ParallelBlock( ir_scope );
        // assert( ir_case_false );
        // ast2casmir[ node->condition_ ] = ir_case_false;
        // ast2parent[ node->condition_ ] = node;
        // ast2parent[ node->else_ ]      = node->condition_;
        
        // ir_stmt->addBlock( ir_case_false );
    }
}


void libcasm_ir::AstToCasmIRPass::visit_case_pre( CaseNode* node, T val )
{
    VISIT;
    // printf( "%p -> %p\n",      node, node->expr );

    ExecutionSemanticsBlock* ir_scope =
        lookupParent< ExecutionSemanticsBlock >( node );
    assert( ir_scope );
    
    for( auto& a : node->case_list )
    {
        // printf( "%p: %p -> %p\n", &node->case_list, a.first, a.second );
        
        Value* ir_case = new ParallelBlock( ir_scope );
        assert( ir_case );
        ast2casmir[ (AstNode*)&a ] = ir_case;
        ast2parent[ a.second ] = (AstNode*)&a;
    }
}


void libcasm_ir::AstToCasmIRPass::visit_case( CaseNode* node, T val, const std::vector< T >& case_labels )
{
    VISIT;
    
    ExecutionSemanticsBlock* ir_scope =
        lookupParent< ExecutionSemanticsBlock >( node );
    assert( ir_scope );
    
    BranchStatement* ir_stmt = new BranchStatement( ir_scope );
    assert( ir_stmt );
    
    Value* ir_expr = lookup< Value >( node->expr );
    assert( ir_expr );
    assert
    (  Value::isa< Instruction   >( ir_expr )
    or Value::isa< ConstantValue >( ir_expr )
    or Value::isa< Identifier    >( ir_expr )
    );

    Instruction* ir_switch = new SwitchInstruction( ir_expr );
    assert( ir_switch );
    
    ir_stmt->add( ir_switch );
    
    ExecutionSemanticsBlock* default_case = 0;
    
    for( auto& a : node->case_list )
    {
        // printf( "case: %p: %p -> %p\n", &node->case_list, a.first, a.second );
        
        ExecutionSemanticsBlock* ir_case =
            lookupParent< ExecutionSemanticsBlock >( a.second );
        assert( ir_case );
        
        if( !a.first )
        {
            default_case = ir_case;
            continue;
        }
        
        Value* ir_label = lookup< Value >( a.first  );
        assert( ir_label );
        
        ir_switch->add( ir_label );
        ir_stmt->addBlock( ir_case );
    }
    
    if( default_case )
    {
        ir_stmt->addBlock( default_case );
    }
}

T libcasm_ir::AstToCasmIRPass::visit_expression( BinaryExpression* node, T lhs, T rhs )
{
    VISIT;
    // printf( "%s, %p, %p\n", operator_to_str( node->op ).c_str(), node->left_, node->right_ );
    
    Value* ir_lhs = lookup< Value >( node->left_  );
    Value* ir_rhs = lookup< Value >( node->right_ );
    assert( ir_lhs );
    assert( ir_rhs );
    
    Value* ir_expr = 0;
    switch( node->op )
    {
        case ExpressionOperation::ADD:
            ir_expr = new AddInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::SUB:
            ir_expr = new SubInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::MUL:
            ir_expr = new MulInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::DIV:
            ir_expr = new DivInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::RAT_DIV:
            ir_expr = new RivInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::MOD:
            ir_expr = new ModInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::EQ:
            ir_expr = new EquInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::NEQ:
            ir_expr = new NeqInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::LESSER:
            ir_expr = new LthInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::LESSEREQ:
            ir_expr = new LeqInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::GREATER:
            ir_expr = new GthInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::GREATEREQ:
            ir_expr = new GeqInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::OR:
            ir_expr = new OrInstruction(  ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::XOR:
            ir_expr = new XorInstruction( ir_lhs, ir_rhs );
            break;
        case ExpressionOperation::AND:
            ir_expr = new AndInstruction( ir_lhs, ir_rhs );
            break;
        default:
            assert( 0 && "unsupported expression operation" );
            FIXME;
    }
    
    assert( ir_expr );
    ast2casmir[ node ] = ir_expr;
    return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_expression_single( UnaryExpression* node, T val )
{
    VISIT;
    // printf( "%s, %p\n", operator_to_str( node->op ).c_str(), node->left_ );
    
    Value* ir_lhs = lookup< Value >( node->expr_  );
    assert( ir_lhs );
    
    Value* ir_expr = 0;
    switch( node->op )
    {
        case ExpressionOperation::NOT:
            ir_expr = new NotInstruction( ir_lhs );
            break;
        default:
            assert( 0 && "unsupported expression operation" );
            FIXME;
    }
    
    assert( ir_expr );
    ast2casmir[ node ] = ir_expr;
    return 0;
}
    
T libcasm_ir::AstToCasmIRPass::visit_function_atom( FunctionAtom* node, std::vector< T >& args )
{
    VISIT;
    // printf( "%s, %p", node->name.c_str(), node );
    // if( node->arguments )
    // {
    //     for( auto a : *(node->arguments) )
    //     {
    //         printf( ", %p", a );
    //     }
    // }
    // printf( "\n" );
    
    if( node->symbol_type == FunctionAtom::SymbolType::PARAMETER )
    {
        Value* scope = 0;
        if( current_scope.size() > 0 )
        {
            scope = current_scope.back();
        }
        
        Value* ir_ident = Identifier::create( getType( &node->type_ ), node->name.c_str(), scope );
        
        assert( ir_ident );        
        ast2casmir[ node ] = ir_ident;
        return 0;
    }
    
    Type* ty_ident = 0;
    switch( node->symbol_type )
    {
        case FunctionAtom::SymbolType::FUNCTION:
        {
            assert( node->symbol );
            ty_ident = getType( node->symbol->return_type_ );
            for( auto argument : node->symbol->arguments_ )
            {
                ty_ident->addParameter( getType( argument ) );
            }
            break;
        }
        default:
            assert( 0 && "not implemented function atom identifier symbol type" );
    }
    
    Value* ir_ident = Identifier::create( ty_ident, node->name.c_str() );
    assert( ir_ident );
    
    LocationInstruction* ir_loc    = new LocationInstruction( ir_ident );
    assert( ir_loc );
    if( node->arguments )
    {
        for( auto a : *(node->arguments) )
        {
            Value* instr = lookup< Value >( a );
            ir_loc->add( instr );        
        }
    }
    
    LookupInstruction* ir_lup = new LookupInstruction( ir_loc );
    assert( ir_lup );
    ast2casmir[ node ] = ir_lup;
    
    return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_function_atom_subrange( FunctionAtom* node, T args[], uint16_t argc    )
{
    VISIT;
    FIXME;
    return 0;
}

void libcasm_ir::AstToCasmIRPass::visit_derived_function_atom_pre( FunctionAtom* node, std::vector< T >& args )
{
    VISIT;
}

T libcasm_ir::AstToCasmIRPass::visit_derived_function_atom( FunctionAtom* node, T expr )
{
    VISIT;
    string x;
    // printf( "derived: %s, %s\n"
    //         , node->name.c_str()
    //         , x.c_str()
    //     );

    assert( node->symbol );
    assert( node->symbol_type == FunctionAtom::SymbolType::DERIVED );
    
    Type* ty_ident = getType( node->symbol->return_type_ );
    for( auto argument : node->symbol->arguments_ )
    {
        ty_ident->addParameter( getType( argument ) );
    }
    
    Value* ir_ident = Identifier::create( ty_ident, node->name.c_str() );
    assert( ir_ident );
    
    CallInstruction* ir_call = new CallInstruction( ir_ident );
    assert( ir_call );
    ast2casmir[ node ] = ir_call;
    if( node->arguments )
    {
        for( auto a : *(node->arguments) )
        {
            Value* instr = lookup< Value >( a );
            ir_call->add( instr );        
        }
    }
    
    return 0;
}



T libcasm_ir::AstToCasmIRPass::visit_undef_atom( UndefAtom* node )
{
    VISIT;
    // printf( "undef\n" );
    
    Value* ir_const = 0;
    
    switch( node->type_.t )
    {
        case TypeType::RULEREF:
            ir_const = RulePointerConstant::create(); break;
        case TypeType::BOOLEAN:
            ir_const = BooleanConstant::create(); break;
        case TypeType::BIT:
            ir_const = BitConstant::create( node->type_.bitsize ); break;
        case TypeType::INTEGER:
            ir_const = IntegerConstant::create(); break;
        case TypeType::STRING:
            ir_const = StringConstant::create(); break;
        default:
            assert( 0 && "unimplemented undef constant!" );
    }
    
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
    
    getSpecification()->add( ir_const );
    return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_boolean_atom( BooleanAtom* node )
{
    VISIT;
    // printf( "%u\n", node->value );
    
    BooleanConstant* ir_const
        = BooleanConstant::create( (Type::Boolean)node->value );
    
    assert( ir_const );
    ast2casmir[ node ] = ir_const;

    getSpecification()->add( ir_const );
    return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_int_atom( IntegerAtom* node )
{
    VISIT;
    // printf( "%lu\n", node->val_    );
    
    IntegerConstant* ir_const
        = IntegerConstant::create( (Type::Integer)node->val_ );    
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
    
    getSpecification()->add( ir_const );
    return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_bit_atom( IntegerAtom* node )
{
    VISIT;
    // printf( "%lu (0x%lx)\n", node->val_, node->val_    );
    
    BitConstant* ir_const = BitConstant::create( (u64)node->val_, node->type_.bitsize );    
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
    
    getSpecification()->add( ir_const );
    return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_floating_atom( FloatingAtom* node )
{
    VISIT;
    FIXME;
    return 0;
}
    
T libcasm_ir::AstToCasmIRPass::visit_rational_atom( RationalAtom* node )
{
    VISIT;
    FIXME;
    return 0;
}
    
T libcasm_ir::AstToCasmIRPass::visit_string_atom( StringAtom* node )
{
    VISIT;
    
    Value* ir_const    = StringConstant::create( node->string.c_str() );    
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
    
    // printf( "%s\n", node->string.c_str() );

    getSpecification()->add( ir_const );
    return 0;
}

    
T libcasm_ir::AstToCasmIRPass::visit_self_atom( SelfAtom* node )
{
    VISIT;

    Value* ir_const    = AgentConstant::create( 0 );
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
    
    getSpecification()->add( ir_const );
    return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_rule_atom( RuleAtom* node )
{
    VISIT;

    Value* ir_const = RulePointerConstant::create( node->name.c_str() );
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
    
    // printf( "rule: @%s\n", node->name.c_str() );
    
    getSpecification()->add( ir_const );
    return 0;
}
    
T libcasm_ir::AstToCasmIRPass::visit_list_atom( ListAtom* node, std::vector< T > &args )
{
    VISIT; 
    FIXME;
    return 0;
}
    
T libcasm_ir::AstToCasmIRPass::visit_number_range_atom( NumberRangeAtom* node , T start, T end )
{
    VISIT; 
    FIXME;
    return 0; 
}
    
T libcasm_ir::AstToCasmIRPass::visit_builtin_atom( BuiltinAtom* node, std::vector< T >& args )
{
    VISIT;

    // printf( "builtin: %s\n", node->to_str().c_str() );
    
    Type* ty_ident = getType( node->return_type );
    if( node->arguments )
    {
        for( auto a : *(node->arguments) )
        {
            ty_ident->addParameter( getType( &(a->type_) ) );
        }
    }
    
    Value* ir_ident = new libcasm_ir::Builtin( node->to_str().c_str(), ty_ident );
    assert( ir_ident );
    
    CallInstruction* ir_call = new CallInstruction( ir_ident );
    assert( ir_call );
    ast2casmir[ node ] = ir_call;
    if( node->arguments )
    {
        for( auto a : *(node->arguments) )
        {
            Value* instr = lookup< Value >( a );
            ir_call->add( instr );        
        }
    }
    
    getSpecification()->add( ir_call );
    return 0;
}



template<class C>
C* libcasm_ir::AstToCasmIRPass::lookupParent( AstNode* node )
{
    //printf( "%s: ??? %p\n", __FUNCTION__, node );
        
    auto result = ast2parent.find( node );
    if( result != ast2parent.end() )
    {
        //printf( "%s: %p, %p\n", __FUNCTION__, result->first, result->second );
        return lookup< C >( result->second );
    }
        
    return 0;
}

template<class C>
C* libcasm_ir::AstToCasmIRPass::lookup( AstNode* node )
{
    //printf( "%s: %p\n", __FUNCTION__, node );
        
    auto result = ast2casmir.find( node );
    if( result != ast2casmir.end() )
    {
        //printf( "%s: %p\n", __FUNCTION__, result->second );
        if( Value::isa< C >( result->second ) )
        {
            return static_cast< C* >( result->second );
        }
        else
        {
            assert( 0 );
        }
    }
        
    return 0;
}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  


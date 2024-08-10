#include "common/globals.h"
#include "frontend/lex.h"
#include "frontend/symbol_tbl.h"
#include "frontend/tree.h"
#include "codegen/gen.h"
#include "frontend/expr.h"
#include "frontend/statements.h"
#include "frontend/types.h"
#include <iostream>

/*
***** Grammar Start *****

variable_declaration: "int" identifier ";"

***** Grammar End *****
*/
#define check_redecl(sym)   if( sym_present(sym)) \
                            { \
                                std::cerr<<sym.name<<" is redclared\n"; \
                            }
                            
void variable_declaration()
{
    a_token tok;
    a_symbol sym;
    a_type_kind type;

    type = tok_to_type(curr_token.kind);
    get_token();
    tok = expect_ident();
    
    sym = make_symbol(tok , type , a_structural_type_kind::variable);
    check_redecl(sym);
    sym.state = a_variable_state::state_declared;
    add_sym(sym);
    
    // Not here will wait for an assignment to happen
    //gen_globalsym(curr_token.value);
    expect_semi();
}

an_ast_node* function_declaration()
{
    a_token tok;
    a_symbol sym;
    a_symtable_index idx;
    an_ast_node *compound_stmt , *return_tree , body;
    int declared_return_type , evaluated_return_type;

	declared_return_type  = (int)tok_to_type(curr_token.kind);
    get_token();
	tok = expect_ident();
    expect_lparen();
    expect_rparen();

    sym = make_symbol(tok , (a_type_kind)declared_return_type , a_structural_type_kind::function);
    check_redecl(sym);
    sym.state = a_function_state::state_declared;
    idx = add_sym(sym);

    
    compound_stmt = compound_statement();

    // check for return

    if(! is_void_type((a_type_kind)declared_return_type))
     {   
        expect_return();
        expect_lparen();

        return_tree = expr(0);
        evaluated_return_type = (int)return_tree->type_kind;
        if(!type_compatible(evaluated_return_type , declared_return_type , 1))
            {
                std::cerr<<"return type incompatible\n";
                exit(1);
            }

        if(evaluated_return_type)
            return_tree = mk_unary_node((an_ast_node_kind)evaluated_return_type , return_tree , void_ast_type{} , (a_type_kind)declared_return_type);

        return_tree = mk_unary_node(an_ast_node_kind::node_return , return_tree , idx , a_type_kind::none);
        expect_rparen();
        expect_semi();
        expect_rbrace();

        if ( compound_stmt != nullptr)
            compound_stmt  = mk_node(an_ast_node_kind::node_glue , compound_stmt , return_tree , void_ast_type{} , a_type_kind::none);
        else 
            compound_stmt = return_tree;

     }
    else
    {
        if (curr_token.kind == a_token_kind::tok_return)
        {
            get_token();
            expect_semi();
            expect_rbrace();
        }
    }
     
    return mk_unary_node(an_ast_node_kind::node_func, compound_stmt , idx , a_type_kind::none);
}
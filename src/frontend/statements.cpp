#include <iostream>
#include "frontend/lex.h"
#include "frontend/tree.h"
#include "frontend/expr.h"
#include "common/globals.h"
#include "codegen/gen.h"
#include "frontend/decl.h"
#include "frontend/statements.h"

/*
 ***** Grammar Start *****

compound_statement:	'{' '}' //empty
		| '{' statements '}'
		;
		
statements " statement
		| statement statements

statement :	"print" statement ";"
		| variable_declaration
		| assignment_statement
		| if_statement
		| while_statement
		;
assignment_statement: identifier "=" expression ";"

if_statement: if_head
      	| if_head 'else' compound_statement

if_head: 'if' '(' true_false_expression ')' compound_statement  ;

while_statement: 'while' '(' true_false_expression ')' compound_statement  ;

do_while_statement : 'do' compound_statement 'while' '(' true_false_expression ')' ;

identifer:	under_or_alpha
		|	identifier under_or_alpha
		|	identifier digit

under_or_alpha:	"_"
		|	"[a-z]"
		|	"[A-Z]"
	
digit:	"[0-9]"

 ***** Grammar End *****
 */

#define assigned(state)       (std::get<a_variable_state>(state) == a_variable_state::state_assigned)
#define check_decl(sym)     sym_present(sym,==," not declared\n")

an_ast_node* print_statement()
{
	a_token tok;
	an_ast_node*  ast;

	expect_print();
	ast = expr(0);
	ast = mk_unary_node(an_ast_node_kind::node_print, ast ,  void_ast_type{});
	expect_semi();
	return ast;
}

an_ast_node* assign_statement()
{
    a_token tok;
    a_symbol sym;
    a_symtable_index sym_idx;
    an_ast_node *left , *right , *node;
    tok = expect_ident();

    sym = make_symbol(tok);
    check_decl(sym);
    
    if (!assigned(sym.state))
    {
        sym.state = a_variable_state::state_assigned;
        update_symbol_state(sym);
        gen_globalsym(sym.name);
    }

	sym_idx = find_symbol(sym);
    right = mk_leaf_node(an_ast_node_kind::node_lvalue, sym_idx);

    expect_equals();

    // get the assigning expression
    left = expr(0);

    node = mk_node(an_ast_node_kind::node_assign, left, right, 0);

    expect_semi();
	return node;
}

an_ast_node* if_statement()
{
	an_ast_node *cond_node , *true_node , *false_node = nullptr;

	match(a_token_kind::tok_if);
	expect_lparen();

	cond_node = expr(0);

	if ( ! is_comparison_node(cond_node->op))
	{
		std::cerr<<"a comparison operator expected in if\n";
		exit(1);
	}

	expect_rparen();

	true_node = compound_statement();

	if (curr_token.kind == a_token_kind::tok_else)
	{
		get_token();
		false_node = compound_statement();
	}

	return (mk_node(an_ast_node_kind::node_if , cond_node, true_node, false_node, void_ast_type{}));
}

an_ast_node* while_statement()
{
	an_ast_node *cond_node, *while_body;

	match(a_token_kind::tok_while);
	expect_lparen();

	cond_node = expr(0);

	if (! is_comparison_node(cond_node->op))
	{
		std::cerr<<"a comparison operator expected in while\n";
		exit(1);
	}

	expect_rparen();

	while_body = compound_statement();

	return(mk_node(an_ast_node_kind::node_while , cond_node , while_body, void_ast_type{}));
}

an_ast_node* do_while_statement()
{
	an_ast_node *cond_node, *do_body;

	match(a_token_kind::tok_do);
	
	do_body = compound_statement();

	match(a_token_kind::tok_while);

	expect_lparen();
	cond_node = expr(0);
	expect_rparen();
	expect_semi();

	return(mk_node(an_ast_node_kind::node_do_while , cond_node , do_body , void_ast_type{}));

}

an_ast_node* statements()
{
	an_ast_node *tree , *left = nullptr;

	while(1)
	{
		switch (curr_token.kind)
		{
			case a_token_kind::tok_print:
				tree = print_statement();
			break;

			case a_token_kind::tok_int:
				variable_declaration();
				tree = nullptr;
			break;

			case a_token_kind::tok_ident:
				tree = assign_statement();
			break;

			case a_token_kind::tok_if:
				tree = if_statement();
			break;

			case a_token_kind::tok_while:
				tree = while_statement();
			break;

			case a_token_kind::tok_do:
				tree = do_while_statement();
			break;

			case a_token_kind::tok_rbrace:
				expect_rbrace();
				return left;

			default:
				std::cerr<<"Expected a statement\n";
				exit(1);
		}

		if (tree != nullptr)
		{
			if (left == nullptr)
				left = tree;
			else
				left = mk_node(an_ast_node_kind::node_glue , left , tree, void_ast_type{});
		}
	
	}
}



an_ast_node* compound_statement()
{
	an_ast_node  *tree;

	expect_lbrace();

	tree = statements();

	// rbrace already checked by statemnents 
	return tree;
}
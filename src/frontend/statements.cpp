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

function_declaration: 'void' identifier '(' ')' compound_statement;

function_call: identifier '(' expression ')' ; 

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
#define check_decl(sym)     if( !sym_present(sym)) \
                            { \
                                std::cerr<<sym.name<<" is not declared\n"; \
                            }
      

an_ast_node* print_statement()
{
	a_token tok;
	an_ast_node*  ast;
	int lefttype , righttype;

	expect_print();
	ast = expr(0);

	lefttype = (int)a_type_kind::Int;
	righttype =(int)ast->type_kind;

	if(!type_compatible(lefttype , righttype , false))
	{
		std::cerr<<"Incompatible types for print\n";
		exit(1);
	}

	if(righttype) ast = mk_unary_node((an_ast_node_kind)righttype , ast , void_ast_type{} , a_type_kind::Int);
	ast = mk_unary_node(an_ast_node_kind::node_print, ast ,  void_ast_type{} , a_type_kind::none);
	expect_semi();
	return ast;
}

an_ast_node* assign_statement( bool for_counter)
{
    a_token tok;
    a_symbol sym;
    a_symtable_index sym_idx;
    an_ast_node *left , *right , *node;
	a_type_kind type;
	int lefttype , righttype;

	if (peek().kind == a_token_kind::tok_lparen)
		return func_call();

	tok = expect_ident();

	type = get_sym_type(std::get<an_ident>(tok.value));
    sym = make_symbol(tok, type, a_structural_type_kind::variable);

    check_decl(sym);
	
	sym_idx = find_symbol(sym);

    if (!assigned(sym.state))
    {
        sym.state = a_variable_state::state_assigned;
        update_symbol_state(sym);
        gen_globalsym(sym.name);
    }

    right = mk_leaf_node(an_ast_node_kind::node_lvalue, sym_idx, sym.type);

    expect_equals();

    // get the assigning expression
    left = expr(0);

	lefttype = (int)left->type_kind;
	righttype = (int)right->type_kind;
	if(!type_compatible(lefttype , righttype , true))
	{
		std::cerr<<"Incompatible types\n";
		exit(1);
	}

	if(lefttype)
		left = mk_unary_node((an_ast_node_kind)lefttype , left , void_ast_type{} , right->type_kind);

    node = mk_node(an_ast_node_kind::node_assign, left, right, void_ast_type{}, a_type_kind::Int);
	
	if(for_counter) 
	{
		expect_rparen();
		return node;
	}
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

	return (mk_node(an_ast_node_kind::node_if , cond_node, true_node, false_node, void_ast_type{}, a_type_kind::none));
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

	return(mk_node(an_ast_node_kind::node_while , cond_node , while_body, void_ast_type{} , a_type_kind::none));
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

	return(mk_node(an_ast_node_kind::node_do_while , cond_node , do_body , void_ast_type{} , a_type_kind::none));

}
static an_ast_node* for_counter()
{
	an_ast_node* expr;

	// will add increment as well later
	if(curr_token.kind == a_token_kind::tok_ident)
	{
		expr = assign_statement(true);
		return expr;
	}
	else 
	{
		std::cerr<<"Expecting assign statement for \"for\"";
		exit(1);
	}
}
an_ast_node* for_statement()
{
	an_ast_node *init_node, *cond_node, *counter, *body;

	match(a_token_kind::tok_for);

	expect_lparen();

	init_node = assign_statement(false);
	cond_node = expr(0);
	expect_semi();
	counter = for_counter();
	body = compound_statement();

	return(mk_node(an_ast_node_kind::node_glue , init_node, 
															mk_node(an_ast_node_kind::node_while, cond_node , 
																											mk_node(an_ast_node_kind::node_glue, body, counter,void_ast_type{}, a_type_kind::none ), 						
																											void_ast_type{}, a_type_kind::none) ,
															void_ast_type{},a_type_kind::none  ));
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
			case a_token_kind::tok_char:
			case a_token_kind::tok_long:
				variable_declaration();
				tree = nullptr;
			break;

			case a_token_kind::tok_ident:
				tree = assign_statement(false);
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

			case a_token_kind::tok_for:
				tree = for_statement();
			break;

			case a_token_kind::tok_return:
				return left;
			
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
				left = mk_node(an_ast_node_kind::node_glue , left , tree, void_ast_type{}, a_type_kind::none);
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
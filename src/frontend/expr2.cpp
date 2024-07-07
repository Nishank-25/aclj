#include "frontend/tree.h"
#include "frontend/lex.h"
#include <iostream>

/*
***** Grammar START *****

expression: 	term
		| expression + term
		| expression - term
		;

term :  	primary
		| term * term
		| term / term 
		;

primary:    	a_number
         	;

a_number	a_tok_int_literal
		| a_tok_float_literal
		;

***** Grammar END *****
*/
extern const char *tok_str[];

an_ast_node_kind tok_to_node(a_token_kind kind)
{
	switch(kind)
	{
		case a_token_kind::tok_plus:
			return an_ast_node_kind::node_add;
		case a_token_kind::tok_minus:
			return an_ast_node_kind::node_sub;
		case a_token_kind::tok_mul:
			return an_ast_node_kind::node_mul;
		case a_token_kind::tok_div:
			return an_ast_node_kind::node_div;
		default:
			std::cerr<<"Syntax error "<<tok_str[(int)kind] << " is not a binary operator\n";
			exit(1);
	}
}

an_ast_node* primary()
{
	an_ast_node* node;
	a_token tok;
	
	tok = get_token();
	
	switch(tok.kind)
	{
		case a_token_kind::tok_int_literal :
			node = mk_leaf_node(an_ast_node_kind::node_int_literal, tok.value);
			next_token();
			return node;
		case a_token_kind::tok_float_literal :
			node = mk_leaf_node(an_ast_node_kind::node_float_literal,tok.value);
			next_token();
			return node;
		
		default:
			std::cerr<<"Syntax error expecting a literal value\n";
			exit(1);
	}	
}

an_ast_node* term()
{
	an_ast_node *left_operand , *right_operand;
	a_token_kind tok_kind;
	
	left_operand  = primary();
	
	if(curr_token.kind  == a_token_kind::tok_eof)
		return left_operand;	
		
	while(curr_token.kind == a_token_kind::tok_mul || curr_token.kind == a_token_kind::tok_div)
		{
			
			next_token();
			right_operand = primary();
			left_operand  = mk_node(tok_to_node(tok_kind),left_operand,right_operand,std::monostate{});
		        
			if(curr_token.kind == a_token_kind::tok_eof)
				break;
		}
	return left_operand;
}

//1 + 2*3 - 4/5 + 6*7*8 +9/10/11 + 12
an_ast_node* expr()
{
	an_ast_node *left_expr ,*right_term;
	an_ast_node_kind expr_kind; 
	
	/* parse the expr ( left subtree)  */
	left_expr = term();
	
	/* if last token, return  */
	while ( curr_token.kind != a_token_kind::tok_eof )
	{
		
	expr_kind = tok_to_node(curr_token.kind);
	next_token();
	
	/* this will only return when it hits with lower precedence  with higher precedence already formed */
	right_term = term();
        
	/* add it to the left expression */	
	left_expr  = mk_node(expr_kind,left_expr,right_term,std::monostate{});
	
	}
	return left_expr;

}

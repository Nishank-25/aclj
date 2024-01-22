#include "tree.h"
#include <iostream>
/*
***** Grammar START *****

expression: number
          | expression '*' expression
          | expression '/' expression
          | expression '+' expression
          | expression '-' expression
          ;

primary:    a_tok_int_literal
	  |
         ;

***** Grammar END *****
*/
extern const char *tok_str[];

a_ast_node_kind tok_to_node(a_token_kind kind)
{
	switch(kind)
	{
		case a_token_kind::tok_plus:
			return a_ast_node_kind::node_add;
		case a_token_kind::tok_minus:
			return a_ast_node_kind::node_sub;
		case a_token_kind::tok_mul:
			return a_ast_node_kind::node_mul;
		case a_token_kind::tok_div:
			return a_ast_node_kind::node_div;
		default:
			std::cerr<<"Oops that's a wrong arithematic  operation \n";
			exit(1);
	}
}

a_ast_node* primary()
{
	a_ast_node* node;
	a_token tok;
	tok = Tokens[curr_token_pos++];
	
	switch(tok.kind)
	{
		case a_token_kind::tok_int_literal :
			node = mk_leaf_node(a_ast_node_kind::node_int_literal, tok.value);
			return node;
		case a_token_kind::tok_float_literal :
			node = mk_leaf_node(a_ast_node_kind::node_float_literal,tok.value);
			return node;
		
		default:
			std::cerr<<"Syntax error\n";
			exit(1);
	}	
}

a_ast_node* parse_expr()
{
	a_ast_node *left ,*right ,*node; 
	a_ast_node_kind kind; 
	
	/* parse the primary ( left subtree)  */
	left = primary();
	
	/* if last token, return  */
	if(Tokens[curr_token_pos].kind == a_token_kind::tok_eof)
	       return left;
	
	/* convert token operation to node operation */
	kind = tok_to_node(Tokens[curr_token_pos++].kind);
	
	/* parse the right subtree */
	right = parse_expr();

	/* make the node */
	node  = mk_node(kind, left, right, 0);
	return node;

}

/* Pratt parsing for operator precedence */

#include"frontend/tree.h"
#include"frontend/lex.h"
#include<iostream>

a_ast_node_kind tok_to_node(a_token_kind kind);
a_ast_node* primary();
extern const char *tok_str[];

//Operator precedence for each token
int precedence_tbl[] = {10,10,20,20,0,0,0};

//Check that we have a binary operator and get the precedence
int get_precedence(a_token_kind tok_kind)
{
	int prec = precedence_tbl[(int)tok_kind];
	if (prec == 0 )
		{
			std::cerr<<"Syntax error "<<tok_str[(int)tok_kind] << " is not a binary operator\n";
			exit(1);
		}
	return prec;
}

// param precedence is previous tokens precedence
a_ast_node* expr(int precedence)
{
	a_ast_node *left, *right;

	left = primary();
	a_token_kind token_kind = get_token().kind;
	if(token_kind == a_token_kind::tok_semicolon)
		return left;

	while(get_precedence(token_kind) > precedence)
	{
		next_token();
		
		// bind to the operator with same precedence level as left
		right = expr(precedence_tbl[(int)token_kind]);

		// join it to left
		left = mk_node(tok_to_node(token_kind),left,right,std::monostate{});
		
		// If  we hit a semicolon, return the  left node
		token_kind = get_token().kind;
		if(token_kind == a_token_kind::tok_semicolon)
			return left;

	}
// Return the tree we have when the precedence is same or lower.
return left;
}



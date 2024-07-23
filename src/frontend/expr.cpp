/* Pratt parsing for operator precedence */

#include"frontend/tree.h"
#include"frontend/lex.h"
#include<iostream>

an_ast_node_kind tok_to_node(a_token_kind kind);
an_ast_node* primary();
extern const char *tok_str(a_token_kind);

//Operator precedence for each token
int precedence_tbl[] = {
	0,
	10,
	20,20,
	30,30,30,30,
	40,40,
	50,50	
};

//Check that we have a binary operator and get the precedence
int get_precedence(a_token_kind tok_kind)
{
	int prec = precedence_tbl[(int)tok_kind];
	if (prec == 0 )
		{
			std::cerr<<"Syntax error "<<tok_str(tok_kind) << " is not a binary operator\n";
			exit(1);
		}
	return prec;
}

// param precedence is previous tokens precedence
an_ast_node* expr(int precedence)
{
	an_ast_node *left, *right;
	a_token_kind curr_tok_kind;

	left = primary();
	curr_tok_kind = curr_token.kind;
	if(curr_tok_kind == a_token_kind::tok_semicolon || curr_tok_kind == a_token_kind::tok_rparen)
		return left;

	while(get_precedence(curr_tok_kind) > precedence)
	{
		get_token();
		// bind to the operator with same precedence level as left
		right = expr(precedence_tbl[(int)curr_tok_kind]);

		// join it to left
		left = mk_node(tok_to_node(curr_tok_kind),left,right,void_ast_type{});

		// If we hit a semicolon, return the  left node
		curr_tok_kind = curr_token.kind;
		if(curr_tok_kind == a_token_kind::tok_semicolon || curr_tok_kind == a_token_kind::tok_rparen)
			return left;

	}
// Return the tree we have when the precedence is same or lower.
return left;
}



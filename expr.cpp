/* Pratt parsing for operator precedence */

#include"tree.h"
#include"lex.h"
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
	a_token_kind tok_kind;

	left = primary();
	tok_kind = Tokens[curr_token_pos].kind;
	if(tok_kind == a_token_kind::tok_eof)
		return left;

	while(get_precedence(tok_kind) > precedence)
	{
		curr_token_pos++;
		
		// bind to the operator with same precedence level as left
		right = expr(precedence_tbl[(int)tok_kind]);

		// join it to left
		left = mk_node(tok_to_node(tok_kind),left,right,std::monostate{});
		
		// If no tokens left, return just the left node
		tok_kind = Tokens[curr_token_pos].kind;
		if(tok_kind == a_token_kind::tok_eof)
			return left;

	}
// Return the tree we have when the precedence is same or lower.
return left;
}



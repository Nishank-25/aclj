/* Pratt parsing for operator precedence */

#include"frontend/tree.h"
#include"frontend/lex.h"
#include"frontend/symbol_tbl.h"
#include<iostream>

extern const char *tok_str(a_token_kind);
an_ast_node* func_call();

#define check_decl(sym)     if( !sym_present(sym)) \
                            { \
                                std::cerr<<sym<<" is not declared\n"; \
                            }

an_ast_node_kind tok_to_node(a_token_kind kind)
{
	if ( a_token_kind::tok_eof < kind && kind < a_token_kind::tok_int_literal )
	{
		return (an_ast_node_kind)kind;
	}
	else {
		std::cerr<<"Syntax error "<<tok_str(kind) << " is not a binary operator\n";
		exit(1);
	}
}

an_ast_node* primary()
{
	an_ast_node* node;
	a_token tok;
	
	tok = curr_token;
	
	switch(tok.kind)
	{
		case a_token_kind::tok_int_literal :
			if( 0 <= std::get<int>(std::get<a_number>(tok.value))  && std::get<int>(std::get<a_number>(tok.value)) < 256)
				node = mk_leaf_node(an_ast_node_kind::node_int_literal , std::get<a_number>(tok.value), a_type_kind::Char);
			else 
				node = mk_leaf_node(an_ast_node_kind::node_int_literal, std::get<a_number>(tok.value) , a_type_kind::Int);
			
			get_token();
			return node;
		case a_token_kind::tok_float_literal :
			node = mk_leaf_node(an_ast_node_kind::node_float_literal,std::get<a_number>(tok.value), a_type_kind::Int);
			get_token();
			return node;

		case a_token_kind::tok_ident :
		 	a_symtable_index idx;
			idx = find_symbol(std::get<an_ident>(tok.value));

			if (peek().kind == a_token_kind::tok_lparen)
			{
				return func_call();
			}
			if (idx == SYMBOL_NOT_FOUND)
			{
				std::cerr<<"unknown variable"<<std::get<an_ident>(tok.value)<<"\n";
				exit(1);
			}

			node = mk_leaf_node(an_ast_node_kind::node_ident,idx,get_sym_type(std::get<an_ident>(tok.value)));
			get_token();
			return node;
			break;
	
		default:
			std::cerr<<"Syntax error expecting a literal value or an identifier\n";
			exit(1);
	}	
}

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
	int lefttype , rightype;

	left = primary();
	curr_tok_kind = curr_token.kind;
	if(curr_tok_kind == a_token_kind::tok_semicolon || curr_tok_kind == a_token_kind::tok_rparen)
		return left;

	while(get_precedence(curr_tok_kind) > precedence)
	{
		get_token();
		// bind to the operator with same precedence level as left
		right = expr(precedence_tbl[(int)curr_tok_kind]);

		lefttype = (int)left->type_kind;
		rightype = (int)right->type_kind;

		if(!type_compatible(lefttype , rightype , false))
		{
			std::cerr<<"Types incompatible\n";
			exit(1);
		}

		if(lefttype)
			left = mk_unary_node((an_ast_node_kind)lefttype , left , void_ast_type{} , right->type_kind);
		if(rightype)
			right = mk_unary_node((an_ast_node_kind)rightype, right , void_ast_type{} , left->type_kind);

		// join it to left
		left = mk_node(tok_to_node(curr_tok_kind),left,right,void_ast_type{},left->type_kind);

		// If we hit a semicolon, return the  left node
		curr_tok_kind = curr_token.kind;
		if(curr_tok_kind == a_token_kind::tok_semicolon || curr_tok_kind == a_token_kind::tok_rparen)
			return left;

	}
// Return the tree we have when the precedence is same or lower.
return left;
}


an_ast_node* func_call()
{
	a_token tok;
	an_ast_node* args;
	std::string func_name;

	tok = expect_ident();
	func_name = std::get<an_ident>(tok.value);
	
	check_decl(func_name);

	if ( get_sym_stype(func_name) != a_structural_type_kind::function )
	{
		std::cerr<<func_name<<"can't be called\n";
		exit(1);
	}

	if( std::get<a_function_state>(get_sym_state(func_name)) != a_function_state::state_called)
	{
		update_symbol_state(func_name , a_function_state::state_called);
	}
	
	expect_lparen();
	args = expr(0);
	expect_rparen();

	return mk_unary_node(an_ast_node_kind::node_func_call , args ,find_symbol(func_name), get_sym_type(func_name));
}
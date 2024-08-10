#include "frontend/tree.h"
#include "frontend/lex.h"
#include "frontend/symbol_tbl.h"
#include "frontend/types.h"
#include <iostream>
#include <string>

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

/*
an_ast_node* term()
{
	an_ast_node *left_operand , *right_operand;
	a_token_kind tok_kind;
	
	left_operand  = primary();
	
	if(curr_token.kind  == a_token_kind::tok_eof)
		return left_operand;	
		
	while(curr_token.kind == a_token_kind::tok_mul || curr_token.kind == a_token_kind::tok_div)
		{
			
			get_token();
			right_operand = primary();
			left_operand  = mk_node(tok_to_node(curr_token.kind),left_operand,right_operand,std::monostate{});
		        
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
	
	//  parse the expr ( left subtree) 
	left_expr = term();
	
	// if last token, return
	while ( curr_token.kind != a_token_kind::tok_eof )
	{
		
	expr_kind = tok_to_node(curr_token.kind);
	get_token();
	
	//  this will only return when it hits with lower precedence  with higher precedence already formed
	right_term = term();
        
	// add it to the left expression 
	left_expr  = mk_node(expr_kind,left_expr,right_term,std::monostate{});
	
	}
	return left_expr;

}
*/
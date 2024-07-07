#include <iostream>
#include "frontend/lex.h"
#include "frontend/tree.h"
#include "frontend/expr.h"
#include "common/globals.h"
#include "codegen/gen.h"

/*
 ***** Grammar Start *****

statements:	statement
		| statement statements
		;
		
statement:	"print" statement ";"
		|	 variable_declaration
		|	 assignment_expression
		;

identifer:	under_or_alpha
		|	identifier under_or_alpha
		|	identifier digit

under_or_alpha:	"_"
		|	"[a-z]"
		|	"[A-Z]"
	
digit:	"[0-9]"

 ***** Grammar End *****
 */


void print_statement()
{
	a_token tok;
	an_ast_node*  ast;
	int reg;

	expect_print();
	ast = expr(0);
	reg = gen_AST(ast);
	gen_printint(reg);
	gen_free_regs();
	expect_semi();

}

void statements()
{
	while(true)
	{
		switch (curr_token.kind)
		{
			case a_token_kind::tok_print:
				print_statement();
			break;

			case a_token_kind::tok_int:
				variable_declaration();
			break;

			case a_token_kind::tok_ident:
				assign_statement();
			break;

			case a_token_kind::tok_eof:
				return;
			break;

			default:
				std::cerr<<"Expected a statement\n";
				exit(1);
		}
	}
}

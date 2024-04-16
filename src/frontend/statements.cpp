#include <iostream>
#include "frontend/lex.h"
#include "frontend/tree.h"
#include "common/globals.h"
#include "codegen/gen.h"

/*
 ***** Grammar Start *****

statements:	statement
		| statement statements
		;
		
statement:	"print" expression ";"
		;

 ***** Grammar End *****
 */


extern a_ast_node* expr();
extern a_ast_node* expr(int);

void statements()
{
	a_token tok;
	a_ast_node*  ast;
	int reg;

	while(1)
	{
		if ( ! isprint_token(get_token()) )
		{
			std::cerr<<"Expression should start with \"print\"\n";
			exit(1);
		}
		next_token();	
		ast = expr(0);
		reg = gen_AST(ast);
		gen_printint(reg);
		gen_free_regs();
		if ( ! issemi_token(get_token()) )
		{
			std::cerr<<"Statement should end with \";\"\n";
			exit(1);
		}
		else
		{
			next_token();
		}
		if ( iseof_token(get_token()) )
			return;
	}
}

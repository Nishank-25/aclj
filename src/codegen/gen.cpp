#include "frontend/tree.h"
#include "frontend/lex.h"
#include <iostream>


/* Todo: Common you can do better */
void prologue();
void epilogue();
void cgprintint(int);
int  cgload(int);
int  cgadd(int , int);
int  cgsub(int , int);
int  cgmul(int , int);
int  cgdiv(int , int);

static int genAST(a_ast_node *n)
{
	int leftreg, rightreg;
	// Get the left and right sub-tree values
	if (n->left)
		leftreg = genAST(n->left);
	if (n->right)
		rightreg = genAST(n->right);

	switch (n->op) {
		case a_ast_node_kind::node_add:
			return (cgadd(leftreg,rightreg));
		case a_ast_node_kind::node_sub:
			return (cgsub(leftreg,rightreg));
		case a_ast_node_kind::node_mul:
			return (cgmul(leftreg,rightreg));
		case a_ast_node_kind::node_div:
			return (cgdiv(leftreg,rightreg));
		case a_ast_node_kind::node_int_literal:
			return (cgload(get<int>(n->value)));
		default:
			std::cerr<< "Unknown AST operator\n ";
			exit(1);
	}
}

void generatecode(a_ast_node *n)
{
	int reg;
	prologue();
	reg = genAST(n);
	cgprintint(reg);
	epilogue();
}

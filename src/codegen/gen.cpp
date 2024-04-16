#include "frontend/tree.h"
#include "frontend/lex.h"
#include "codegen/gen.h"
#include <iostream>


int gen_AST(a_ast_node *n)
{
	int leftreg, rightreg;
	// Get the left and right sub-tree values
	if (n->left)
		leftreg = gen_AST(n->left);
	if (n->right)
		rightreg = gen_AST(n->right);

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
			return (cgload(std::get<int>(std::get<a_number>(n->value))));
		default:
			std::cerr<< "Unknown AST operator\n ";
			exit(1);
	}
}

#include "frontend/tree.h"
#include "frontend/lex.h"
#include "codegen/gen.h"
#include <iostream>


int gen_AST(an_ast_node *n , int reg)
{
	int leftreg, rightreg;
	// Get the left and right sub-tree values
	if (n->left != nullptr)
		leftreg = gen_AST(n->left , -1);
	if (n->right != nullptr)
		rightreg = gen_AST(n->right , leftreg);

	switch (n->op) {
		case an_ast_node_kind::node_add:
			return (cgadd(leftreg,rightreg));
		case an_ast_node_kind::node_sub:
			return (cgsub(leftreg,rightreg));
		case an_ast_node_kind::node_mul:
			return (cgmul(leftreg,rightreg));
		case an_ast_node_kind::node_div:
			return (cgdiv(leftreg,rightreg));
		case an_ast_node_kind::node_int_literal:
			return (cgload(std::get<int>(std::get<a_number>(n->value))));
		case an_ast_node_kind::node_
		default:
			std::cerr<< "Unknown AST operator\n ";
			exit(1);
	}
}

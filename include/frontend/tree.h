#ifndef TREE_H
#define TREE_H

#include "frontend/lex.h"


// AST NODE KINDS
enum class a_ast_node_kind{
	node_add, // Operation add
	node_sub, // Operation substract
	node_mul, // Operation multiply
	node_div, // Operation divide 
	node_int_literal,
	node_float_literal
};

using a_ast_value = variable_values;

// AST Structure 
struct a_ast_node{

	a_ast_node_kind op; // Operation 
	a_ast_node* left;   // Left child tree
	a_ast_node* right;  // Right child tree
	a_ast_value value;  // integer or float value
};

extern a_ast_node* mk_node(a_ast_node_kind op , a_ast_node* left , a_ast_node* right , a_ast_value value);
extern a_ast_node* mk_leaf_node(a_ast_node_kind op , a_ast_value);
extern a_ast_node* mk_unary_node(a_ast_node_kind op, a_ast_node* left, a_ast_value value);
#endif /* TREE_H */

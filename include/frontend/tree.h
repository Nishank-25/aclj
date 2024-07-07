#ifndef TREE_H
#define TREE_H

#include "frontend/lex.h"
#include "frontend/symbol_tbl.h"
#include <variant>

// AST NODE KINDS
enum class an_ast_node_kind{
	node_add, // Operation add
	node_sub, // Operation substract
	node_mul, // Operation multiply
	node_div, // Operation divide 
	node_int_literal,
	node_float_literal,
	node_lvalue,
	node_ident,
	node_assign
};

using an_ast_value = std::variant<empty,a_number,a_symbtable_index>

// AST Structure 
struct an_ast_node{

	an_ast_node_kind op; // Operation 
	an_ast_node* left;   // Left child tree
	an_ast_node* right;  // Right child tree
	an_ast_value value;  // integer or float value
};

extern an_ast_node* mk_node(an_ast_node_kind op , an_ast_node* left , an_ast_node* right , an_ast_value value);
extern an_ast_node* mk_leaf_node(an_ast_node_kind op , an_ast_value);
extern an_ast_node* mk_unary_node(an_ast_node_kind op, an_ast_node* left, an_ast_value value);
#endif /* TREE_H */

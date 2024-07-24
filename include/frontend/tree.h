#ifndef TREE_H
#define TREE_H

#include "frontend/lex.h"
#include "frontend/symbol_tbl.h"
#include <variant>

// AST NODE KINDS
enum class an_ast_node_kind{
	node_eq_eq = 1,node_neq,
	node_lt,node_gt,node_le,node_ge,
	node_add,node_sub,
	node_mul,node_div,
	node_assign,
	node_int_literal,
	node_float_literal,
	node_ident,
	node_semicolon,
	node_print,
	node_lvalue,
	node_glue,
	node_if,
	node_while,
	node_do_while,
	node_unknown
};
typedef std::monostate void_ast_type;

using an_ast_value = std::variant<void_ast_type,a_number,a_symtable_index>;

// AST Structure 
struct an_ast_node{

	an_ast_node_kind op; // Operation 
	an_ast_node* left;   // Left child tree
	an_ast_node* right;  // Right child tree
	an_ast_node* mid;	 // middle child tree
	an_ast_value value;  // integer or float value or symtbl index
	bool should_gen = 1;
};

extern an_ast_node* mk_node(an_ast_node_kind, an_ast_node*, an_ast_node*, an_ast_value );
extern an_ast_node* mk_node(an_ast_node_kind, an_ast_node*, an_ast_node*, an_ast_node*, an_ast_value);
extern an_ast_node* mk_leaf_node(an_ast_node_kind , an_ast_value);
extern an_ast_node* mk_unary_node(an_ast_node_kind , an_ast_node*, an_ast_value);
extern bool 		is_comparison_node(an_ast_node_kind);
extern bool 		is_jump_node(an_ast_node_kind);
#endif /* TREE_H */

#include "frontend/tree.h"
#include <iostream>
an_ast_node* mk_node(an_ast_node_kind op , an_ast_node* left , an_ast_node* right , an_ast_value value)
{
	an_ast_node*  node;

	try{
		node = (an_ast_node*) new(an_ast_node);
	}
	catch(std::exception &e) {
		std::cerr << e.what() << "\n";
		exit(1);
	}

	node->op = op;
	node->left = left;
	node->right = right;
	node->value = value;
	node->mid = nullptr;
	return node;
}
an_ast_node* mk_node(an_ast_node_kind op , an_ast_node* left , an_ast_node* right , an_ast_node* middle, an_ast_value value)
{
	an_ast_node* node;
	node = mk_node(op,left,right,value);

	node->mid = middle;
	return node;
}
an_ast_node* mk_leaf_node(an_ast_node_kind op , an_ast_value value) {
	return mk_node(op,nullptr,nullptr,value);
}

an_ast_node* mk_unary_node(an_ast_node_kind op, an_ast_node* left, an_ast_value value){
	return mk_node(op,left,nullptr,value);
}

bool is_comparison_node(an_ast_node_kind kind)
{
	if ( an_ast_node_kind::node_eq_eq <= kind && kind <= an_ast_node_kind::node_ge) return true;
	else return false;
}

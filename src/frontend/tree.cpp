#include "frontend/tree.h"
#include <iostream>
a_ast_node* mk_node(a_ast_node_kind op , a_ast_node* left , a_ast_node* right , a_ast_value value)
{
	a_ast_node*  node;

	try{
		node = (a_ast_node*) new(a_ast_node);
	}
	catch(std::exception &e) {
		std::cerr << e.what() << "\n";
		exit(1);
	}

	node->op = op;
	node->left = left;
	node->right = right;
	node->value = value;
	return node;
}

a_ast_node* mk_leaf_node(a_ast_node_kind op , a_ast_value value) {
	return mk_node(op,nullptr,nullptr,value);
}

a_ast_node* mk_unary_node(a_ast_node_kind op, a_ast_node* left, a_ast_value value){
	return mk_node(op,left,nullptr,value);
}



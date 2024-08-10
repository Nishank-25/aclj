#include "frontend/tree.h"
#include <iostream>

an_ast_node* mk_node(an_ast_node_kind op , an_ast_node* left , an_ast_node* right , an_ast_node* middle , an_ast_value value , a_type_kind type_kind)
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
	node->mid = middle;
	node->type_kind = type_kind;
	return node;
}

an_ast_node* mk_node(an_ast_node_kind op , an_ast_node* left , an_ast_node* right , an_ast_value value, a_type_kind type_kind)
{
	an_ast_node* node;
	node = mk_node(op,left,right,nullptr , value,type_kind);

	return node;
}


an_ast_node* mk_leaf_node(an_ast_node_kind op , an_ast_value value, a_type_kind type)
{
	an_ast_node* node;
	node = mk_node(op,nullptr,nullptr,nullptr,value,type);
	return node;
}


an_ast_node* mk_unary_node(an_ast_node_kind op, an_ast_node* node , an_ast_value value , a_type_kind type)
{
	an_ast_node* Node;
	Node = mk_node(op,node,nullptr,nullptr,value, type);
	
	return Node;
}

bool is_comparison_node(an_ast_node_kind kind)
{
	if ( an_ast_node_kind::node_eq_eq <= kind && kind <= an_ast_node_kind::node_ge) return true;
	else return false;
}

bool is_jump_node(an_ast_node_kind kind)
{
	if ( an_ast_node_kind::node_if <= kind && kind <= an_ast_node_kind::node_do_while ) return true;
	else return false;
}
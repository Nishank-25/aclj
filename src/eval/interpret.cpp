#include "frontend/tree.h"
#include <iostream>

a_number operator+(a_number left , a_number right)
{
	if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
		return std::get<double>(left) + std::get<double>(right);

	if(std::holds_alternative<int>(left)  && std::holds_alternative<int>(right))
		return std::get<int>(left) + std::get<int>(right);

	if(std::holds_alternative<int>(left) && std::holds_alternative<double>(right))
		return (static_cast<double>(std::get<int>(left))  + std::get<double>(right));

	if(std::holds_alternative<double>(left) && std::holds_alternative<int>(right))
		return (std::get<double>(left) + static_cast<double>(std::get<int>(right)));
}

a_number operator-(a_number left , a_number right)
{
        if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                return std::get<double>(left) - std::get<double>(right);

        if(std::holds_alternative<int>(left)  && std::holds_alternative<int>(right))
                return std::get<int>(left) - std::get<int>(right);

        if(std::holds_alternative<int>(left) && std::holds_alternative<double>(right))
                return (static_cast<double>(std::get<int>(left))  - std::get<double>(right));

        if(std::holds_alternative<double>(left) && std::holds_alternative<int>(right))
                return (std::get<double>(left) - static_cast<double>(std::get<int>(right)));
}

a_number operator*(a_number left , a_number right)
{
        if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                return std::get<double>(left) * std::get<double>(right);

        if(std::holds_alternative<int>(left)  && std::holds_alternative<int>(right))
                return std::get<int>(left) * std::get<int>(right);

        if(std::holds_alternative<int>(left) && std::holds_alternative<double>(right))
                return (static_cast<double>(std::get<int>(left))  * std::get<double>(right));

        if(std::holds_alternative<double>(left) && std::holds_alternative<int>(right))
                return (std::get<double>(left) * static_cast<double>(std::get<int>(right)));
}

a_number operator/(a_number left , a_number right)
{
        if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                return std::get<double>(left) / std::get<double>(right);

        if(std::holds_alternative<int>(left)  && std::holds_alternative<int>(right))
		return ( static_cast<double>(std::get<int>(left)) / static_cast<double>(std::get<int>(right)) );

        if(std::holds_alternative<int>(left) && std::holds_alternative<double>(right))
                return (static_cast<double>(std::get<int>(left))  / std::get<double>(right));

        if(std::holds_alternative<double>(left) && std::holds_alternative<int>(right))
                return (std::get<double>(left) / static_cast<double>(std::get<int>(right)));
}

a_number interpret_ast(a_ast_node* ast)
{
	a_number leftval , rightval;

	if(ast->left != nullptr)  leftval  = interpret_ast(ast->left);
	if(ast->right != nullptr) rightval = interpret_ast(ast->right);
	
	switch(ast->op)
	{
		case a_ast_node_kind::node_add:
			return (leftval + rightval);
		case a_ast_node_kind::node_sub:
			return (leftval - rightval);
		case a_ast_node_kind::node_mul:
			return (leftval * rightval);
		case a_ast_node_kind::node_div:
			return (leftval / rightval);
		case a_ast_node_kind::node_int_literal:
		case a_ast_node_kind::node_float_literal:
			return std::get<a_number>(ast->value);
		default:
			std::cerr<<"Unkown operator\n";
			exit(1);
	}
}


#include "util/helper.h"
#include "frontend/lex.h"
#include "codegen/gen.h"
#include <iomanip>
#include <iostream>

int print[5];
std::string input_file;

const char *tok_str(a_token_kind kind )
{
	switch(kind)
	{
		case a_token_kind::tok_plus 			: return "+";
        case a_token_kind::tok_minus			: return "-";
        case a_token_kind::tok_mul  			: return "*";
        case a_token_kind::tok_div  			: return "/";
        case a_token_kind::tok_int_literal		: return "integer literal";
		case a_token_kind::tok_float_literal	: return "float literal";
		case a_token_kind::tok_ident			: return "identifier";
		case a_token_kind::tok_eq				: return "=";
		case a_token_kind::tok_semicolon		: return ";";
		case a_token_kind::tok_eof				: return "eof";
		case a_token_kind::tok_eq_eq			: return "==";
		case a_token_kind::tok_neq				: return "!=";
		case a_token_kind::tok_lt				: return "<";
		case a_token_kind::tok_gt				: return ">";
		case a_token_kind::tok_le				: return "<=";
		case a_token_kind::tok_ge				: return ">=";
		case a_token_kind::tok_print			: return "print";
		case a_token_kind::tok_int				: return "int";
		case a_token_kind::tok_lbrace			: return "{";
		case a_token_kind::tok_rbrace			: return "}";
		case a_token_kind::tok_rparen			: return ")";
		case a_token_kind::tok_lparen			: return "(";
		case a_token_kind::tok_if				: return "if";
		case a_token_kind::tok_else				: return "else";
		default : return "unknown";
	}
}
const char* get_token_name(a_token_kind kind)
{
	return tok_str(kind);
}

void print_tokens()
{
        a_token tok;
	for(auto tok : Tokens)
        {
                std::cout<<"Token: "<<"{"<< tok_str(tok.kind)<<"}";
                if ( tok.kind == a_token_kind::tok_int_literal)
                {
			std::cout<<", value : ";
                        std::cout<<std::get<int>(std::get<a_number>((tok.value)));
                }
                if ( tok.kind == a_token_kind::tok_float_literal)
                {
                        std::cout<<", value : ";
			std::cout<<std::get<double>(std::get<a_number>((tok.value)));
                }
                std::cout<<"\n";

        }
	std::cout<<"\n";
}

static const char *op_str[] = { "+", "-", "*" , "/"};

int left_spaces =  14;
int edge_spaces =  left_spaces + 1;
int parent_space = left_spaces + 2;
bool parent_node = true;

std::string spaces(int num) { return std::string(num,' '); } 


int print_ast(an_ast_node* ast)
{
	// last right child (base case)
	if (ast->left  == nullptr || ast->right == nullptr) 
	{
		if(ast->op == an_ast_node_kind::node_int_literal)
 		std::cout<<spaces(3)<<std::get<int>(std::get<a_number>(ast->value))<<"\n";
		if(ast->op == an_ast_node_kind::node_float_literal)
                std::cout<<spaces(3)<<std::get<double>(std::get<a_number>(ast->value))<<"\n";
		std::cout<<"\n";
	       	return 0; 
	
	}

	// print parent node or right child
 	int num_of_digits = 1;
	if(ast->left->op == an_ast_node_kind::node_int_literal)
		num_of_digits = std::to_string(std::get<int>(std::get<a_number>((ast->left->value)))).size();
	if(ast->left->op == an_ast_node_kind::node_float_literal)
	{
		std::string num = std::to_string(std::get<double>(std::get<a_number>((ast->left->value))));
		std::size_t size = num.size();
		std::size_t i = 0;
		while (num[i] != '.') { ++i; }
		while ( num[i] != '0' && i < size ) { ++i; }
		num_of_digits = i;
	}
	if ( parent_node )
	{
		std::cout<<spaces(parent_space)<<op_str[(int)ast->op]<<"\n";
		parent_node = false;
	}
	else { 
		std::cout<<spaces(3)<<op_str[(int)ast->op]<<"\n";
	}

	std::cout<<spaces(edge_spaces)<<"/"<<" "<<"\\"<<"\n";
	edge_spaces += 2;
	
	// print left child
	std::cout<<std::setprecision(10);
	if(ast->left->op == an_ast_node_kind::node_int_literal) 
		std::cout<<spaces(left_spaces - num_of_digits + 1)<<std::get<int>(std::get<a_number>((ast->left->value)));
	if(ast->left->op == an_ast_node_kind::node_float_literal)
		std::cout<<spaces(left_spaces - num_of_digits + 1)<<std::get<double>(std::get<a_number>((ast->left->value)));
	left_spaces += 2;
	
	// print right sub tree
	return print_ast(ast->right);
}

static void scan_cmd_print(std::string print_flag)
{
	std::string flags{"-Ptokens"};
	if( flags == print_flag ) { print[PRINT_TOKENS] = 1;} 
	
	flags = "-Past-without-precedence";
	if( flags == print_flag ) { print[PRINT_AST_WITHOUT_PRECEDENCE] = 1; }

}
void scan_cmd_line(int argc , char const* argv[] )
{
	if ( argc < 2) { std::cerr<<"No args??\n"; exit(1); }
	
	int args = argc;
	std::vector<std::string>cmd_line;	
	// look for "-", the next character decides what to scan
	
	for(std::size_t i = 1; i <= args - 1; ++i)
	{
		cmd_line.push_back(argv[i]);
	}
	
	for(auto flag : cmd_line)
	{
		if ( flag[0] == '-' )
		{
			switch(flag[1])
			{
				case 'P':
					scan_cmd_print(flag);
					break;
				case 'O':
					optim = true;
					break;
				default :
					std::cerr<<"Unrecognized flag\n";
			}
		}
		else if ( flag.find("my") !=  std::string::npos)
		{
			input_file = flag;
		}
		else
		{ 
			std::cerr<<"Unkown flag:"<<flag<<"\n";
		}
	}
}

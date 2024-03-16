#include "util/helper.h"
#include <iomanip>
#include <iostream>

int print[5];
std::string input_file;

const char *tok_str[] = { "+", "-" , "*" , "/" , "int literal" , "float literal" ,"End of File", "unknown token"};
void print_tokens()
{
        a_token tok;
	for(auto tok : Tokens)
        {
                std::cout<<"Token: "<<"{"<< tok_str[(int)tok.kind]<<"}";
                if ( tok.kind == a_token_kind::tok_int_literal)
                {
			std::cout<<", value : ";
                        std::cout<<std::get<int>(tok.value);
                }
                if ( tok.kind == a_token_kind::tok_float_literal)
                {
                        std::cout<<", value : ";
			std::cout<<std::get<double>(tok.value);
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


int print_ast(a_ast_node* ast)
{
	// last right child (base case)
	if (ast->left  == nullptr || ast->right == nullptr) 
	{
		if(std::holds_alternative<int>(ast->value))
 		std::cout<<spaces(3)<<std::get<int>(ast->value)<<"\n";
		if(std::holds_alternative<double>(ast->value))
                std::cout<<spaces(3)<<std::get<double>(ast->value)<<"\n";
		std::cout<<"\n";
	       	return 0; 
	
	}

	// print parent node or right child
 	int num_of_digits = 1;
	if(std::holds_alternative<int>(ast->left->value))
		num_of_digits = std::to_string(std::get<int>(ast->left->value)).size();
	if(std::holds_alternative<double>(ast->left->value))
	{
		std::string num = std::to_string(std::get<double>(ast->left->value));
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
	if(std::holds_alternative<int>(ast->left->value)) 
		std::cout<<spaces(left_spaces - num_of_digits + 1)<<std::get<int>(ast->left->value);
	if(std::holds_alternative<double>(ast->left->value))
		std::cout<<spaces(left_spaces - num_of_digits + 1)<<std::get<double>(ast->left->value);
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

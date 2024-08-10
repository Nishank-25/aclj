#include <iostream>
#include <fstream>
#include "frontend/lex.h"
#include "frontend/tree.h"
#include "util/helper.h"
#include "codegen/gen.h"
#include "frontend/decl.h"

/***** GLOBALS *****/ 

extern a_number interpret_ast(an_ast_node*);

std::ofstream asm_file{"out.s"};

/***** GLOBALS *****/ 

// for reading file into std::vector. This will be used as input for lexical scanner 

inline std::vector<char> read_vector_from_disk(std::string file_path)
{
   // Task 1.1 : Read from the file
        std::ifstream source_file(file_path, std::ios::in | std::ios::binary);
	
   // Task 1.2 : Scan the source and save it
	if(source_file.good())
	{
		std::vector<char> data((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());
		if ( source_file.get() == EOF ) { data.push_back(EOF); } 
		return data;
	}
	else
	{
		std::cerr<<"Input file not good";
		exit(0);
	}
}


int main(int argc, char const *argv[])
{
//	init();
	a_token tok;
	an_ast_node* tree;
	scan_cmd_line(argc,argv);
	/* Todo: where should input_file come from. Hmm... util??  */
	if(input_file.empty()) { std::cerr<<"Whom do I compile??\n"; exit(1);} 
	source_code = read_vector_from_disk(input_file);
	
	// Tokenize 
	while(scan(&tok))
	{
		Tokens.push_back(tok);
	}
	
	if(tok.kind == a_token_kind::tok_eof) { Tokens.push_back(tok); }	
	
	gen_prologue();
	get_token();
	while(1)
	{
		tree = function_declaration();
		gen_AST(tree,NOREG,tree->op);
		if (curr_token.kind == a_token_kind::tok_eof)
			break;
	}
	asm_file.close();

/*	// interpret ( understand the ast)
	auto result = interpret_ast(ast);
	
	if(std::holds_alternative<int>(result))
		std::cout<<"Result: "<<std::get<int>(result)<<"\n\n";
	if(std::holds_alternative<double>(result))
		std::cout<<"Result: "<<std::get<double>(result)<<"\n\n";
	
	if( print[PRINT_TOKENS] == 1 ) { std::cout<<"Tokens\n"; print_tokens(); }
	if( print[PRINT_AST_WITHOUT_PRECEDENCE] == 1 ) { print_ast(ast); }
*/
        return 0;
}

#include <iostream>
#include <fstream>
#include "lex.h"

/***** GLOBALS *****/ 

extern bool scan(a_arith_token*);
std::vector<char> source_code;
size_t curr_pos = 0;
char cache;

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
		std::cerr<<"Not available";
		exit(0);
	}
}

char *tok_str[] = { "+", "-" , "*" , "/" , "int literal" , "float literal" , "unknown token"};

static inline void scan_file()
{
	a_arith_token tok;
	while(scan(&tok))
	{
		std::cout<<"Token: "<<"{"<< tok_str[(int)tok.kind]<<"}";
		std::cout<<", value : ";
		if ( tok.kind == a_arith_kind::arith_int_literal)
		{
			std::cout<<std::get<int>(tok.literal_value);
		}
		if ( tok.kind == a_arith_kind::arith_float_literal)
		{
			std::cout<<std::get<double>(tok.literal_value);
		}
		std::cout<<"\n";

	}

}


int main(int argc, char const *argv[])
{
//	init();
        source_code = read_vector_from_disk(argv[1]);
	scan_file();
        
}

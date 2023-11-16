#include <iostream>
#include <fstream>
#include <vector>

enum class a_lexeme_kind { 

	lex_fn,
	lex_identifier,
	lex_takes,
	lex_returns,
	lex_int,
	lex_lparen,
	lex_rparen,
};
const char * lexeme_kinds[] = {"lex_fn", "lex_identifier" , "lex_takes", "lex_returns" , "lex_int" , "lex_paren" , "lex_rparen"};

class a_token { 

public: 
	std::string lexeme;
	a_lexeme_kind kind;

};


a_lexeme_kind get_lexeme_kind(const std::string lexeme)
{
	if ( lexeme == "fn" ) return a_lexeme_kind::lex_fn;
	if ( lexeme == "takes") return a_lexeme_kind::lex_takes;
	if ( lexeme == "returns") return a_lexeme_kind::lex_returns;
	if ( lexeme == "int") return a_lexeme_kind::lex_int;
	if ( lexeme == "{") return a_lexeme_kind::lex_lparen;
	if ( lexeme == "}") return a_lexeme_kind::lex_rparen;
	return a_lexeme_kind::lex_identifier;
}

std::vector<a_token> scan_source_code(const std::vector<uint8_t> source_code)
{
	std::vector<a_token> tokens;
	size_t source_size = source_code.size();
	if( source_size <= 0) { std::cerr << "Source is empty\n"; exit(0); }  
	std::string temp;
	a_token tok;
	for(size_t i = 0; i < source_size; ++i)
	{
		std::cout<<i;
		if ( source_code[i] == '/' && source_code[i+1] == '*' )
		{
			do
			{ 
				++i;
			}	
			while (source_code[i] != '/');
			++i; // \n
			
		}
		if ( source_code[i] ==  ' ' || source_code[i] ==  '\n' )
		{
			tok.lexeme = temp;
			tok.kind   = get_lexeme_kind(temp);
			tokens.push_back(tok);
		}
		else 
		{
			temp[i] = source_code[i];
		}
	}	
	return tokens;
}

inline std::vector<uint8_t> read_vector_from_disk(std::string file_path)
{
   // Task 1.1 : Read from the file
        std::ifstream source_file(file_path, std::ios::in | std::ios::binary);
	
   // Task 1.2 : Scan the source and save it
	if(source_file.good())
	{
		std::vector<uint8_t> data((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());
		return data;
	}
	else
	{
		std::cerr<<"Not available";
		exit(0);
	}
}


int main(int argc, char const *argv[])
{
   
        std::vector<uint8_t> source_code = read_vector_from_disk("test/test.my");

        
   // Task 1.4 : Tokenize and print tokens
	std::vector<a_token> tokens = scan_source_code(source_code);
/*	for(a_token tok : tokens)
		std::cout<<"<"<<tok.lexeme<<","<<lexeme_kinds[(int)tok.kind]<<">"<<"\n";
   // Task 1.3 : Print the program as it was in the file
	for(char ch : source_code)
		std::cout<< ch;
*/
       std::cout<<"\nNo of chars : "<<source_code.size();
}

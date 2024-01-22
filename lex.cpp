#include "lex.h"
#include <cctype>
#include <string>
#include <iostream>

std::vector<char> source_code;
std::size_t curr_source_pos = 0;
std::vector<a_token> Tokens;
std::size_t curr_token_pos =  0;
char cache;
// for reading next character. read one at a time. will also cache

static char next()
{

	if (cache)
	{ 
		char temp = cache;
		cache = 0;
		return temp;
	}
	
	return source_code[curr_source_pos++];  	
}

// for caching  character. 

static void cache_char(char pb)
{
	cache = pb;
}

// for skipping whitespace character.

static int skip(void)
{
	char c;

	c = next();
	while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c)
	{
		c = next();
	}
	return (c);
}

// for converting contigous number literals to an int or float
static a_number scan_number(char c)
{
	char number[10];
	a_number value;
	char digit = c;
	bool is_floating_point = false;
	for(size_t i = 0;std::isdigit(digit) || digit == '.' ; ++i)
	{ 
		number[i] = digit;
		if (digit == '.') { is_floating_point = true;}
		digit = next();
	}
	cache_char(digit);

	if ( is_floating_point) { value = std::stod(std::string(number)) ; } 
	else { value = std::stoi(std::string(number)); }
	
	return value;

}

// for scanning the tokens

bool scan(a_token *tok)
{
	char c;
	c = skip();
	switch (c)
	{
		case EOF :
				tok->kind = a_token_kind::tok_eof;
				tok->value = std::monostate{};
				return false;
		case '+' :
				tok->kind = a_token_kind::tok_plus;
				tok->value = std::monostate{};
				break;
 		case '-' :
				tok->kind = a_token_kind::tok_minus;
				tok->value = std::monostate{};
				break;
		case '*' :
				tok->kind = a_token_kind::tok_mul;
				tok->value = std::monostate{};
				break;
		case '/' :
				tok->kind = a_token_kind::tok_div;
				tok->value = std::monostate{};
				break;
		
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9':
			{
				tok->value = scan_number(c);
				if(std::holds_alternative<double>(tok->value)) 
				{ 
					tok->kind = a_token_kind::tok_float_literal;
					break;
				}
				if(std::holds_alternative<int>(tok->value))
				{
					tok->kind = a_token_kind::tok_int_literal;
					break;
				}
			}
		default :
			std::cerr<<"I don't know how '"<<c<<"' fits here\n";
			exit(1);
	}	
	return true;
}

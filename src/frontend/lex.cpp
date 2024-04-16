#include "frontend/lex.h"
#include <cctype>
#include <string>
#include <iostream>

std::vector<char> source_code;
std::size_t curr_source_pos = 0;
std::vector<a_token> Tokens;
std::size_t curr_token_pos =  0;
a_token curr_token;
char cache;


//
a_token get_token()
{
	return Tokens[curr_token_pos];
}

void next_token()
{
	++curr_token_pos;
	curr_token = Tokens[curr_token_pos];
}

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

#define is_whitespace(c) (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c ) 

// for converting contigous number literals to an int or float
static a_number scan_number()
{
	char number[10];
	a_number value;
	char digit = next();
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

// for now we only have "print" which is a keyword
// print arithematic expression
// ex : print 1 + 2
static void scan_identifier( an_ident& id )
{
	char c = next();
	char buf[6];
	size_t i = 0;

	// combination of alphabets, digits, and underscore
	for(; std::isalnum(c) || c == '_'; ++i ) 
	{
		if( i > ( MAX_IDENT_LEN - 1) )
		{
			std::cerr<<"Identifier too long\n";
			exit(1);
		}
		else
		{
			buf[i] = c;
			c = next();
		}
	}
	buf[++i] = '\0';	
	cache_char(c);
	id = std::string(buf);
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
				cache_char(c);
				a_number num = scan_number();
				tok->value = num;
			        if ( !is_whitespace(next()) )
				{
					std::cerr<< "Not a number literal\n";
					exit(1);
				}
				
				if(std::holds_alternative<double>(num)) 
				{ 
					tok->kind = a_token_kind::tok_float_literal;
					break;
				}
				if(std::holds_alternative<int>(num))
				{
					tok->kind = a_token_kind::tok_int_literal;
					break;
				}
			}
		case ';':
				tok->kind = a_token_kind::tok_semicolon;
				tok->value = std::monostate{};
				break;
		default :
			{
				if(std::isalpha(c) || c == '_')
				{
					an_ident id;
					cache_char(c);
					scan_identifier(id);
					if( id == std::string{"print"}  )
					{
						tok->kind  = a_token_kind::tok_print;
						tok->value = std::monostate{}; 
						break;
					}
					else
					{
						std::cerr<<"Unrecognized symbol: "<<id<<" \n Be patient I will add you favourite keywords\n";
						exit(1);

					}
				}
				else
				{
					std::cerr<<"I don't know how '"<<c<<"' fits here\n";
					exit(1);
				}
			}
	}	
	return true;
}

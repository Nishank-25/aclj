#include "lex.h"
#include <cctype>
#include <string>

// for reading next character. read one at a time. will also cache

static char next()
{

	if (cache)
	{ 
		char temp = cache;
		cache = 0;
		return temp;
	}
	
	return source_code[curr_pos++];  	
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

bool scan(a_arith_token *tok)
{
	char c;
	c = skip();
	switch (c)
	{
		case EOF :
				return false;
		case '+' :
				tok->kind = a_arith_kind::arith_plus;
				break;
 		case '-' :
				tok->kind = a_arith_kind::arith_minus;
				break;
		case '*' :
				tok->kind = a_arith_kind::arith_mul;
				break;
		case '/' :
				tok->kind = a_arith_kind::arith_div;
				break;
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9':
			{
				tok->literal_value = scan_number(c);
				if(std::holds_alternative<double>(tok->literal_value)) 
				{ 
					tok->kind = a_arith_kind::arith_float_literal;
					break;
				}
				if(std::holds_alternative<int>(tok->literal_value))
				{
					tok->kind = a_arith_kind::arith_int_literal;
					break;
				}
			}
		default :
			tok->kind = a_arith_kind::arith_unknown;
	}	
	return true;
}

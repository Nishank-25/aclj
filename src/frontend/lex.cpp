#include <cctype>
#include <cstring>
#include <iostream>
#include "frontend/lex.h"
#include "common/globals.h"


std::vector<char> source_code;
std::size_t curr_source_pos = 0;
std::vector<a_token> Tokens;
std::size_t curr_token_pos =  0;
a_token curr_token;
char cache;


//
a_token get_token()
{
	curr_token = Tokens[curr_token_pos++];
	return curr_token;
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
static an_ident scan_identifier()
{
	char c = next();
	char  buf[MAX_IDENT_LEN];
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
	cache_char(c);
	return an_ident{buf,i};
}

// if its a keyword return the token
static a_token keyword(an_ident id)
{
	char c = id[0];
	a_token tok;
	switch (c)
	{
		case 'i':
				if (id[1] == 'f')
				{
					tok.kind = a_token_kind::tok_if;
					tok.value = void_token{};
					return tok;
				}
				else if (std::strcmp(id.data() ,"int") == 0)
				{
					tok.kind = a_token_kind::tok_int;
					tok.value = void_token{};
					return tok;
				}
		case 'p':
				if (std::strcmp(id.data() ,"print") == 0)
				{
					tok.kind = a_token_kind::tok_print;
					tok.value = void_token{};
					return tok;
				}
		case 'e':
				if(std::strcmp(id.data() , "else") == 0 )
				{
					tok.kind = a_token_kind::tok_else;
					tok.value = void_token{};
					return tok;
				}
	
		default:
				tok.kind = a_token_kind::tok_unknown;
				tok.value = void_token{};
				return tok;
		
	}
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
				tok->value = void_token{};
				return false;
		case '+' :
				tok->kind = a_token_kind::tok_plus;
				tok->value = void_token{};
				break;
 		case '-' :
				tok->kind = a_token_kind::tok_minus;
				tok->value = void_token{};
				break;
		case '*' :
				tok->kind = a_token_kind::tok_mul;
				tok->value = void_token{};
				break;
		case '/' :
				tok->kind = a_token_kind::tok_div;
				tok->value = void_token{};
				break;
		case '}' :
				tok->kind = a_token_kind::tok_rbrace;
				tok->value = void_token{};
				break;
		case '{' :
				tok->kind = a_token_kind::tok_lbrace;
				tok->value = void_token{};
				break;
		case ')' :
				tok->kind = a_token_kind::tok_rparen;
				tok->value = void_token{};
				break;
		case '(' :
				tok->kind = a_token_kind::tok_lparen;
				tok->value = void_token{};
				break;
		case '=' :
				if ((c = next()) == '=' )
					tok->kind = a_token_kind::tok_eq_eq;
				else {
					tok->kind = a_token_kind::tok_eq;
					cache_char(c);
				}
				tok->value = void_token{};
				break;
		case '<' :
				if ((c = next()) == '=' )
					tok->kind = a_token_kind::tok_le;
				else { 
					tok->kind = a_token_kind::tok_lt;
					cache_char(c);
				}
				tok->value = void_token{};
				break;
		case '>' :
				if ((c = next()) == '=')
					tok->kind = a_token_kind::tok_ge;
				else { 
					tok->kind = a_token_kind::tok_gt;
					cache_char(c);
				}
				tok->value = void_token{};
				break;
		case '!' :
				if ((c = next()) == '=')
				{
					tok->kind = a_token_kind::tok_neq;
					tok->value = void_token{};
					break;
				}
				else if(is_whitespace(c)){
					std::cerr<<"unary ! not supported yet\n";
					exit(1);
				}
				else{
					cache_char(c);
					break;
				}
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9':
			{
				cache_char(c);
				a_number num = scan_number();
				tok->value = num;
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
				break;
			}
		case ';':
				tok->kind = a_token_kind::tok_semicolon;
				tok->value = void_token{};
				break;
		default :
			{
				if(std::isalpha(c) || c == '_')
				{
					an_ident id;
					cache_char(c);
					id = scan_identifier();
					*tok = keyword(id);	
					if(tok->kind == a_token_kind::tok_unknown)
					{
						// its a variable name(for now)
						tok->kind = a_token_kind::tok_ident;
						tok->value = id;
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

#ifndef LEX_H
#define LEX_H

#include <vector>
#include <variant>
#include <string>
#include <string>

enum class a_token_kind {
	    tok_eof,
		tok_eq_eq,tok_neq,
		tok_lt,tok_gt,tok_le,tok_ge,
		tok_plus,tok_minus,
		tok_mul,tok_div,
		tok_eq,
        tok_int_literal,
		tok_float_literal,
		tok_ident,
		tok_semicolon,
		tok_print,
		tok_int,
		tok_if,
		tok_else,
		tok_lbrace,
		tok_rbrace,
		tok_lparen,
		tok_rparen,
		tok_while,
		tok_do,
		tok_for,
		tok_unknown
};

typedef std::monostate void_token;

using a_number         = std::variant<int,double>;
using an_ident         = std::string;
using a_token_value    = std::variant<void_token,a_number,an_ident>;

struct a_token {
        a_token_kind     kind;
        a_token_value    value;
};


extern bool scan(a_token*);
extern  a_token  get_token();

extern std::vector<char> source_code;
extern std::size_t curr_source_pos;
extern std::vector<a_token> Tokens;
extern std::size_t curr_token_pos;
extern a_token curr_token;
extern char cache;


#define MAX_IDENT_LEN 50 // max identifier characters

#endif /* LEX_H */

#ifndef LEX_H
#define LEX_H

#include <vector>
#include <variant>
#include <string_view>

enum class a_token_kind {
        tok_plus,
        tok_minus,
        tok_mul,
        tok_div,
        tok_int_literal,
		tok_float_literal,
		tok_print,
		tok_int,
		tok_ident,
		tok_equals,
		tok_semicolon,
		tok_eof,
		tok_unknown
};

using a_number         = std::variant<int,double>;
using a_token_value    = std::variant<std::monostate,a_number,std::string_view>;
using an_ident         = std::string_view;

struct a_token {
        a_token_kind     kind;
        a_token_value    value;
};


extern bool scan(a_token*);
extern  a_token  get_token();
extern  void next_token();

extern std::vector<char> source_code;
extern std::size_t curr_source_pos;
extern std::vector<a_token> Tokens;
extern std::size_t curr_token_pos;
extern a_token curr_token;
extern char cache;


#define MAX_IDENT_LEN 50 // max identifier characters

#endif /* LEX_H */

#ifndef LEX_H
#define LEX_H

#include <vector>
#include <variant>

enum class a_token_kind {
        tok_plus,
        tok_minus,
        tok_mul,
        tok_div,
        tok_int_literal,
	tok_float_literal,
	tok_eof,
	tok_unknown
};

using a_number = std::variant<std::monostate,int,double>;

struct a_token {
        a_token_kind kind;
        a_number     value;
};


extern bool scan(a_token*);

extern std::vector<char> source_code;
extern std::size_t curr_source_pos;
extern std::vector<a_token> Tokens;
extern std::size_t curr_token_pos;
extern char cache;


#endif /* LEX_H */

#ifndef LEX_H
#define LEX_H

#include <vector>
#include <variant>

enum class a_arith_kind {
        arith_plus,
        arith_minus,
        arith_mul,
        arith_div,
        arith_int_literal,
	arith_float_literal,
	arith_unknown
};

using a_number = std::variant<int,double>;

class a_arith_token {
public:
        a_arith_kind kind;
        a_number     literal_value;
};

extern std::vector<char> source_code;
extern std::size_t curr_pos;
extern char cache;

#endif /* LEX_H */

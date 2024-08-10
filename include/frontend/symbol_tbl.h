#ifndef SYMBOL_TBL_H
#define SYMBOL_TBL_H

#include <string>
#include <variant>

#include "common/globals.h"
#include "frontend/types.h"

enum class a_variable_state
{
    state_declared,
    state_initialised,
    state_assigned,
    state_referenced // used
};

enum class a_function_state
{
    state_declared,
    state_defined,
    state_called // used
};

typedef std::monostate empty_state;
using a_symbol_state = std::variant<empty_state,a_variable_state, a_function_state>;

struct a_symbol { 
    std::string name;
    a_symbol_state state; 
    a_type_kind type;
    a_structural_type_kind stype;
};

#define MAX_ENTRIES 1024
typedef int a_symtable_index;
#define SYMBOL_NOT_FOUND -1
#define NO_EMPTY_SLOT -1

extern a_symtable_index find_symbol( a_symbol& );
extern a_symtable_index find_symbol(std::string);
extern a_symtable_index add_sym(const a_symbol );
extern a_symbol make_symbol(a_token, a_type_kind , a_structural_type_kind);
extern std::string get_sym_name(a_symtable_index);
extern a_type_kind get_sym_type(std::string);
extern a_symbol_state get_sym_state(std::string);
extern a_structural_type_kind get_sym_stype(std::string);
extern a_symtable_index update_symbol_state(std::string, a_symbol_state);
extern a_symtable_index update_symbol_state(const a_symbol);
extern bool sym_present(a_symbol);
extern bool sym_present(std::string);

#endif /*SYMBOL_TBL_H*/
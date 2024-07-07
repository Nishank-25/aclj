#ifndef SYMBOL_TBL_H
#define SYMBOL_TBL_H

#include <vector>
#include <string_view>
#include <variant>

#include "common/globals.h"


#define MAX_ENTRIES 1024

static std::vector<a_symbol> global_symtable;
size_t global_symtable_size = 0;
typedef int a_symbtable_index;
#define SYMBOL_NOT_FOUND -1
#define NO_EMPTY_SLOT -1
#define SYMBOl_FOUND ()

extern a_symtable_index find_symbol( const a_symbol& );
extern a_symbtable_index add_sym(const a_symbol );
extern a_symbol make_symbol(a_token);

enum class a_variable_state
{
    state_declared,
    state_defined,
    state_assigned,
    state_referenced // used
};

enum class a_function_state
{
    state_declared,
    state_defined,
    state_called // used
};

using a_symbol_state = std::variant<empty,a_variable_state, a_function_state>;

struct a_symbol { 
    std::string_view name;
    a_symbol_state state; 
};

#endif
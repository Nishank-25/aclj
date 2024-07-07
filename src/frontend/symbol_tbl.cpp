#include<iostream>
#include<frontend/symbol_tbl.h>
#include "symbol_tbl.h"

a_symtable_index find_symbol(const a_symbol& symbol)
{
    a_symbtable_index index = 0;
    for(a_symbol sym : global_symtable)
        {
            if (sym.name == symbol.name)
                return index;
            ++index;
        }

    return SYMBOL_NOT_FOUND;
}

static a_symbtable_index get_new_slot()
{
    return (global_symtable_size < MAX_ENTRIES) ? ++global_symtable_size : NO_EMPTY_SLOT;
}

void update_symbol_state(const a_symbol sym)
{
    a_symbtable_index index = find_symbol(sym);
    global_symtable[index].state = sym.state;
    return index;
}
a_symbtable_index add_sym(const a_symbol sym)
{
    a_symbtable_index index = get_new_slot();
    
    if ( index == -1){ std::cerr<< "Symbol table full!!!"; exit(1);}
    global_symtable[index] = sym;
    return index;
}

a_symbol make_symbol(a_token tok)
{
    a_symbol sym;
    sym.name = tok.value;
    sym.state = empty;
}
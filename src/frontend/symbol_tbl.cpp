#include<string>
#include<iostream>
#include<vector>
#include<frontend/symbol_tbl.h>

std::vector<a_symbol> global_symtable;
size_t global_symtable_size = 0;

a_symtable_index find_symbol(a_symbol& symbol)
{
    a_symtable_index index = 0;
    for(a_symbol sym : global_symtable)
        {
            if (sym.name == symbol.name)
            {
                symbol = sym;
                return index;
            }
            ++index;
        }

    return SYMBOL_NOT_FOUND;
}

a_symtable_index find_symbol(std::string name)
{
    a_symtable_index index = 0;
    for(a_symbol sym : global_symtable)
    {
        if (sym.name == name)
            return index;
        ++index;
    }
    return SYMBOL_NOT_FOUND;
}

static a_symtable_index get_new_slot()
{
    return (global_symtable_size < MAX_ENTRIES) ? global_symtable_size++ : NO_EMPTY_SLOT;
}

a_symtable_index update_symbol_state(const a_symbol sym)
{
    a_symtable_index index = find_symbol(sym.name);
    global_symtable[index].state = sym.state;
    return index;
}
a_symtable_index add_sym(a_symbol sym)
{
    a_symtable_index index = get_new_slot();
    
    if ( index == -1){ std::cerr<< "Symbol table full!!!"; exit(1);}
    global_symtable.push_back(sym);
    return index;
}

a_symbol make_symbol(a_token tok)
{
    a_symbol sym;
    sym.name = std::get<an_ident>(tok.value);
    sym.state = empty_state{};
    return sym;
}

std::string get_sym_name (a_symtable_index idx)
{
    return global_symtable[idx].name;
}

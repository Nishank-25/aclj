#include "common/globals.h"
#include "frontend/lex.h"
#include "frontend/symbol_tbl.h"
#include "frontend/tree.h"
#include "codegen/gen.h"
#include "frontend/expr.h"
#include <iostream>

/*
***** Grammar Start *****

variable_declaration: "int" identifier ";"

***** Grammar End *****
*/
#define check_redecl(sym)   sym_present(sym,!=," redeclared\n")

void variable_declaration()
{
    a_token tok;
    a_symbol sym;

    expect_int();
    tok = expect_ident();
    
    sym = make_symbol(tok);
    check_redecl(sym);
    sym.state = a_variable_state::state_declared;
    add_sym(sym);
    
    // Not here will wait for an assignment to happen
    //gen_globalsym(curr_token.value);
    expect_semi();
}
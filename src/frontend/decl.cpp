#include "common/globals.h"
#include "frontend/lex.h"
#include "frontend/symbol_tbl.h"
#include "frontend/tree.h"
#include "codegen/gen.h"
#include <iostream>

/*
***** Grammar Start *****

variable_declaration: "int" identifier ";"

assignment_expression: identifier "=" expression ";"

***** Grammar End *****
*/

#define decl(sym,op,str)    if ( find_symbol(sym) op SYMBOL_NOT_FOUND) \
                            {                                          \
                                std::cerr<<sym.name<<str;              \
                                exit(1);                               \
                            }

#define check_decl(sym)     decl(sym,==," not declared\n")
#define check_redecl(sym)   decl(sym,!=," redeclared\n")

void check_decl(a_symbol sym)
{
    if (find_symbol(sym) == SYMBOL_NOT_FOUND)
    {
        std::cerr<<sym.name<<" not declared\n"
    }
}
void variable_declaration()
{
    a_token tok;
    a_symbol sym;

    expect_int();
    tok = expect_ident();
    
    sym = make_symbol(tok);
    check_redecl(sym);
    sym.state = a_variable_state::state_declared;
    add_symbol(sym);
    
    // Not here will wait for an assignment to happen
    //gen_globalsym(curr_token.value);
    expect_semi();
}

void assign_expression()
{
    a_token tok;
    a_symbol sym;
    a_symbtable_index sym_idx;

    tok = expect_ident();

    sym = make_symbol(tok);
    check_decl(sym);
    
    sym.state = a_variable_state::state_assigned;
    sym_idx = update_symbol_state(sym);
    gen_globalsym(sym.name);

    right = mk_leaf_node(an_ast_node_kind::node_lvalue, sym_idx);

    expect_equals();

    // get the assigning expression
    left = expr(0);

    node = mk_node(an_ast_node_kind::node_assign, left, right, 0);

    gen_AST(node,-1);
    gen_free_regs();

    expect_semi();
}


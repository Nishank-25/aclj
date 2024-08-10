#ifndef GEN_H
#define GEN_H

#include <string>
#include <frontend/tree.h>
/* This should be in target specific headers */

void cgprologue();
void cgepilogue();
void cgprintint(int);
int  cgload(int);
int  cgadd(int , int);
int  cgsub(int , int);
int  cgmul(int , int);
int  cgdiv(int , int);
int  cgload_sym(std::string);
int  cgstr_sym(int,std::string);
int  cgcomp_op(an_ast_node_kind, int,int);
int  cg_compare_and_set(an_ast_node_kind , int , int);
int  cg_compare_and_jump(an_ast_node_kind , int, int, int);
void cg_jump(int);
void cg_label(int);
void cg_func_prologue(std::string);
void cg_func_epilogue(a_symtable_index);
int  cg_widen(int , a_type_kind , a_type_kind);
void cg_return(int , std::string);
int  cg_call(int , std::string);

int  gen_AST(an_ast_node*, int , an_ast_node_kind);
void gen_prologue();
void gen_free_regs();
void gen_printint(int);
void gen_globalsym(std::string);

#define NOREG -1
#endif /* GEN_H */

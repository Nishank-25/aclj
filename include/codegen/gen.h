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
int  cg_eq(int,int);
int  cg_neq(int,int);
int  cg_le(int,int);
int  cg_ge(int,int);
int  cg_lt(int,int);
int  cg_gt(int,int);
int  cg_compare_and_set(an_ast_node_kind , int , int);
int  cg_compare_and_jump(an_ast_node_kind , int, int, int);
void cg_jump(int);
void cg_label(int);

int  gen_AST(an_ast_node*, int , an_ast_node_kind);
void gen_prologue();
void gen_epilogue();
void gen_free_regs();
void gen_printint(int);
void gen_globalsym(std::string);

#define NOREG -1
#endif /* GEN_H */

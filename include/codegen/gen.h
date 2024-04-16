#ifndef GEN_H
#define GEN_H

/* This should be in target specific headers */

void cgprologue();
void cgepilogue();
void cgprintint(int);
int  cgload(int);
int  cgadd(int , int);
int  cgsub(int , int);
int  cgmul(int , int);
int  cgdiv(int , int);


int  gen_AST(a_ast_node* );
void gen_prologue();
void gen_epilogue();
void gen_free_regs();
void gen_printint(int);

#endif /* GEN_H */

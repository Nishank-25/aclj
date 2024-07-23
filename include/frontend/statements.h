#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "frontend/tree.h"

an_ast_node* print_statement();
an_ast_node* assign_statement();
an_ast_node* if_statement();
an_ast_node* statements();
an_ast_node* compound_statement();


#endif
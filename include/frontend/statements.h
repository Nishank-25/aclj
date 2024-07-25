#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "frontend/tree.h"

an_ast_node* print_statement();
an_ast_node* assign_statement(bool);
an_ast_node* if_statement();
an_ast_node* statements();
an_ast_node* compound_statement();
an_ast_node* while_statement();
an_ast_node* do_while_statement();
an_ast_node* for_statement();


#endif
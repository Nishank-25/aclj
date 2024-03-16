#include "frontend/lex.h"
#include "frontend/tree.h"
#include <string>


extern void print_tokens();
extern int  print_ast(a_ast_node*);
extern void scan_cmd_line(int,char const* argcv[]);

extern int print[5];
extern std::string input_file;
#define PRINT_TOKENS 0
#define PRINT_AST_WITHOUT_PRECEDENCE 1
#define PRINT_AST_WITH_PRECEDENCE 2

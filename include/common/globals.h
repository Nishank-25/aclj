#ifndef GLOBALS_H
#define GLOBALS_H

#include"frontend/lex.h"
extern a_token match(a_token_kind);

#define expect_print()  match(a_token_kind::tok_print)
#define expect_semi()   match(a_token_kind::tok_semicolon)
#define expect_eof()    match(a_token_kind::tok_eof)
#define expect_int()    match(a_token_kind::tok_int)
#define expect_ident()  match(a_token_kind::tok_ident)
#define expect_equals() match(a_token_kind::tok_eq)
#define expect_lbrace() match(a_token_kind::tok_lbrace)
#define expect_rbrace() match(a_token_kind::tok_rbrace)
#define expect_lparen() match(a_token_kind::tok_lparen)
#define expect_rparen() match(a_token_kind::tok_rparen)

#endif /* GLOBALS_H */

#ifndef GLOBALS_H
#define GLOBALS_H

#include<util/helper.h>

#define expect_print()  match(a_token_kind::tok_print);
#define expect_semi()   match(a_token_kind::tok_semicolon);
#define expect_eof()    match(a_token_kind::tok_eof);
#define expect_int()    match(a_token_kind::tok_int);
#define expect_ident()  match(a_token_kind::tok_ident);
#define expect_equals() match(a_token_kind::tok_equals);

using empty = std::monostate;

#endif /* GLOBALS_H */

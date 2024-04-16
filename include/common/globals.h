#ifndef GLOBALS_H
#define GLOBALS_H

#define isprint_token(tok)  ( tok.kind == a_token_kind::tok_print )
#define issemi_token(tok)   ( tok.kind == a_token_kind::tok_semicolon)
#define iseof_token(tok)    ( tok.kind == a_token_kind::tok_eof)

#endif /* GLOBALS_H */

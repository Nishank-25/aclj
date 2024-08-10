#ifndef TYPES_H
#define TYPES_H

#include "frontend/lex.h"

enum class a_type_kind
{
    none = 16,
    Char,
    Int,
    Long,
    Void
};

enum class a_structural_type_kind
{
    variable,
    function
};


extern int type_size[5];

#define is_void_type(t) (t == a_type_kind::Void)
#define is_int_type(t)  (t == a_type_kind::Int)
#define is_char_type(t) (t == a_type_kind::Char)
#define is_long_type(t) (t == a_type_kind::Long)
extern a_type_kind tok_to_type(a_token_kind);
extern bool type_compatible(int&, int&, bool); 
#endif
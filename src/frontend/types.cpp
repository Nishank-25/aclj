#include "frontend/types.h"
#include "frontend/lex.h"
#include "util/helper.h"
#include <iostream>

a_type_kind tok_to_type(a_token_kind tok_kind)
{
    if (a_token_kind::tok_char <= tok_kind && tok_kind <= a_token_kind::tok_void)
        return ( (a_type_kind) tok_kind);
    else
    {
        std::cerr<<"Unkown type "<<tok_str(tok_kind)<<"\n";
        exit(1);
    }
}

int type_size[] = {0 , 1 , 4, 8, 0};

// Given two primitive types, return true if they are compatible,
// false otherwise. Also return either zero or an A_WIDEN
// operation if one has to be widened to match the other.
// If onlyright is true, only widen left to right.
bool type_compatible(int& left, int& right, bool onlyright) {
    
    int left_size , right_size;

    if (left == right)
        {
            left = right = 0;
            return true;
        }

    left_size = type_size[left - (int)a_type_kind::none];
    right_size = type_size[left - (int)a_type_kind::none];

    if ( (left_size == 0) || (right_size == 0) ) return false; 

    if (left_size < right_size )
    {
        left = (int) an_ast_node_kind::node_widen; right = 0; return true;
    }
    
    if ( left_size > right_size )
    {
        if (onlyright) return false;
        left = 0; right = (int) an_ast_node_kind::node_widen; return false;
    }
  // Anything remaining is compatible
  left = right = 0;
  return true;
}
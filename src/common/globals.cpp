#include<common/globals.h>
#include"util/helper.h"
#include<iostream>

a_token match(a_token_kind expected)
{
    if(curr_token.kind == expected) 
    { 
        a_token tok = curr_token;
        get_token();
        return tok;
    }
    else
    {
        std::cerr<<"Expected "<<get_token_name(expected)<<"\n";
        exit(1);
    }
    
}
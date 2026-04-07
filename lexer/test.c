#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "../jsonvalue.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <jsonfile>\n", argv[0]);
        return EXIT_FAILURE;
    }
    Lexer l;
    lexer_init(&l, argv[1]);
    Token *t_head = tokenize(&l);
    print_tokens(t_head);
    
    return 0;
}
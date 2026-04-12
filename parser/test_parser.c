#include <stdio.h>
#include <stdlib.h>
#include "../lexer/lexer.h"
#include "../jsonvalue.h"
#include "parser.h"

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
    // print_tokens(t_head);

    Parser p;
    parser_init(&p, t_head);
    JsonValue *jv = parser_value(&p);
    lexer_free(&l);
    token_free(t_head);
    print_value(jv);
    jsonvalue_free(jv);
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson.h"
#include "jsonvalue.h"
#include "./parser/parser.h"
#include "./lexer/lexer.h"

JsonValue *jsonvalue_parse(char *filename)
{
    Lexer l;
    Parser p;
    lexer_init(&l, filename);
    Token *t = tokenize(&l);
    parser_init(&p, t);
    JsonValue *jv = parser_value(&p);
    lexer_free(&l);
    token_free(t);

    return jv;
}

void js_pretty_print(JsonValue *jv)
{
    ;
}
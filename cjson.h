#ifndef CJSON_C
#define CJSON_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsonvalue.h"
#include "./parser/parser.h"
#include "./lexer/lexer.h"

JsonValue *jsonvalue_parse(char *filename);

extern void jsonvalue_free(JsonValue *jv);

void js_pretty_print(JsonValue *jv);
    
#endif
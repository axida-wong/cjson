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

extern void pretty_print(JsonValue *j);

JsonValue *get_object_value(JsonValue *jv, const char *key);

JsonValue *get_array_value(JsonValue *jv, int index);

JsonValue *get_value(JsonValue *jv, int depth, ...);

JsonValue *create_array();

JsonValue *create_object();

    
#endif
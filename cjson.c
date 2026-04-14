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

JsonValue *get_object_value(JsonValue *jv, const char *key)
{
    if(jv->tag != JSON_OBJECT)
    {
        fprintf(stderr, "The tag of JsonValue is not object.\n");
        return NULL;
    }
    if(!key || *key == '\0')
    {
        fprintf(stderr, "key can't be a null string");
        return NULL;
    }

    Object *tmp_obj = jv->obj;
    while(tmp_obj)
    {
        if(strcmp(tmp_obj->key, key) == 0)
            return tmp_obj->jv;             
        else
            tmp_obj = tmp_obj->next;
    }

    return NULL;
}

JsonValue *get_array_value(JsonValue *jv, int index)
{
    if(jv->tag != JSON_ARRAY)
    {
        fprintf(stderr, "The tag of JsonValue is not object.\n");
        return NULL;
    }
   
    Array *tmp_arr = jv->arr;
    int i = 0;
    while(i < index && tmp_arr != NULL)
    {
        tmp_arr = tmp_arr->next;
        i++;
    }
    
    if(!tmp_arr)
        return NULL;
    else
        return tmp_arr->elem;
}

JsonValue *get_value(JsonValue *jv, int depth, ...)
{
    ;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../jsonvalue.h"
#include "parser.h"

int init_parser(Parser *p, Token *t)
{
    p->t = t;
}

JsonValue *paerser_object(Parser *p)
{
    if(p->t->tag == TOKEN_NULL)
        return NULL;

    JsonValue *j = (JsonValue *)malloc(sizeof(struct jsonvalue ));
    j->tag = JSON_OBJECT;
    j->obj = NULL;
    p->t = p->t->next;
    Object *tmp_obj = j->obj;

    while(p->t->tag != TOKEN_RBRACE)
    {
        if(p->t->tag == TOKEN_STRING && p->t->next->tag == TOKEN_COLON)
        {
            tmp_obj->key = p->t->str;
            p->t = p->t->next->next;
            tmp_obj = (Object *)malloc(sizeof(struct object *));
            tmp_obj->jv = parser_value(p);
        }
        else
        {
            fprintf(stderr, "Fortmat of the json file is wrong.");
            return NULL;
        }

        if(p->t->tag == TOKEN_COMMA)
        {
            p->t = p->t->next;
            continue;
        }
        else
        {
            fprintf(stderr, "Fortmat of the json file is wrong.");
            return NULL;
        }
    }
    tmp_obj = NULL;

    p->t = p->t->next;

    return j;
}
JsonValue *parser_array(Parser *p)
{
    if(p->t->tag == TOKEN_NULL)
        return NULL;

    JsonValue *j = (JsonValue *)malloc(sizeof(struct jsonvalue ));
    j->tag = JSON_ARRAY;
    j->arr = NULL;
    p->t = p->t->next; 
    Array *tmp_arr = j->arr;

    while(p->t->tag != TOKEN_RBRACKET)
    {
        tmp_arr = (Array *)malloc(sizeof(struct object));
        tmp_arr->elem = parser_array(p);
        p->t = p->t->next;
        if(p->t->tag == TOKEN_COMMA)
        {
            fprintf(stderr, "Fortmat of the json file is wrong.");
            return NULL;
        }
        tmp_arr = tmp_arr->next;
    }
    tmp_arr = NULL;

    return j;
}

JsonValue *parser_value(Parser *p)
{
    if(p->t->tag == TOKEN_NULL)
        return NULL;

    JsonValue *jv = (JsonValue *)malloc(sizeof(struct jsonvalue ));
    switch(p->t->tag)
    {
    case TOKEN_LBRACE:
        jv = paerser_object(p);
        break;
    case TOKEN_LBRACKET:
        jv = parser_array(p);
        break;
    case TOKEN_NUMBER:
        jv->tag = JSON_NUMBER;
        jv->num = p->t->num;
        break;
    case TOKEN_STRING:
        p->t = p->t->next;
        jv->tag = JSON_STRING;
        jv->str = p->t->str;
        break;
    case TOKEN_BOOL:
        p->t = p->t->next;
        jv->tag = JSON_BOOL;
        jv->boolean = p->t->b;
    case TOKEN_NULL: 
        p->t = p->t->next;
        jv->tag = JSON_NULL;
        jv->null = NULL;
        break;
    }
    jv->next = NULL;

    return jv;
}
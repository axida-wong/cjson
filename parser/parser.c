#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../jsonvalue.h"
#include "parser.h"

void parser_init(Parser *p, Token *t)
{
    p->t = t;
}

JsonValue *parser_object(Parser *p)
{
    p->t = p->t->next;
    if(p->t->tag == TOKEN_NULL)
        return NULL;

    JsonValue *j = (JsonValue *)malloc(sizeof(struct jsonvalue ));
    j->tag = JSON_OBJECT;
    Object dummy;
    Object *tmp_obj = &dummy;

    while(p->t->tag != TOKEN_RBRACE)
    {
        if(p->t->tag == TOKEN_STRING && p->t->next->tag == TOKEN_COLON)
        {
            tmp_obj->next = (Object *)malloc(sizeof(struct object ));
            tmp_obj = tmp_obj->next;
            tmp_obj->key = p->t->str;
            p->t = p->t->next->next;
            tmp_obj->jv = parser_value(p);
            tmp_obj->next = NULL;
        }
        else
        {
            fprintf(stderr, "Fortmat of the json file is wrong.");
            printf("%d", p->t->tag);
            return NULL;
        }

        if(p->t->tag == TOKEN_COMMA)
            p->t = p->t->next;
    }
    j->obj = dummy.next;

    p->t = p->t->next;

    return j;
}

JsonValue *parser_array(Parser *p)
{
    p->t = p->t->next;
    if(p->t->tag == TOKEN_NULL)
        return NULL;

    JsonValue *j = (JsonValue *)malloc(sizeof(struct jsonvalue ));
    j->tag = JSON_ARRAY;
    Array dummy;
    Array *tmp_arr = &dummy;

    while(p->t->tag != TOKEN_RBRACKET)
    {
        tmp_arr->next = (Array *)malloc(sizeof(struct array));
        tmp_arr = tmp_arr->next;
        tmp_arr->elem = parser_value(p);
        if(p->t->tag == TOKEN_COMMA)
            p->t = p->t->next;
    }
    j->arr = dummy.next;
    p->t = p->t->next;

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
        jv = parser_object(p);
        break;
    case TOKEN_LBRACKET:
        jv = parser_array(p);
        break;
    case TOKEN_NUMBER:
        jv->tag = JSON_NUMBER;
        jv->num = p->t->num;
        p->t = p->t->next;
        break;
    case TOKEN_STRING:
        jv->tag = JSON_STRING;
        jv->str = p->t->str;
        p->t = p->t->next;
        break;
    case TOKEN_BOOL:
        jv->tag = JSON_BOOL;
        jv->boolean = p->t->b;
        p->t = p->t->next;
        break;
    case TOKEN_NULL: 
        p->t = p->t->next;
        jv->tag = JSON_NULL;
        jv->null = NULL;
        break;
    }
    jv->next = NULL;

    return jv;
}

void print_object(JsonValue *j)
{
    Object *o = j->obj;
    printf("{\n");
    while(o)
    {
        printf("\t");
        printf("%s: ", o->key);
        print_value(o->jv);
        o = o->next;
    }
    printf("}\n");
}

void print_array(JsonValue *j)
{
    Array *a = j->arr;
    printf("[\n");
    while(a)
    {
        printf("\t");
        print_value(a->elem);
        a = a->next;
    }
    printf("]\n");
}

void print_value(JsonValue *j)
{   
    switch (j->tag)
    {
    case JSON_NULL:
        printf(" null,\n");
        break;
    case JSON_BOOL:
        printf(" %s,", j->boolean ? "true": "false");
        break;
    case JSON_NUMBER:
        printf(" %g,", j->num);
        break;
    case JSON_STRING:
        printf(" \"%s\",", j->str);
        break;
    case JSON_ARRAY:
        print_array(j);
        break;
    case JSON_OBJECT:
        print_object(j);
        break;
    default:
        break;
    }
}

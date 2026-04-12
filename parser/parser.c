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

static JsonValue *new_value(enum JSONVALUE_TAG tag)
{
    JsonValue *j = (JsonValue *)malloc(sizeof(struct jsonvalue));
    j->tag = tag;
    j->next = NULL;
    return j;
}

static bool is_eof(Token *t)
{
    return t->tag == TOKEN_EOF;
}

static void parser_forward(Parser *p)
{
    p->t = p->t->next;
}

JsonValue *parser_object(Parser *p)
{
    parser_forward(p);
    if(is_eof(p->t))
    {
        fprintf(stderr, "Format of the json file is wrong.%d\n", p->t->tag);
        exit(EXIT_FAILURE);
    }
    if(p->t->tag == TOKEN_RBRACE)
    {
        JsonValue *j = new_value(JSON_OBJECT);
        j->obj = NULL;
        parser_forward(p);
        return  j;
    }

    JsonValue *j = new_value(JSON_OBJECT);
    Object dummy; // set dummy head node
    dummy.next = NULL;
    Object *tmp_obj = &dummy;

    while(true)
    {
        if(p->t->tag == TOKEN_STRING && p->t->next->tag == TOKEN_COLON)
        {
            tmp_obj->next = (Object *)malloc(sizeof(struct object ));
            tmp_obj = tmp_obj->next;
            tmp_obj->key = p->t->str;
            parser_forward(p);
            parser_forward(p);
            tmp_obj->jv = parser_value(p);
            tmp_obj->next = NULL;
        }
        else
        {
            fprintf(stderr, "Format of the json file is wrong.%d\n", p->t->tag);
            exit(EXIT_FAILURE);
        }

        if(p->t->tag == TOKEN_COMMA)
            parser_forward(p);
        else if(p->t->tag == TOKEN_RBRACE)
        {
            parser_forward(p);
            break;
        }
        else
        {
            fprintf(stderr, "Format of the json file is wrong.%d\n", p->t->tag);
            exit(EXIT_FAILURE);
        }
    }
    j->obj = dummy.next;

    return j;
}

JsonValue *parser_array(Parser *p)
{
    parser_forward(p);
    if(p->t->tag == TOKEN_EOF)
    {
        fprintf(stderr, "Format of the json file is wrong.%d\n", p->t->tag);
        exit(EXIT_FAILURE);
    }
    if(p->t->tag == TOKEN_RBRACKET)
    {
        JsonValue *j = new_value(JSON_ARRAY);
        // JsonValue *j = malloc(sizeof(struct jsonvalue));
        // j->tag = JSON_ARRAY;
        j->arr = NULL;
        parser_forward(p);
        return  j;
    }

    JsonValue *j = new_value(JSON_ARRAY);
    Array dummy; // set dummy header node
    dummy.next = NULL;
    Array *tmp_arr = &dummy;

    while(true)
    {
        tmp_arr->next = (Array *)malloc(sizeof(struct array));
        tmp_arr = tmp_arr->next;
        tmp_arr->next = NULL;
        tmp_arr->elem = parser_value(p);

        if(p->t->tag == TOKEN_COMMA) 
            parser_forward(p);
        else if(p->t->tag == TOKEN_RBRACKET)
        {
            parser_forward(p);
            break;
        }
        else
        {
            fprintf(stderr, "Format of the json file is wrong.%d\n", p->t->tag);
            exit(EXIT_FAILURE);
        }
    }
    j->arr = dummy.next;

    return j;
}

JsonValue *parser_value(Parser *p)
{
    JsonValue *jv = NULL;
    switch(p->t->tag)
    {
    case TOKEN_LBRACE:
        jv = parser_object(p);
        break;
    case TOKEN_LBRACKET:
        jv = parser_array(p);
        break;
    case TOKEN_NUMBER:
        jv = new_value(JSON_NUMBER);
        jv->num = p->t->num;
        parser_forward(p);
        break;
    case TOKEN_STRING:
        jv = new_value(JSON_STRING);
        jv->str = p->t->str;
        parser_forward(p);
        break;
    case TOKEN_BOOL:
        jv = new_value(JSON_BOOL);
        jv->boolean = p->t->b;
        parser_forward(p);
        break;
    case TOKEN_NULL: 
        jv = new_value(JSON_NULL);
        jv->null = NULL;
        parser_forward(p);
        break;
    // case TOKEN_EOF:
    default:
        fprintf(stderr, "Unexpectted token: %d\n", p->t->tag);
        exit(EXIT_FAILURE);
    }

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

void jsonvalue_free(JsonValue *jv)
{
    if(!jv)
        return ;
    switch (jv->tag)
    {
    case JSON_ARRAY:
    {
        Array *cur = jv->arr;
        while(!cur)
        {
            Array *tmp = cur->next;
            jsonvalue_free(cur->elem);
            free(cur);
            cur = tmp;
        }
    }
    case JSON_OBJECT:
    {
        Object *cur = jv->obj;
        while(!cur)
        {
            Object *tmp = cur->next;
            jsonvalue_free(cur->jv);
            free(cur);
            cur = tmp;
        }
    }
    break;
    default:
        break;
    }
    free(jv);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"
#include "../jsonvalue.h"
#include "parser.h"

static bool is_eof(Token *t);
static void parser_forward(Parser *p);
static JsonValue *new_value(enum JSONVALUE_TAG tag);
static void print_depth(int depth);
static void pretty_recurse_print(JsonValue *j, int depth);

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
            // tmp_obj->key = p->t->str;
            tmp_obj->key = strdup(p->t->str);
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
        // jv->str = p->t->str;
        jv->str = strdup(p->t->str);
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

static void print_depth(int depth)
{
    for(int i = 0; i < depth; i++)
        printf("    ");
}

void print_object(JsonValue *j, int depth)
{
    Object *o = j->obj;
    print_depth(depth - 1);
    printf("{\n");
    while(o)
    {
        print_depth(depth);
        printf("%s: ", o->key);
        pretty_recurse_print(o->jv, depth);
        if(o->next != giNULL)
            putchar(',');
        putchar('\n');
        o = o->next;
    }
    printf("}\n");
}

void print_array(JsonValue *j, int depth)
{
    Array *a = j->arr;
    print_depth(depth);
    printf("[\n");
    while(a)
    {
        pretty_recurse_print(a->elem, depth+1);
        if(a->next != NULL)
            putchar(',');
        putchar('\n');
        a = a->next;
    }
    print_depth(depth);
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
        print_array(j, 0);
        break;
    case JSON_OBJECT:
        print_object(j, 0);
        break;
    default:
        break;
    }
}

static void pretty_recurse_print(JsonValue *j, int depth)
{   
    for(int i = 0; i < depth; i++)
        printf("    ");

    switch (j->tag)
    {
    case JSON_NULL:
        printf(" null");
        break;
    case JSON_BOOL:
        printf(" %s", j->boolean ? "true": "false");
        break;
    case JSON_NUMBER:
        printf(" %g", j->num);
        break;
    case JSON_STRING:
        printf(" \"%s\"", j->str);
        break;
    case JSON_ARRAY:
        print_array(j, depth + 1);
        break;
    case JSON_OBJECT:
        print_object(j, depth + 1);
        break;
    default:
        break;
    }
}

void pretty_print(JsonValue *j)
{
    //default tab size is 4
    pretty_recurse_print(j, 0);
}

void jsonvalue_free(JsonValue *j)
{
    if(!j)
        return ;
    switch (j->tag)
    {
    case JSON_ARRAY:
    {
        Array *cur = j->arr;
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
        Object *cur = j->obj;
        while(!cur)
        {
            Object *tmp = cur->next;
            free(cur->key);
            jsonvalue_free(cur->jv);
            free(cur);
            cur = tmp;
        }
    }
    break;
    default:
        break;
    }
    free(j);
}
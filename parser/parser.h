#ifndef PARSER_H
#define PARSER_H

#include "../jsonvalue.h"
#include "../lexer/lexer.h"

typedef struct parser
{
    Token *t;
}Parser;

//init a parser, p->t point the start 
//the token t
//pre: p: pointer of a parser
//     t: pointer of the token
void parser_init(Parser *p, Token *t);

//create a json object recursivly
//pre: p: pointer of a parser
//
//post: return the pointer of the 
//jsonvalue
JsonValue *parser_object(Parser *p);

//create a json array recursively
//pre: p: pointer of a parser
//
//post: return the pointer of the 
//jsonvalue
JsonValue *parser_array(Parser *p);

//create a jsonvalue structure, it 
//recursivly calling itself or 
//parser_object, parser_array
JsonValue *parser_value(Parser *p);

//print the object recursivly
void print_object(JsonValue *j);

//print the array recursivly
void print_array(JsonValue *j);

//print the jsonvalue recursivly
void print_value(JsonValue *j);

//free the jsonvalue AST
void jsonvalue_free(JsonValue *jv);
#endif
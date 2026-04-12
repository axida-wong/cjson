#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

typedef struct lexer
{
    char *src;
    int pos;
}Lexer;

enum TOKEN_TAG
{
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,   
    TOKEN_RBRACKET,   
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_BOOL,
    TOKEN_NULL,
    TOKEN_EOF
};

typedef struct token
{
    union 
    {
        // char c; 多余，左右括号的信息已经在tag里包含了。
        bool b;
        double num;
        char *str;
    };
    enum TOKEN_TAG tag;
    struct token *next;
}Token;

//init a lexer, load all the content of the
//file into a buffer that has enough size.
//pre:  l: pointer of the lexer.
//      filename: filename of the jsonfile. 
//
//post: return 0 when the function execute
//      successfully. 
int lexer_init(Lexer *l, char *filename);

//get next token, return the pointer of the
//new token.
//pre:  l: pointer of the lexer.
//
//post: return the pointer of new token
Token *next_token(Lexer *l);

//transform all the content of a buffer of 
//lexer into token linklist.
//pre:  l: pointer of the lexer.
//
//post: return the header pointer of the 
//      linklist.
Token *tokenize(Lexer *l);

//print all the token of the linklist of the
//tokens.
//pre:  head: the pointer pointing the head
//      node of token linklist.
void print_tokens(Token *head);

//free the lexer
void lexer_free(Lexer *l);

//free the token linklist
void token_free(Token *t);

#endif
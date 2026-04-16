#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "../jsonvalue.h"

typedef char *buffer;

static int load_jsonfile(char *filename, buffer *buf)
{
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        fprintf(stderr, "can't open the file %s\n", filename);
        return -1;
    }

    //fseek and ftell, read all the content of file at one time.
    //SEEK_CUR, current position
    //SEEK_SET, front of the file
    //SEEK_END, end of the file
    fseek(fp, 0, SEEK_END);
    long buf_size = ftell(fp);
    *buf = (char *) malloc(sizeof(char ) * (buf_size + 1));
    if(*buf == NULL)
    {
        fprintf(stderr, "Lack of memory!\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    size_t readn = fread(*buf, 1, buf_size, fp);
    if(readn != buf_size)
    {
        fprintf(stderr, "read file error\n");
    }
    *(*buf + buf_size) = '\0'; 
    fclose(fp);

    return 0;
}

//init a lexer
int lexer_init(Lexer *l, char *filename)
{
    buffer b;
    if(load_jsonfile(filename, &b) == 0)
    {
        l->pos = 0;
        l->src = b;

        return 0;
    }
    else
        return -1;

}

static char *get_token_string(Lexer *l)
{
    l->pos++;
    int start = l->pos;
    while(l->src[l->pos] != '\"' && l->src[l->pos] != '\0')
        if(l->src[l->pos] == '\\' && l->src[l->pos + 1] != '\0')
            l->pos += 2;
        else
            l->pos++;
    int cnt = l->pos - start; // 算出长度
    l->pos = start; //恢复开头
    //reading process
    char *s = (char *)malloc(sizeof(char ) * (cnt + 1));
    s[cnt] = '\0';
    for(int i = 0; i < cnt; i++)
        s[i] = l->src[l->pos++];
    l->pos++;

    return s;
}

static Token *get_token_keyword(Lexer *l)
{
    Token *t = (Token *)malloc(sizeof(struct token ));
    t->next = NULL;
    switch (l->src[l->pos])
    {
    case 'n':
        if(strncmp(l->src + l->pos, "null", 4) == 0)
        {
            l->pos += 4;
            t->tag = TOKEN_NULL;
        }
        else 
        {
            fprintf(stderr, "The format of json file is not corret.\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 't':
        if(strncmp(l->src + l->pos, "true", 4) == 0)
        {
            l->pos += 4;
            t->b = true;
            t->tag = TOKEN_BOOL;
        }
        else 
        {
            fprintf(stderr, "The format of json file is not corret .\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 'f':
        if(strncmp(l->src + l->pos, "false", 5) == 0)
        {
            l->pos += 5;
            t->b = false;
            t->tag = TOKEN_BOOL;
        }
        else 
        {
            fprintf(stderr, "The format of json file is not corret.\n");
            exit(EXIT_FAILURE);
        }
        break;
    }

    return t;
}

static double get_token_number(Lexer *l)
{
    char *end;    
    double num = strtod(l->src + l->pos, &end);
    l->pos = end - l->src; // 重新更新pos,到数字结束的位置

    return num;
}

static Token *char_analyzie(Lexer *l)
{
    Token * tmp = (Token *)malloc(sizeof(struct token ));
	tmp->next = NULL;

    switch (l->src[l->pos])
    {
    case '{':
        tmp->tag = TOKEN_LBRACE;
        l->pos++;
        break;
    case '}':
        tmp->tag = TOKEN_RBRACE;
        l->pos++;
        break;
    case '[':
        tmp->tag = TOKEN_LBRACKET;
        l->pos++;
        break;
    case ']':
        tmp->tag = TOKEN_RBRACKET;
        l->pos++;
        break;
    case ':':
        tmp->tag = TOKEN_COLON;
        l->pos++;
        break;
    case ',':
        tmp->tag = TOKEN_COMMA;
        l->pos++;
        break;
    case '\"':
        tmp->tag = TOKEN_STRING;
        tmp->str = get_token_string(l);
        break;
    case 'n':
    case 't':
    case 'f':
        return get_token_keyword(l);
        break;
    default: //the return value of isdigit is not (0,1), is 1/2048 or 0
        if(isdigit(l->src[l->pos]) || l->src[l->pos] == '-')
        {
            tmp->tag = TOKEN_NUMBER;
            tmp->num = get_token_number(l);           
        }
        else
        {
            fprintf(stderr, "unexpected char: '%c' ascii=%d dat pos=%d\n",
                l->src[l->pos], l->src[l->pos], l->pos);
            l->pos++;
			free(tmp);//here is the garbage token, release memory of the tmp
			return next_token(l);
        }
        break;
    }

    return tmp;
}

Token *next_token(Lexer *l)
{
    //skip the blank character
    while(l->src[l->pos] == '\n' || l->src[l->pos] == '\r' ||
        l->src[l->pos] == '\t' || l->src[l->pos] == ' ' || 
        (l->src[l->pos] > 0 && l->src[l->pos] < 32))
        l->pos++;

    if(l->src[l->pos] == '\0')
    {
        Token *t = (Token *)malloc(sizeof(struct token));
        t->tag = TOKEN_EOF;
        t->next = NULL;
        return t;
    }
    
    Token *t = char_analyzie(l);
    return t;
}

Token *tokenize(Lexer *l)
{
    Token *header = next_token(l);
    Token *cur = header;
    while(cur->tag != TOKEN_EOF)
    {
        cur->next = next_token(l); 
        cur = cur->next;
    }
    
    return header;
}

void print_tokens(Token *head)
{
	if(!head)
	{
		fprintf(stderr, "pointer of token is null.\n");
		return ;
	}
    // Token is a linklist of tokens
    while(head->tag != TOKEN_EOF)
    {
        putchar(' ');
        switch (head->tag)
        {
        case TOKEN_LBRACE:
            putchar('{');
            break;
        case TOKEN_RBRACE:
            putchar('}');
            break;
        case TOKEN_LBRACKET:
            putchar('[');
            break;
        case TOKEN_RBRACKET:
            putchar(']');
            break;
        case TOKEN_COLON:
            putchar(':');
            break;
        case TOKEN_COMMA:
            putchar(',');
            break;
        case TOKEN_STRING:
            printf("%s", head->str);
            break;
        case TOKEN_NUMBER:
            printf("%g", head->num);
            break;
        case TOKEN_BOOL:
            if(head->b)
                printf("true");
            else
                printf("false");
            break;
        case TOKEN_NULL:
            printf("null");
            break;
        default:
            fprintf(stderr, "The format of json file were wrong somewhere.\n");
            break;
        }

        head = head->next;
        putchar('\n');
    }
}

void lexer_free(Lexer *l)
{
    free(l->src);
}

void token_free(Token *t)
{
    Token *tmp;
    while(t != NULL)
    {
        tmp = t->next;
        if(t->tag == TOKEN_STRING && t->str != NULL)
            free(t->str);
        free(t);
        t = tmp;
    }
}

#ifndef JSONVALUE_H
#define JSONVALUE_H
#include <stdbool.h>

typedef struct object Object;
typedef struct array Array;
typedef struct jsonvalue JsonValue; 

enum JSONVALUE_TAG
{
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT,
};

typedef struct jsonvalue
{
    enum JSONVALUE_TAG tag;
    union 
    {
        void *null;
        bool boolean;
        double num;
        char *str;
        Array *arr;
        Object *obj;
    };
    struct jsonvalue *next;
} JsonValue;

typedef struct object
{
    char *key;
    JsonValue *jv;
    struct object *next;
}Object;

typedef struct array
{
    JsonValue *elem;
    struct array *next;
}Array;

#endif

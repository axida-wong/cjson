#ifndef CJSON_C
#define CJSON_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsonvalue.h"
#include "./parser/parser.h"
#include "./lexer/lexer.h"

/*
 * get json value struct from the content 
 * stored in filename.
 * 
 * returns the pointer of the JsonValue 
 * struct.
 * 
 * pre: a string that store the filename
 * pos: the pointer of JsonValue 
 */
JsonValue *jsonvalue_parse(char *filename);

/*
 * free the JsonValue structure.
 * 
 * pre: jv is the pointer pointing the 
 * json value struct.
 */
extern void jsonvalue_free(JsonValue *jv);


/*
 * pretty print the JsonValue structure.
 * 
 * pre: jv is the pointer pointing the 
 *      json value struct.
 */
extern void pretty_print(JsonValue *j);

/*
 * get the value of the key-value pair.
 * equivalent to 
 * 
 * value = jsonvalue[key]
 * 
 * but just one level, not support 
 * usage like this,
 * 
 * value = jsonvalue[key1][key2]
 * 
 * pre: jv is the JsonValue pointer.
 *      key the key of the key-value pair
 * pos: return the jsonvalue pointer 
 *      that crossponding to the key. 
 */
JsonValue *get_object_value(JsonValue *jv, const char *key);

/*
 * get the value of the array.
 * equivalent to 
 * 
 * value = jsonvalue[index]
 * 
 * but just one level, not support 
 * usage like this,
 * 
 * value = jsonvalue[i1][i2]
 * 
 * pre: jv is the JsonValue pointer.
 *      index of the value that you
 *      expected.
 * pos: return the jsonvalue pointer 
 *      that at the position of index. 
 */
JsonValue *get_array_value(JsonValue *jv, int index);

/**
 * get value of the of jsonvalue that
 * not only support one level. 
 * 
 * equivalent to 
 *  jsonvalue[key1][key2][key3]
 *
 * pre: jv is the JsonValue pointer.
 *      ... is the sequence of char *,
 *      is the key sequence. but the 
 *      last char * has to be NULL.
 *      
 * post: return the jsonvalue pointer.
 */
JsonValue *get_value(JsonValue *jv, ...);

JsonValue *create_array();

JsonValue *create_object();

    
#endif
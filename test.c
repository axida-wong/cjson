#include <stdio.h>
#include <stdlib.h>

#include "jsonvalue.h"
#include "cjson.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <jsonfile>\n", argv[0]);
        return EXIT_FAILURE;
    }
    JsonValue *j;
    j = jsonvalue_parse(argv[1]);
    print_value(j);
    jsonvalue_free(j);
    return 0;
}
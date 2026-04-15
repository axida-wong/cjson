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
    // print_value(j);
    pretty_print(j);
    JsonValue *j1 = get_object_value(j, "score");
    printf("\n%f\n", j1->num);
    j1 = get_object_value(j, "bullshit");
    printf("\n%p\n", j1);
    JsonValue *j2 = get_object_value(j, "arr");
    print_array(j2, 0);
    printf("\n%d\n", j2->tag);
    JsonValue *j3 = get_array_value(j2, 0);
    printf("\n%d\n", j3->tag);
    printf("\n%g\n", j3->num);
    putchar('\n');
    JsonValue *j4 = get_value(j, "a1", "b1", NULL);
    pretty_print(j4);
    putchar('\n');
    JsonValue *j5 = get_value(j, "a1", "b2", NULL);
    print_array(j5, 0);
    putchar('\n');
    JsonValue *j6 = get_value(j, "a1", "b3", NULL);
    pretty_print(j6);
    putchar('\n');
    jsonvalue_free(j);
    return 0;
}
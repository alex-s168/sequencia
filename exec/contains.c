#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

OPERATION(contains) {
    if (input.type == SQ_NULL) {
        return SQVAL_NULL();
    }

    if (input.type == SQ_ARRAY) {
        if (arg.type != SQ_ARRAY) {
            fprintf(stderr, "Contains needs an array as argument to search for in array!\n");
            sqfree(input);
            return SQVAL_NULL();
        }

        bool found = true;
        for (size_t i = 0; i < input.arr.len; i ++) {
            for (size_t j = 0; j < arg.arr.len; j ++) {
                if (!sqeq(input.arr.items[i], arg.arr.items[i])) {
                    found = false;
                    break;
                }
            }

            if (found)
                break;
        }

        sqfree(input);
        return SQVAL_NUM(found ? 1 : 0);
    }

    if (input.type == SQ_STRING) {
        if (arg.type != SQ_STRING) {
            fprintf(stderr, "Contains needs a string as argument to search for in string!\n");
            sqfree(input);
            return SQVAL_NULL();
        }

        const char *where = strstr(input.str, arg.str);
        sqfree(input);
        return SQVAL_NUM(where == NULL ? 0 : 1);
    }

    fprintf(stderr, "Contains only operates on arrays and strings!\n");
    sqfree(input);
    return SQVAL_NULL();
}

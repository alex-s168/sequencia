#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

OPERATION(append) {
    if (input.type == SQ_ARRAY) {
        SQArr arr = input.arr;
        sqarr_add(&arr, sqdup(arg));

        return SQVAL_ARR(arr);
    }

    if (input.type == SQ_STRING) {
        if (arg.type != SQ_STRING) {
            ERR("Can only \"append\" string to string!");
            sqfree(input);
            return SQVAL_NULL();
        }

        const size_t app_len = strlen(arg.str);

        const size_t len = strlen(input.str);
        const SQStr new = realloc(input.str, len + app_len + 1);
        memcpy(new + len, arg.str, app_len);
        new[len + app_len] = '\0';

        return SQVAL_STR(new);
    }

    ERR("\"append\" only operates on arrays and strings!");
    sqfree(input);
    return SQVAL_NULL();
}

OPERATION(prepend) {
    if (input.type == SQ_ARRAY) {
        SQValue argdup = sqdup(arg);
        DynamicList_insertAt(&input.arr, 0, &argdup);
        return input;
    }

    if (input.type == SQ_STRING) {
        if (arg.type != SQ_STRING) {
            ERR("Can only \"prepend\" string to string!");
            sqfree(input);
            return SQVAL_NULL();
        }

        const size_t app_len = strlen(arg.str);
        const size_t len = strlen(input.str);
        const SQStr new = malloc(len + app_len + 1);
        memcpy(new, arg.str, app_len);
        memcpy(new + app_len, input.str, len);
        new[len + app_len] = '\0';
        free(input.str);
        return SQVAL_STR(new);
    }

    ERR("\"prepend\" only operates on arrays and strings!");
    sqfree(input);
    return SQVAL_NULL();
}

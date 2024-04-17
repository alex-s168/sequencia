#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

OPERATION(append) {
    if (input.type == SQ_ARRAY) {
        if (arg.type != SQ_ARRAY) {
            fprintf(stderr, "Can only append array to array!\n");
            sqfree(input);
            return SQVAL_NULL();
        }

        SQArr arr = input.arr;
        sqarr_add(&arr, sqdup(arg));

        return SQVAL_ARR(arr);
    }

    if (input.type == SQ_STRING) {
        if (arg.type != SQ_STRING) {
            fprintf(stderr, "Can only append string to string!\n");
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

    fprintf(stderr, "Can only append to array or string!\n");
    sqfree(input);
    return SQVAL_NULL();
}

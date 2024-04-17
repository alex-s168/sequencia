#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

OPERATION(join) {
    if (arg.type != SQ_STRING) {
        fprintf(stderr, "Can only join with strings!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    const size_t delim_len = strlen(arg.str);

    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Can only join array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    char *res = malloc(1);
    size_t res_len = 0;

    for (size_t i = 0; i < input.arr.len; i ++) {
        char *b = sqstringify(input.arr.items[i]);
        if (b == NULL)
            continue;

        if (i > 0) {
            res = realloc(res, res_len + delim_len + 1);
            memcpy(res + res_len, arg.str, delim_len);
            res_len += delim_len;
        }

        const size_t len = strlen(b);
        res = realloc(res, res_len + len + 1);
        memcpy(res + res_len, b, len);
        res_len += len;
    }

    res[res_len] = '\0';

    sqfree(input);
    return SQVAL_STR(res);
}

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

OPERATION(join) {
    if (arg.type == SQ_NULL) {
        arg = SQVAL_STR(""); // we don't free arg later so we don't need strdup
    }

    if (arg.type != SQ_STRING) {
        ERR("Can only \"join\" with string as argument!");
        sqfree(input);
        return SQVAL_NULL();
    }

    const size_t delim_len = strlen(arg.str);

    if (input.type != SQ_ARRAY) {
        ERR("Can only \"join\" arrays!");
        sqfree(input);
        return SQVAL_NULL();
    }

    char *res = malloc(1);
    size_t res_len = 0;

    bool is0 = true;
    for (size_t i = 0; i < input.arr.fixed.len; i ++) {
        SQValue val = *sqarr_at(input.arr, i);
        if (val.type == SQ_NULL)
            continue;

        char *b = sqstringify(val);
        if (b == NULL)
            continue;

        if (!is0) {
            res = realloc(res, res_len + delim_len + 1);
            memcpy(res + res_len, arg.str, delim_len);
            res_len += delim_len;
        }
        is0 = false;

        const size_t len = strlen(b);
        res = realloc(res, res_len + len + 1);
        memcpy(res + res_len, b, len);
        res_len += len;
    }

    res[res_len] = '\0';

    sqfree(input);
    return SQVAL_STR(res);
}

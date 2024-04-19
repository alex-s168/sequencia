#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(any) {
    if (input.type != SQ_ARRAY) {
        ERR("\"any\" only operates on arrays!");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.arr.fixed.len == 0) {
        sqfree(input);
        return SQVAL_NUM(0);
    }

    bool any = false;
    for (size_t i = 0; i < input.arr.fixed.len; i ++) {
        if (sqarr_at(input.arr, i)->type == SQ_NUMBER && sqarr_at(input.arr, i)->num != 0) {
            any = true;
            break;
        }
    }
    sqfree(input);
    return SQVAL_NUM(any ? 1 : 0);
}

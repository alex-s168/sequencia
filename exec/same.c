#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(same) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Same only operates on arrays!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.arr.len == 0) {
        sqfree(input);
        return SQVAL_NUM(1);
    }

    SQValue v0 = input.arr.items[0];
    bool eq = true;
    for (size_t i = 1; i < input.arr.len; i ++) {
        if (!sqeq(v0, input.arr.items[i])) {
            eq = false;
            break;
        }
    }
    sqfree(input);
    return SQVAL_NUM(eq ? 1 : 0);
}

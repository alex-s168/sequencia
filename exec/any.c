#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(any) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Any only operates on arrays!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.arr.len == 0) {
        sqfree(input);
        return SQVAL_NUM(0);
    }

    bool any = false;
    for (size_t i = 0; i < input.arr.len; i ++) {
        if (input.arr.items[i].type == SQ_NUMBER && input.arr.items[i].num != 0) {
            any = true;
            break;
        }
    }
    sqfree(input);
    return SQVAL_NUM(any ? 1 : 0);
}

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(axis) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Can only apply axis on array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.arr.len == 0) {
        fprintf(stderr, "Input to axis needs to have at least one element!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    const SQValue axis = input.arr.items[0];

    const SQArr res = sqarr_new(input.arr.len - 1);
    for (size_t i = 1; i < input.arr.len; i ++) {
        const SQArr el = sqarr_new(2);
        el.items[0] = sqdup(axis);
        el.items[1] = input.arr.items[i];
        res.items[i - 1] = SQVAL_ARR(el);
    }

    sqfree(axis);
    free(input.arr.items);
    return SQVAL_ARR(res);
}

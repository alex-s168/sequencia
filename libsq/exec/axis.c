#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(axis) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Can only apply axis on array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.arr.fixed.len == 0) {
        fprintf(stderr, "Input to axis needs to have at least one element!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    const SQValue axis = *sqarr_at(input.arr, 0);

    const SQArr res = sqarr_new(input.arr.fixed.len - 1);
    for (size_t i = 1; i < input.arr.fixed.len; i ++) {
        const SQArr el = sqarr_new(2);
        *sqarr_at(el, 0) = sqdup(axis);
        *sqarr_at(el, 1) = *sqarr_at(input.arr, i);
        *sqarr_at(res, i + 1) = SQVAL_ARR(el);
    }

    sqfree(axis);
    sqarr_free_norec(input.arr);
    return SQVAL_ARR(res);
}

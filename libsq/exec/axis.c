#include <stdbool.h>

#include "operations.h"

OPERATION(axis) {
    if (input.type != SQ_ARRAY) {
        ERR("Can only apply \"axis\" on array!");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.arr.fixed.len == 0) {
        ERR("Input to \"axis\" needs to have at least one element!");
        sqfree(input);
        return SQVAL_NULL();
    }

    const SQValue axis = *sqarr_at(input.arr, 0);
    DynamicList_removeAt(&input.arr, 0);

    for (size_t i = 0; i < input.arr.fixed.len; i ++) {
        const SQArr el = sqarr_new(2);
        *sqarr_at(el, 0) = sqdup(axis);
        *sqarr_at(el, 1) = *sqarr_at(input.arr, i);
        *sqarr_at(input.arr, i) = SQVAL_ARR(el);
    }

    sqfree(axis);
    return input;
}

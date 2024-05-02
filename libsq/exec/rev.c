#include <stdbool.h>

#include "operations.h"

OPERATION(rev) {
    if (input.type != SQ_ARRAY) {
        ERR("Can only \"rev\" elements of array!");
        sqfree(input);
        return SQVAL_NULL();
    }

    const SQArr new = sqarr_new(input.arr.fixed.len);

    for (size_t i = 0; i < input.arr.fixed.len; i ++) {
        *sqarr_at(new, input.arr.fixed.len - 1 - i) = *sqarr_at(input.arr, i);
    }

    sqarr_free_norec(input.arr);
    return SQVAL_ARR(new);
}

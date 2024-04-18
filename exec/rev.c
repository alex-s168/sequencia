#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(rev) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Can only reverse elements of array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    const SQArr new = sqarr_new(input.arr.len);

    for (size_t i = 0; i < input.arr.len; i ++) {
        new.items[input.arr.len - 1 - i] = input.arr.items[i];
    }

    free(input.arr.items);
    return SQVAL_ARR(new);
}

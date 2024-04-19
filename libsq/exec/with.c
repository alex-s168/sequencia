#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(with) {
    if (input.type == SQ_ARRAY && input.arr.fixed.len > 0) {
        const SQValue val = sqdup(*sqarr_at(input.arr, 0));
        const SQArr arr = sqarr_new(input.arr.fixed.len - 1);
        for (size_t i = 0; i < arr.fixed.len - 1; i ++)
            *sqarr_at(arr, i) = *sqarr_at(input.arr, i + 1);
        sqfree(input);
        return sqexec(SQVAL_ARR(arr), sqcommand_clone(children), val);
    }

    const SQValue r = sqexec(SQVAL_NULL(), sqcommand_clone(children), input);
    sqfree(input);
    return r;
}

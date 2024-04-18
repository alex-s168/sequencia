#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(with) {
    if (input.type == SQ_ARRAY && input.arr.len > 0) {
        const SQValue val = sqdup(input.arr.items[0]);
        const SQArr arr = sqarr_new(input.arr.len - 1);
        for (size_t i = 0; i < arr.len - 1; i ++)
            arr.items[i] = input.arr.items[i + 1];
        sqfree(input);
        return sqexec(SQVAL_ARR(arr), sqcommand_clone(children), val);
    }

    const SQValue r = sqexec(SQVAL_NULL(), sqcommand_clone(children), input);
    sqfree(input);
    return r;
}
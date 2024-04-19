#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

static void desh_rec(SQArr *dest, SQValue val) {
    if (val.type == SQ_ARRAY) {
        for (size_t i = 0; i < val.arr.fixed.len; i ++) {
            desh_rec(dest, sqdup(*sqarr_at(val.arr, i)));
        }
        return;
    }

    if (val.type == SQ_NULL)
        return;

    sqarr_add(dest, val);
}

OPERATION(deshape) {
    SQArr res = sqarr_new(0);
    desh_rec(&res, input);
    sqfree(input);
    return SQVAL_ARR(res);
}

OPERATION(flatten) {
    if (input.type != SQ_ARRAY)
        return input;

    SQArr res = sqarr_new(0);

    for (size_t i = 0; i < input.arr.fixed.len; i ++) {
        const SQValue item = *sqarr_at(input.arr, i);

        if (item.type != SQ_ARRAY) {
            sqarr_add(&res, item);
            continue;
        }

        for (size_t j = 0; j < item.arr.fixed.len; j ++)
            sqarr_add(&res, *sqarr_at(item.arr, j));
    }

    sqfree(input);
    return SQVAL_ARR(res);
}

OPERATION(flatmap) {
    SQCommand cmd;
    cmd.lines = NULL;
    cmd.lines_len = 0;
    return sqop_flatten(sqop_map(input, children, arg), cmd, SQVAL_NULL());
}

#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(group) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Can only group elements of array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.type != SQ_NUMBER) {
        fprintf(stderr, "Argument to group needs to be a number!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQArr res = sqarr_new(0);
    for (size_t i = 0; i < input.arr.fixed.len; i += arg.num) {
        SQArr temp = sqarr_new(0);
        for (size_t j = 0; j < arg.num; j ++) {
            if (i + j >= input.arr.fixed.len)
                break;
            sqarr_add(&temp, *sqarr_at(input.arr, i + j));
        }
        sqarr_add(&res, SQVAL_ARR(temp));
    }

    sqarr_free_norec(input.arr);
    return SQVAL_ARR(res);
}

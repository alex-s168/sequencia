#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    for (size_t i = 0; i < input.arr.len; i += arg.num) {
        SQArr temp = sqarr_new(0);
        for (size_t j = 0; j < arg.num; j ++) {
            if (i + j >= input.arr.len)
                break;
            sqarr_add(&temp, input.arr.items[i + j]);
        }
        sqarr_add(&res, SQVAL_ARR(temp));
    }

    free(input.arr.items);
    return SQVAL_ARR(res);
}

#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(ranges) {
    if (arg.type != SQ_ARRAY) {
        fprintf(stderr, "Arg to ranges needs to be an array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.arr.fixed.len % 2 != 0) {
        fprintf(stderr, "Arg to ranges needs to have an even amount of elements!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Ranges can only operate on arrays!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQArr res = sqarr_new(0);

    for (size_t i = 0; i < arg.arr.fixed.len; i += 2) {
        const SQValue startVal = *sqarr_at(arg.arr, i);
        const SQValue endVal = *sqarr_at(arg.arr, i + 1);

        if (startVal.type != SQ_NUMBER || endVal.type != SQ_NUMBER) {
            fprintf(stderr, "Arg to ranges needs to be an array of numbers!\n");
            continue;
        }

        SQNum start = startVal.num;
        if (start < 0)
            start = input.arr.fixed.len + start;
        if (start >= input.arr.fixed.len)
            continue;

        SQNum end = endVal.num;
        if (end < 0)
            end = input.arr.fixed.len + end;
        if (end >= input.arr.fixed.len)
            end = input.arr.fixed.len - 1;

        if (start > end) {
            for (long int j = start; j >= end; j --) {
                sqarr_add(&res, sqdup(*sqarr_at(input.arr, j)));
            }
        } else {
            for (size_t j = start; j <= end; j ++) {
                sqarr_add(&res, sqdup(*sqarr_at(input.arr, j)));
            }
        }
    }

    sqfree(input);
    return SQVAL_ARR(res);
}

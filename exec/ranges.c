#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(ranges) {
    if (arg.type != SQ_ARRAY) {
        fprintf(stderr, "Arg to ranges needs to be an array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.arr.len % 2 != 0) {
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

    for (size_t i = 0; i < arg.arr.len; i += 2) {
        const SQValue startVal = arg.arr.items[i];
        const SQValue endVal = arg.arr.items[i + 1];

        if (startVal.type != SQ_NUMBER || endVal.type != SQ_NUMBER) {
            fprintf(stderr, "Arg to ranges needs to be an array of numbers!\n");
            continue;
        }

        SQNum start = startVal.num;
        if (start < 0)
            start = input.arr.len + start;
        if (start >= input.arr.len)
            continue;

        SQNum end = endVal.num;
        if (end < 0)
            end = input.arr.len + end;
        if (end >= input.arr.len)
            end = input.arr.len - 1;

        if (start > end) {
            for (long int j = start; j >= end; j --) {
                sqarr_add(&res, sqdup(input.arr.items[j]));
            }
        } else {
            for (size_t j = start; j <= end; j ++) {
                sqarr_add(&res, sqdup(input.arr.items[j]));
            }
        }
    }

    sqfree(input);
    return SQVAL_ARR(res);
}

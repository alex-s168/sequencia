#include <stdbool.h>

#include "operations.h"

OPERATION(ranges) {
    if (arg.type != SQ_ARRAY) {
        ERR("Argument to \"ranges\" needs to be an array!");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.arr.fixed.len % 2 != 0) {
        ERR("Argument to \"ranges\" needs to have an even amount of elements!");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.type != SQ_ARRAY) {
        ERR("\"ranges\" can only operate on arrays!");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQArr res = sqarr_new(0);

    for (size_t i = 0; i < arg.arr.fixed.len; i += 2) {
        const SQValue startVal = *sqarr_at(arg.arr, i);
        const SQValue endVal = *sqarr_at(arg.arr, i + 1);

        if (startVal.type != SQ_NUMBER || endVal.type != SQ_NUMBER) {
            ERR("Argument to \"ranges\" needs to be an array of numbers!");
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
            DynamicList_reserve(&res, start - end);
            for (long int j = start; j >= end; j --) {
                sqarr_add(&res, sqdup(*sqarr_at(input.arr, j)));
            }
        } else {
            DynamicList_reserve(&res, end - start);
            for (size_t j = start; j <= end; j ++) {
                sqarr_add(&res, sqdup(*sqarr_at(input.arr, j)));
            }
        }
    }

    sqfree(input);
    return SQVAL_ARR(res);
}

#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(select) {
    if (input.type != SQ_ARRAY) {
        ERR("Can only perform \"select\" on array!");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.type == SQ_ARRAY) {
        SQArr res = sqarr_new(0);

        for (size_t i = 0; i < arg.arr.fixed.len; i ++) {
            const SQValue index = *sqarr_at(arg.arr, i);
            if (index.type != SQ_NUMBER) {
                ERR("Can only perform \"select\" with array of indecies!");
                continue;
            }
            SQNum indexid = index.num;
            if (indexid < 0)
                indexid = input.arr.fixed.len + indexid;
            if (indexid >= input.arr.fixed.len)
                continue;
            sqarr_add(&res, sqdup(*sqarr_at(input.arr, indexid)));
        }

        sqfree(input);
        return SQVAL_ARR(res);
    }

    if (arg.type == SQ_NUMBER) {
        SQNum indexid = arg.num;
        if (indexid < 0)
            indexid = input.arr.fixed.len + indexid;
        SQValue val;
        if (indexid >= input.arr.fixed.len)
            val = SQVAL_NULL();
        else
            val = sqdup(*sqarr_at(input.arr, indexid));

        sqfree(input);
        return val;
    }

    ERR("Can only perform \"select\" with array of indecies or number (index)!");
    sqfree(input);
    return SQVAL_NULL();
}

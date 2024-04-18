#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(select) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Can only perform select on array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.type == SQ_ARRAY) {
        SQArr res = sqarr_new(0);

        for (size_t i = 0; i < arg.arr.len; i ++) {
            const SQValue index = arg.arr.items[i];
            if (index.type != SQ_NUMBER) {
                fprintf(stderr, "Can only perform select with array of indecies!\n");
                continue;
            }
            SQNum indexid = index.num;
            if (indexid < 0)
                indexid = input.arr.len + indexid;
            if (indexid >= input.arr.len)
                continue;
            sqarr_add(&res, sqdup(input.arr.items[indexid]));
        }

        sqfree(input);
        return SQVAL_ARR(res);
    }

    if (arg.type == SQ_NUMBER) {
        SQNum indexid = arg.num;
        if (indexid < 0)
            indexid = input.arr.len + indexid;
        SQValue val;
        if (indexid >= input.arr.len)
            val = SQVAL_NULL();
        else
            val = sqdup(input.arr.items[indexid]);

        sqfree(input);
        return val;
    }

    fprintf(stderr, "Can only perform select with array of indecies or number (index)!\n");
    sqfree(input);
    return SQVAL_NULL();
}

#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(filter) {
    if (input.type != SQ_ARRAY) {
        ERR("Can only \"filter\" an array!");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQArr res = sqarr_new(0);

    for (size_t i = 0; i < input.arr.fixed.len; i ++) {
        const SQValue r = sqexecs(sqdup(*sqarr_at(input.arr, i)), sqcommand_clone(children));
        if (r.type == SQ_NULL)
            continue;
        if (r.type != SQ_NUMBER) {
            ERR("\"filter\" block needs to return 0 for false and any other number for true!");
            sqarr_free_rec(res);
            sqfree(input);
            return SQVAL_NULL();
        }
        if (r.num != 0) {
            sqarr_add(&res, sqdup(*sqarr_at(input.arr, i)));
        }
    }

    sqfree(input);
    return SQVAL_ARR(res);
}

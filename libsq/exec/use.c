#include <stdbool.h>

#include "operations.h"

OPERATION(use) {
    if (input.type != SQ_ARRAY) {
        ERR("\"use\" only operates on arrays!");
        sqfree(input);
        return SQVAL_NULL();
    }
    
    if (arg.type == SQ_ARRAY) {        
        SQArr args = sqarr_new(0);
        for (size_t i = 0; i < arg.arr.fixed.len; i++) {
            if (sqarr_at(arg.arr, i)->type != SQ_NUMBER)
                continue;
            SQNum index = sqarr_at(arg.arr, i)->num;
            if (index < 0)
                index = input.arr.fixed.len + index;
            if (index >= input.arr.fixed.len)
                continue;
            sqarr_add(&args, sqdup(*sqarr_at(input.arr, i)));
        }
        const SQValue res2 = sqexecs(SQVAL_ARR(args), sqcommand_clone(children));
        SQArr res = sqarr_new(1);
        *sqarr_at(res, 0) = res2;

        for (size_t i = 0; i < input.arr.fixed.len; i ++) {
            bool found = false;
            for (size_t j = 0; j < arg.arr.fixed.len; j ++) {
                if (sqarr_at(arg.arr, j)->type == SQ_NUMBER && sqarr_at(arg.arr, j)->num == i) {
                    found = true;
                    break;
                }
            }
            if (found)
                continue;
            sqarr_add(&res, sqdup(*sqarr_at(input.arr, i)));
        }

        sqfree(input);

        return SQVAL_ARR(res);
    }

    if (arg.type == SQ_NUMBER) {
        SQNum index = arg.num;
        if (index < 0)
            index = input.arr.fixed.len + index;
        if (index >= input.arr.fixed.len) {
            sqfree(input);
            return SQVAL_NULL();
        }

        const SQValue res2 = sqexecs(sqdup(*sqarr_at(input.arr, index)), sqcommand_clone(children));

        SQArr res = sqarr_new(input.arr.fixed.len);
        *sqarr_at(res, 0) = res2;

        for (size_t i = 0; i < input.arr.fixed.len; i ++) {
            if (i == index)
                continue;

            sqarr_add(&res, sqdup(*sqarr_at(input.arr, index)));
        }

        sqfree(input);

        return SQVAL_ARR(res);
    }

    ERR("\"use\" takes array of indices or index as argument!");
    sqfree(input);
    return SQVAL_NULL();
}

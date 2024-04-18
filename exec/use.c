#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(use) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Use only operates on arrays!\n");
        sqfree(input);
        return SQVAL_NULL();
    }
    
    if (arg.type == SQ_ARRAY) {
        
        SQArr args = sqarr_new(0);
        for (size_t i = 0; i < arg.arr.len; i++) {
            if (arg.arr.items[i].type != SQ_NUMBER)
                continue;
            SQNum index = arg.arr.items[i].num;
            if (index < 0)
                index = input.arr.len + index;
            if (index >= input.arr.len)
                continue;
            sqarr_add(&args, sqdup(input.arr.items[i]));
        }
        const SQValue res2 = sqexec(SQVAL_ARR(args), sqcommand_clone(children));
        SQArr res = sqarr_new(1);
        res.items[0] = res2;

        for (size_t i = 0; i < input.arr.len; i ++) {
            bool found = false;
            for (size_t j = 0; j < arg.arr.len; j ++) {
                if (arg.arr.items[j].type == SQ_NUMBER && arg.arr.items[j].num == i) {
                    found = true;
                    break;
                }
            }
            if (found)
                continue;
            sqarr_add(&res, sqdup(input.arr.items[i]));
        }

        sqfree(input);

        return SQVAL_ARR(res);
    }

    if (arg.type == SQ_NUMBER) {
        SQNum index = arg.num;
        if (index < 0)
            index = input.arr.len + index;
        if (index >= input.arr.len) {
            sqfree(input);
            return SQVAL_NULL();
        }

        const SQValue res2 = sqexec(sqdup(input.arr.items[index]), sqcommand_clone(children));

        SQArr res = sqarr_new(input.arr.len);
        res.items[0] = res2;

        for (size_t i = 0; i < input.arr.len; i ++) {
            if (i == index)
                continue;

            sqarr_add(&res, sqdup(input.arr.items[index]));
        }

        sqfree(input);

        return SQVAL_ARR(res);
    }

    fprintf(stderr, "Use takes array of indecies or index as argument!\n");
    sqfree(input);
    return SQVAL_NULL();
}
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(map) {
    if (input.type != SQ_ARRAY) {
        fprintf(stderr, "Can only map array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.type == SQ_NULL) {
        size_t writer = 0;
        for (size_t i = 0; i < input.arr.len; i ++) {
            const SQValue val = sqexecs(input.arr.items[i], sqcommand_clone(children));
            if (val.type == SQ_NULL)
                continue;
            input.arr.items[writer ++] = val;
        }
        input.arr.len = writer;

        return input;
    }

    if (arg.type == SQ_ARRAY) {
        size_t writer = 0;
        for (size_t i = 0; i < arg.arr.len; i ++) {
            if (arg.arr.items[i].type != SQ_NUMBER)
                continue;

            SQNum index = arg.arr.items[i].num;
            if (index < 0)
                index = input.arr.len + index;
            if (index >= input.arr.len)
                continue;

            const SQValue val = sqexecs(input.arr.items[index], sqcommand_clone(children));
            if (val.type == SQ_NULL)
                continue;
            input.arr.items[writer ++] = val;
        }
        input.arr.len = writer;
        return input;
    }

    if (arg.type == SQ_NUMBER) {
        SQNum index = arg.num;
        if (index < 0)
            index = input.arr.len + index;
        if (index < input.arr.len)
            input.arr.items[index] = sqexecs(input.arr.items[index], sqcommand_clone(children));
        return input;
    }

    fprintf(stderr, "Can only map index with index array or index num!\n");
    sqfree(input);
    return SQVAL_NULL();
}

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
        for (size_t i = 0; i < input.arr.len; i ++) {
            input.arr.items[i] = sqexec(input.arr.items[i], sqcommand_clone(children));
        }

        return input;
    }

    if (arg.type == SQ_ARRAY) {
        for (size_t i = 0; i < arg.arr.len; i ++) {
            if (arg.arr.items[i].type != SQ_NUMBER)
                continue;

            SQNum index = arg.arr.items[i].num;
            if (index < 0)
                index = input.arr.len + index;
            if (index < input.arr.len)
                input.arr.items[index] = sqexec(input.arr.items[index], sqcommand_clone(children));
        }
        return input;
    }

    if (arg.type == SQ_NUMBER) {
        SQNum index = arg.num;
        if (index < 0)
            index = input.arr.len + index;
        if (index < input.arr.len)
            input.arr.items[index] = sqexec(input.arr.items[index], sqcommand_clone(children));
        return input;
    }

    fprintf(stderr, "Can only map index with index array or index num!\n");
    sqfree(input);
    return SQVAL_NULL();
}

#include <stdbool.h>

#include "operations.h"

OPERATION(map) {
    if (input.type != SQ_ARRAY) {
        ERR("\"map\" only operates on arrays!");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.type == SQ_NULL) {
        size_t writer = 0;
        for (size_t i = 0; i < input.arr.fixed.len; i ++) {
            const SQValue val = sqexecs(*sqarr_at(input.arr, i), sqcommand_clone(children));
            if (val.type == SQ_NULL)
                continue;
            *sqarr_at(input.arr, writer ++) = val;
        }
        input.arr.fixed.len = writer;

        return input;
    }

    if (arg.type == SQ_ARRAY) {
        size_t writer = 0;
        for (size_t i = 0; i < arg.arr.fixed.len; i ++) {
            if (sqarr_at(arg.arr, i)->type != SQ_NUMBER)
                continue;

            SQNum index = sqarr_at(arg.arr, i)->num;
            if (index < 0)
                index = input.arr.fixed.len + index;
            if (index >= input.arr.fixed.len)
                continue;

            const SQValue val = sqexecs(*sqarr_at(input.arr, index), sqcommand_clone(children));
            if (val.type == SQ_NULL)
                continue;
            *sqarr_at(input.arr, writer ++) = val;
        }
        input.arr.fixed.len = writer;
        return input;
    }

    if (arg.type == SQ_NUMBER) {
        SQNum index = arg.num;
        if (index < 0)
            index = input.arr.fixed.len + index;
        if (index < input.arr.fixed.len)
            *sqarr_at(input.arr, index) = sqexecs(*sqarr_at(input.arr, index), sqcommand_clone(children));
        return input;
    }

    ERR("Can only perform \"map\"-index with index array or index num!");
    sqfree(input);
    return SQVAL_NULL();
}

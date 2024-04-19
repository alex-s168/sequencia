#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(make) {
    if (arg.type != SQ_NUMBER) {
        fprintf(stderr, "Argument to make needs to be length of array!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (arg.num <= 0) {
        fprintf(stderr, "Invalid array length for make!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    const size_t len = arg.num;

    const SQArr arr = sqarr_new(len);
    *sqarr_at(arr, 0) = input;
    for (size_t i = 1; i < len; i ++)
        *sqarr_at(arr, i) = sqdup(input);

    return SQVAL_ARR(arr);
}

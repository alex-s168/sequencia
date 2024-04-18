#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    arr.items[0] = input;
    for (size_t i = 1; i < len; i ++)
        arr.items[i] = sqdup(input);

    return SQVAL_ARR(arr);
}

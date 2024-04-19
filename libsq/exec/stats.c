#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "operations.h"

OPERATION(len) {
    if (input.type == SQ_ARRAY) {
        const size_t len = input.arr.fixed.len;
        sqfree(input);
        return SQVAL_NUM(len);
    }
    if (input.type == SQ_STRING) {
        const size_t len = strlen(input.str);
        sqfree(input);
        return SQVAL_NUM(len);
    }
    fprintf(stderr, "Can only get length of strings and arrays!\n");
    sqfree(input);
    return SQVAL_NULL();
}

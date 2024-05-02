#include <stdbool.h>

#include "operations.h"

OPERATION(len) {
    if (input.type == SQ_ARRAY) {
        const size_t len = input.arr.fixed.len;
        sqfree(input);
        return SQVAL_NUM(len);
    }
    if (input.type == SQ_STRING) {
        const size_t len = input.str.fixed.len;
        sqfree(input);
        return SQVAL_NUM(len);
    }
    ERR("Can only get \"len\" of strings and arrays!");
    sqfree(input);
    return SQVAL_NULL();
}

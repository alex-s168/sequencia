#include <stdbool.h>

#include "operations.h"

OPERATION(noempty) {
    if (input.type == SQ_ARRAY && input.arr.fixed.len == 0) {
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.type == SQ_STRING && input.str[0] == '\0') {
        sqfree(input);
        return SQVAL_NULL();
    }

    return input;
}

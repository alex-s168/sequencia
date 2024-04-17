#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(noempty) {
    if (input.type == SQ_ARRAY && input.arr.len == 0) {
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.type == SQ_STRING && input.str[0] == '\0') {
        sqfree(input);
        return SQVAL_NULL();
    }

    return input;
}

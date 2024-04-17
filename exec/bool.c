#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(invert) {
    if (input.type != SQ_NUMBER) {
        fprintf(stderr, "Can only invert numbers (bools)!\n");
        sqfree(input);
        return SQVAL_NULL();
    }
    return SQVAL_NUM(!input.num);
}

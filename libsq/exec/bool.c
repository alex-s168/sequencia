#include <stdbool.h>

#include "operations.h"

OPERATION(invert) {
    if (input.type != SQ_NUMBER) {
        ERR("Can only invert numbers (booleans)!");
        sqfree(input);
        return SQVAL_NULL();
    }
    return SQVAL_NUM(!input.num);
}

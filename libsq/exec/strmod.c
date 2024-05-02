#include <ctype.h>
#include <stdbool.h>

#include "operations.h"

OPERATION(tolower) {
    if (input.type != SQ_STRING) {
        ERR("Can only convert string \"tolower\"!");
        sqfree(input);
        return SQVAL_NULL();
    }

    for (size_t i = 0; i < input.str.fixed.len; i ++) {
        char *c = FixedList_get(input.str.fixed, i);
        *c = towlower(*c);
    }

    return input;
}

OPERATION(toupper) {
    if (input.type != SQ_STRING) {
        ERR("Can only convert string \"toupper\"!");
        sqfree(input);
        return SQVAL_NULL();
    }

    for (size_t i = 0; i < input.str.fixed.len; i ++) {
        char *c = FixedList_get(input.str.fixed, i);
        *c = toupper(*c);
    }

    return input;
}

#include <ctype.h>
#include <stdbool.h>

#include "operations.h"

OPERATION(tolower) {
    if (input.type != SQ_STRING) {
        ERR("Can only convert string \"tolower\"!");
        sqfree(input);
        return SQVAL_NULL();
    }

    char *ptr = input.str;
    while (*ptr != '\0') {
        *ptr = tolower(*ptr);
        ptr ++;
    }

    return input;
}

OPERATION(toupper) {
    if (input.type != SQ_STRING) {
        ERR("Can only convert string \"toupper\"!");
        sqfree(input);
        return SQVAL_NULL();
    }

    char *ptr = input.str;
    while (*ptr != '\0') {
        *ptr = toupper(*ptr);
        ptr ++;
    }

    return input;
}

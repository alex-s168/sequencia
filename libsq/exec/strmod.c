#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(tolower) {
    if (input.type != SQ_STRING) {
        fprintf(stderr, "Can only convert string to lowercase!");
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
        fprintf(stderr, "Can only convert string to uppercase!");
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

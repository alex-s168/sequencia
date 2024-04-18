#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

static SQValue filterstr_cmd(const SQValue input, int (*passes)(int c), const bool invert) {
    if (input.type != SQ_STRING) {
        fprintf(stderr, "Expected string!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    const size_t len = strlen(input.str);

    size_t filtered_len = 0;
    for (size_t i = 0; i < len; i ++) {
        filtered_len += passes(input.str[i]);
    }

    char *new = malloc(len + 1);
    char *ptr = new;
    for (size_t i = 0; i < len; i ++) {
        if (passes(input.str[i]) ^ invert)
            *ptr++ = input.str[i];
    }
    *ptr = '\0';
    sqfree(input);
    return SQVAL_STR(new);
}

OPERATION(falpha) {
    return filterstr_cmd(input, isalpha, false);
}

OPERATION(fdigit) {
    return filterstr_cmd(input, isdigit, false);
}

OPERATION(falnum) {
    return filterstr_cmd(input, isalnum, false);
}

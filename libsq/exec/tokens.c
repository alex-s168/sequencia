#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(tokens) {
    if (input.type != SQ_STRING) {
        ERR("Can only get \"tokens\" of string!");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQArr res = sqarr_new(0);

    char *acc = malloc(1);
    acc[0] = '\0';
    size_t acc_len = 0;

    const char *src = input.str;
    while (*src != '\0') {
        bool add_later = false;
        char c = *src++;

        if (isspace(c))
            goto push;

        if (acc[0] == '\0')
            goto add;

        if (isalpha(acc[0])) {
            if (isalpha(c))
                goto add;
            add_later = true;
            goto push;
        }

        if (isdigit(acc[0])) {
            if (isdigit(c))
                goto add;
            add_later = true;
            goto push;
        }

        add_later = true;
        goto push;

        continue;
        add:
            acc = realloc(acc, acc_len + 2);
        acc[acc_len ++] = c;
        continue;
        push:
            acc[acc_len] = '\0';
        sqarr_add(&res, SQVAL_STR(acc));
        if (add_later) {
            acc = malloc(2);
            acc[0] = c;
            acc[1] = '\0';
            acc_len = 1;
        }
        else {
            acc = malloc(1);
            acc[0] = '\0';
            acc_len = 0;
        }
    }

    sqarr_add(&res, SQVAL_STR(acc));

    sqfree(input);
    return SQVAL_ARR(res);
}

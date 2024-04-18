#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "operations.h"
#include "../../minilibs/utils.h"

OPERATION(split) {
    if (arg.type == SQ_NULL) {
        arg = SQVAL_STR(""); // we don't dup because it will never be freed
    }

    if (arg.type != SQ_STRING) {
        fprintf(stderr, "Can only split with string delimeter argument!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.type != SQ_STRING) {
        fprintf(stderr, "Can only split strings!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQArr arr = sqarr_new(0);

    if (arg.str[0] == '\0') {
        char *str = input.str;
        while (*str != '\0') {
            static char buf[2];
            buf[0] = *str;
            buf[1] = '\0';
            sqarr_add(&arr, SQVAL_STR(strdup(buf)));
            str ++;
        }
    }
    else {
        SPLITERATE(input.str, arg.str, elem) {
            sqarr_add(&arr, SQVAL_STR(strdup(elem)));
        }
    }

    sqfree(input);
    return SQVAL_ARR(arr);
}

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(parse) {
    if (input.type != SQ_STRING) {
        fprintf(stderr, "Can only parse string!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    int base = 10;
    if (arg.type == SQ_NUMBER) {
        base = arg.num;
    }

    char *end;
    const SQNum num = strtol(input.str, &end, base);
    if (*end) {
        sqfree(input);
        return SQVAL_NULL();
    }

    sqfree(input);
    return SQVAL_NUM(num);
}

OPERATION(str) {
    char *res = sqstringify(input);

    sqfree(input);
    return SQVAL_STR(res);
}

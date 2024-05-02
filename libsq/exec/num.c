#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

OPERATION(parse) {
    if (input.type != SQ_STRING) {
        fprintf(stderr, "Can only parse string!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    size_t end;
    SQValue res = sqparse(zview(input.str), &end);

    sqfree(input);
    return res;
}

OPERATION(str) {
    SQStr res = sqstringify(input);
    sqfree(input);
    return SQVAL_STR(res);
}

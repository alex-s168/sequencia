#include <stdbool.h>

#include "operations.h"

OPERATION(append) {
    if (input.type == SQ_ARRAY) {
        SQArr arr = input.arr;
        sqarr_add(&arr, sqdup(arg));

        return SQVAL_ARR(arr);
    }

    if (input.type == SQ_STRING) {
        if (arg.type != SQ_STRING) {
            ERR("Can only \"append\" string to string!");
            sqfree(input);
            return SQVAL_NULL();
        }

        SQStr str = input.str;

       if (DynamicList_addAll(&str, arg.str.fixed.data, arg.str.fixed.len))
            ERR("Error adding to list!");

       return SQVAL_STR(str);
    }

    ERR("\"append\" only operates on arrays and strings!");
    sqfree(input);
    return SQVAL_NULL();
}

OPERATION(prepend) {
    if (input.type == SQ_ARRAY) {
        SQValue argdup = sqdup(arg);
        DynamicList_insertAt(&input.arr, 0, &argdup);
        return input;
    }

    if (input.type == SQ_STRING) {
        if (arg.type != SQ_STRING) {
            ERR("Can only \"prepend\" string to string!");
            sqfree(input);
            return SQVAL_NULL();
        }

        SQStr str = input.str;

        if (DynamicList_insertAllAt(&str, 0, arg.str.fixed.data, arg.str.fixed.len))
            ERR("Error adding to list!");

        return SQVAL_STR(str);
    }

    ERR("\"prepend\" only operates on arrays and strings!");
    sqfree(input);
    return SQVAL_NULL();
}

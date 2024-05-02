#include <stdbool.h>

#include "operations.h"

OPERATION(contains) {
    if (input.type == SQ_NULL) {
        return SQVAL_NULL();
    }

    if (input.type == SQ_ARRAY) {
        if (arg.type != SQ_ARRAY) {
            ERR("\"contains\" needs an array as argument to search for in array!");
            sqfree(input);
            return SQVAL_NULL();
        }

        bool found = true;
        for (size_t i = 0; i < input.arr.fixed.len; i ++) {
            for (size_t j = 0; j < arg.arr.fixed.len; j ++) {
                if (!sqeq(*sqarr_at(input.arr, i), *sqarr_at(arg.arr, i))) {
                    found = false;
                    break;
                }
            }

            if (found)
                break;
        }

        sqfree(input);
        return SQVAL_NUM(found ? 1 : 0);
    }

    if (input.type == SQ_STRING) {
        if (arg.type != SQ_STRING) {
            ERR("\"contains\" needs a string as argument to search for in string!");
            sqfree(input);
            return SQVAL_NULL();
        }

        bool found = true;
        for (size_t i = 0; i < input.str.fixed.len; i ++) {
            for (size_t j = 0; j < arg.str.fixed.len; j ++) {
                char a = *(char*)FixedList_get(input.str.fixed, i);
                char b = *(char*)FixedList_get(arg.str.fixed, i);
                if (a != b) {
                    found = false;
                    break;
                }
            }

            if (found)
                break;
        }

        sqfree(input);
        return SQVAL_NUM(found ? 1 : 0);
    }

    ERR("\"contains\" only operates on arrays and strings!");
    sqfree(input);
    return SQVAL_NULL();
}

#include <stdbool.h>

#include "operations.h"

OPERATION(split) {
    if (arg.type == SQ_NULL) {
        arg = SQVAL_STR(zempty()); // we don't dup because it will never be freed and we don't need to free because it won't do any allocation
    }

    if (arg.type != SQ_STRING) {
        ERR("Can only \"split\" with string delimiter argument!");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.type != SQ_STRING) {
        ERR("Can only \"split\" strings!");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQArr arr = sqarr_new(0);

    if (arg.str.fixed.len == 0) {
        for (size_t i = 0; i < input.str.fixed.len; i ++) {
            char c = *(char*)FixedList_get(input.str.fixed, i);
            SQStr new = zempty();
            DynamicList_add(&new, &c);
            sqarr_add(&arr, SQVAL_STR(new));
        }
    }
    else {
        SQStr acc = zempty();
        for (size_t i = 0; i < input.str.fixed.len; i ++) {
            char c = *(char*)FixedList_get(input.str.fixed, i);
            DynamicList_add(&acc, &c);

            bool match = true;
            for (size_t i = 0; i < arg.str.fixed.len; i ++) {
                if (i >= acc.fixed.len) {
                    match = false;
                    break;
                }

                if (*(char*)FixedList_get(acc.fixed, i) != *(char*)FixedList_get(arg.str.fixed, i)) {
                    match = false;
                    break;
                }
            }

            if (match) {
                sqarr_add(&arr, SQVAL_STR(acc));
                acc = zempty();
            }
        }
    }

    sqfree(input);
    return SQVAL_ARR(arr);
}

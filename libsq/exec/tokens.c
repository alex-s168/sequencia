#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "operations.h"

OPERATION(tokens) {
    if (input.type != SQ_STRING) {
        ERR("Can only get \"tokens\" of string!");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQArr res = sqarr_new(0);

    SQStr acc = zempty();
#define ACC(n) *(char*)FixedList_get(acc.fixed, n)

    for (size_t i = 0; i < input.str.fixed.len; i ++) {
        bool add_later = false;
        char c = *(char*)FixedList_get(input.str.fixed, i);
        i++;

        if (isspace(c))
            goto push;

        if (ACC(0) == '\0')
            goto add;

        if (isalpha(ACC(0))) {
            if (isalpha(c))
                goto add;
            add_later = true;
            goto push;
        }

        if (isdigit(ACC(0))) {
            if (isdigit(c))
                goto add;
            add_later = true;
            goto push;
        }

        add_later = true;
        goto push;

        continue;
        add:
            DynamicList_add(&acc, &c);
        continue;
        push:
        sqarr_add(&res, SQVAL_STR(acc));
        if (add_later) {
            acc = zempty();
            DynamicList_add(&acc, &c);
        }
        else {
            acc = zempty();
        }
    }

    sqarr_add(&res, SQVAL_STR(acc));

    sqfree(input);
    return SQVAL_ARR(res);
}

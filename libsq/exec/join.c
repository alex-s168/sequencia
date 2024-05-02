#include <stdbool.h>

#include "operations.h"

OPERATION(join) {
    if (arg.type == SQ_NULL) {
        arg = SQVAL_STR(zempty()); // we don't free arg later so we don't need zdup
    }

    if (arg.type != SQ_STRING) {
        ERR("Can only \"join\" with string as argument!");
        sqfree(input);
        return SQVAL_NULL();
    }

    if (input.type != SQ_ARRAY) {
        ERR("Can only \"join\" arrays!");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQStr res = zempty();

    bool is0 = true;
    for (size_t i = 0; i < input.arr.fixed.len; i ++) {
        SQValue val = *sqarr_at(input.arr, i);
        if (val.type == SQ_NULL)
            continue;

        SQStr b = sqstringify(val);

        if (!is0) {
            DynamicList_addAll(&res, arg.str.fixed.data, arg.str.fixed.len);
        }
        is0 = false;

        DynamicList_addAll(&res, b.fixed.data, b.fixed.len);
        zfree(b);
    }

    sqfree(input);
    return SQVAL_STR(res);
}

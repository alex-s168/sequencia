#include <ctype.h>
#include <stdbool.h>

#include "operations.h"

static SQValue filterstr_cmd(const SQValue input, int (*passes)(int c), const bool invert) {
    if (input.type != SQ_STRING) {
        ERR("expected string!");
        sqfree(input);
        return SQVAL_NULL();
    }

    SQStr new = zempty();
    for (size_t i = 0; i < input.str.fixed.len; i ++) {
        char c = *(char*)FixedList_get(input.str.fixed, i);
        bool pass = passes(c);
        if ((pass && !invert) || (!pass && invert))
            DynamicList_add(&new, &c);
    }

    sqfree(input);
    return SQVAL_STR(new);
}

#define FN(name,impl) OPERATION(f##name) { return filterstr_cmd(input, impl, false); } OPERATION(f##n##name) { return filterstr_cmd(input, impl, true); }

FN(alpha, isalpha)
FN(digit, isdigit);
FN(alnum, isalnum);
FN(punct, ispunct);
FN(white, isspace);

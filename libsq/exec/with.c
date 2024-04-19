#include <stdbool.h>

#include "operations.h"

OPERATION(with) {
    if (input.type == SQ_ARRAY && input.arr.fixed.len > 0) {
        const SQValue val = sqdup(*sqarr_at(input.arr, 0));
        DynamicList_removeAt(&input.arr, 0);
        return sqexec(input, sqcommand_clone(children), val);
    }

    const SQValue r = sqexec(SQVAL_NULL(), sqcommand_clone(children), input);
    sqfree(input);
    return r;
}

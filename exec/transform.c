#include <stdbool.h>

#include "operations.h"

OPERATION(transform) {
    const SQArr res = sqarr_new(2);
    res.items[0] = sqdup(input);
    res.items[1] = sqexec(sqdup(input), sqcommand_clone(children));
    sqfree(input);
    return SQVAL_ARR(res);
}

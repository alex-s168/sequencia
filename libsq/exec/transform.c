#include <stdbool.h>

#include "operations.h"

OPERATION(transform) {
    const SQArr res = sqarr_new(2);
    *sqarr_at(res, 0) = sqdup(input);
    *sqarr_at(res, 1) = sqexecs(sqdup(input), sqcommand_clone(children));
    sqfree(input);
    return SQVAL_ARR(res);
}

#include <stdlib.h>

#include "analysis.h"

void sqaerrors_clean(SQAErrors *errs) {
    for (size_t i = 0; i < errs->fixed.len; i ++) {
        SQAError *err = (SQAError *) FixedList_get(errs->fixed, i);
        free(err->msg);
    }
    DynamicList_clear(errs);
}

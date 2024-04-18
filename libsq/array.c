#include <stdlib.h>

#include "sequencia.h"

SQArr sqarr_new(size_t len) {
    struct DynamicList li;
    DynamicList_init(&li, sizeof(SQValue), getLIBCAlloc(), len);
    return li;
}

void sqarr_add(SQArr *arr, SQValue val) {
    DynamicList_add(arr, &val);
}

void sqarr_reserve(SQArr *arr, size_t additional) {
    DynamicList_reserve(arr, additional);
}

void sqarr_free_rec(SQArr arr) {
    for (size_t i = 0; i < arr.fixed.len; i ++) {
        SQValue elem = *(SQValue*)FixedList_get(arr.fixed, i);
        if (elem.type == SQ_ARRAY)
            sqarr_free_rec(elem.arr);
    }
    DynamicList_clear(&arr);
}

void sqarr_free_norec(SQArr arr) {
    DynamicList_clear(&arr);
}

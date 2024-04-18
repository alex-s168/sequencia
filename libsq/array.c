#include <stdlib.h>
#include <string.h>

#include "sequencia.h"

SQArr sqarr_new(size_t len) {
    SQArr res;
    res.items = malloc(sizeof(SQValue) * len);
    memset(res.items, 0, sizeof(SQValue) * len);
    res.len = len;
    return res;
}

void sqarr_add(SQArr *arr, SQValue val) {
    arr->items = realloc(arr->items, sizeof(SQValue) * (arr->len + 1));
    arr->items[arr->len ++] = val;
}

void sqarr_free_rec(SQArr arr) {
    for (size_t i = 0; i < arr.len; i ++)
        if (arr.items[i].type == SQ_ARRAY)
            sqarr_free_rec(arr.items[i].arr);

    free(arr.items);
}

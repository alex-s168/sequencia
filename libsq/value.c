#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "sequencia.h"

SQValue sqparse(SQStrView str, size_t *end) {
    size_t index = 0;
#define CURRN(o) ((index + o) >= str.fixed.len ? '\0' : *(char*)FixedList_get(str.fixed, index + o))
#define CURR CURRN(0)

    while (CURR == ' ')
        index ++;

    if (CURR == '[') {
        index ++;
        SQArr arr = sqarr_new(0);
        while (true) {
            size_t temp;
            SQStrView toparse = str;
            toparse.fixed.data += sizeof(char) * index;
            toparse.fixed.len -= index;
            const SQValue val = sqparse(toparse, &temp);
            sqarr_add(&arr, val);
            index += temp; 
            if (CURR != ',')
                break;
            index ++;
        }
        if (CURR == ']')
            index ++;
        *end = index;
        return SQVAL_ARR(arr);
    }

    if (CURR == '"') {
        index ++;
        int en = FixedList_indexOfLast(str.fixed, (char[]) { '"' }) - index;
        if (en < 0) {
            *end = index;
            return SQVAL_NULL();
        }

        SQStrView view = str;
        view.fixed.data += sizeof(char) * index;
        view.fixed.len = en;
        const SQStr res = zdupv(view);
        *end = index;
        return SQVAL_STR(res);
    }

    static char buf[256];
    memcpy(buf, str.fixed.data + sizeof(char) * index, str.fixed.len - index);
    buf[str.fixed.len - index + 1] = '\0';

    char *endd;
    const SQNum num = strtol(buf, &endd, 10);
    if (endd == buf) {
        if (CURRN(0) == 'n' && CURRN(1) == 'l') {
            *end = index + 2;
            return SQVAL_STR(zdupc("\n"));
        }
        if (CURRN(0) == 't' && CURRN(1) == 'a' && CURRN(2) == 'b') {
            *end = index + 3;
            return SQVAL_STR(zdupc("\t"));
        }
        if (CURRN(0) == 'n' && CURRN(1) == 'u' && CURRN(2) == 'l' && CURRN(3) == 'l') {
            *end = index + 4;
            return SQVAL_NULL();
        }
        return SQVAL_NULL();
    }

    return SQVAL_NUM(num);
}

SQStr sqstringify(SQValue val) {
    switch (val.type) {
        case SQ_NULL: {
            return zempty();
        }

        case SQ_NUMBER: {
            static char buf[256];
            sprintf(buf, "%lld", val.num);
            return zdupc(buf);
        }

        case SQ_STRING: {
            return zdup(val.str);
        }

        case SQ_ARRAY: {
            if (val.arr.fixed.len == 0)
                return zempty();
            fprintf(stderr, "Can't automatically join array!\n");
            return zempty();
        }
    }
    assert(false);
}

void sqoutput(SQValue *val, FILE *out, bool debug, bool ptrs, size_t indent) {
    switch (val->type) {
        case SQ_NULL:
            if (debug) {
                fprintf(out, "(null)");
            }
            break;

        case SQ_NUMBER: {
            fprintf(out, "%lld", val->num);
        }
        break;

        case SQ_STRING: {
            zterminate(&val->str);
            char *str = val->str.fixed.data;
            if (debug) {
                if (ptrs)
                    fprintf(out, "0x%p=\"%s\"", str, str);
                else
                    fprintf(out, "\"%s\"", str);
            }
            else {
                fprintf(out, "%s", str);
            }
        }
        break;

        case SQ_ARRAY: {
            if (debug) {
                if (ptrs)
                    fprintf(out, "0x%p=[\n", val->arr.fixed.data);
                else
                    fprintf(out, "[\n");
                for (size_t i = 0; i < val->arr.fixed.len; i ++) {
                    if (i > 0)
                        fprintf(out, ",\n");
                    for (size_t j = 0; j < indent + 1; j ++)
                        fputs("  ", out);
                    sqoutput(sqarr_at(val->arr, i), out, true, ptrs, indent + 1);
                }
                fputc('\n', out);
                for (size_t j = 0; j < indent; j ++)
                    fputs("  ", out);
                fputc(']', out);
            }
            else {
                ERR("Cannot print array! Try to run with \"-d\" (or \"--debug-output\")\n");
            }
        }
        break;
    }
}

void sqfree(SQValue val) {
    if (val.type == SQ_ARRAY)
        sqarr_free_rec(val.arr);
    else if (val.type == SQ_STRING)
        zfree(val.str);
}

SQValue sqdup(const SQValue val) {
    switch (val.type) {
        case SQ_NULL:
        case SQ_NUMBER:
            return val;

        case SQ_STRING:
            return SQVAL_STR(zdup(val.str));

        case SQ_ARRAY: {
            const SQArr res = sqarr_new(val.arr.fixed.len);
            for (size_t i = 0; i < val.arr.fixed.len; i ++)
                *sqarr_at(res, i) = sqdup(*sqarr_at(val.arr, i));
            return SQVAL_ARR(res);
        }

        default:
            assert(false);
    }
}

bool sqeq(const SQValue a, const SQValue b) {
    if (a.type != b.type)
        return false;

    switch (a.type) {
        case SQ_NULL:
            return true;

        case SQ_NUMBER:
            return a.num == b.num;

        case SQ_STRING:
            return zequal(a.str, b.str);

        case SQ_ARRAY: {
            if (a.arr.fixed.len != b.arr.fixed.len)
                return false;
            bool eq = true;
            for (size_t i = 0; i < a.arr.fixed.len; i ++) {
                if (!sqeq(*sqarr_at(a.arr, i), *sqarr_at(b.arr, i))) {
                    eq = false;
                    break;
                }
            }
            return eq;
        }

        default:
            assert(false);
    }
}

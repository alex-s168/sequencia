#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sequencia.h"

SQValue sqparse(char *str, char **end) {
    while (*str == ' ')
        str ++;

    if (*str == '[') {
        str ++;
        SQArr arr = sqarr_new(0);
        while (true) {
            char *temp;
            const SQValue val = sqparse(str, &temp);
            sqarr_add(&arr, val);
            str = temp;
            if (*str != ',')
                break;
            str ++;
        }
        if (*str == ']')
            str ++;
        *end = str;
        return SQVAL_ARR(arr);
    }

    if (*str == '"') {
        str ++;
        const char *start = str;
        while (*str != '\0') {
            if (*str == '"') {
                *str = '\0';
                str ++;
                break;
            }
            str ++;
        }
        const SQStr res = strdup(start);
        *end = str;
        return SQVAL_STR(res);
    }

    const SQNum num = strtol(str, end, 10);
    if (*end == str) {
        if (str[0] == 'n' && str[1] == 'l') {
            *end = str + 2;
            return SQVAL_STR(strdup("\n"));
        }
        if (str[0] == 't' && str[1] == 'a' && str[2] == 'b') {
            *end = str + 3;
            return SQVAL_STR(strdup("\t"));
        }
        if (str[0] == 'n' && str[1] == 'u' && str[2] == 'l' && str[3] == 'l') {
            *end = str + 4;
            return SQVAL_NULL();
        }
        return SQVAL_NULL();
    }

    return SQVAL_NUM(num);
}

char *sqstringify(SQValue val) {
    switch (val.type) {
        case SQ_NULL: {
            return NULL;
        }

        case SQ_NUMBER: {
            static char buf[256];
            sprintf(buf, "%lld", val.num);
            return strdup(buf);
        }

        case SQ_STRING: {
            return strdup(val.str);
        }

        case SQ_ARRAY: {
            fprintf(stderr, "Can't automatically join array!\n");
            return NULL;
        }
    }
    assert(false);
}

void sqoutput(SQValue val, FILE *out, bool debug, bool ptrs, size_t indent) {
    switch (val.type) {
        case SQ_NULL:
            if (debug) {
                fprintf(out, "(null)");
            }
            break;

        case SQ_NUMBER: {
            fprintf(out, "%lld", val.num);
        }
        break;

        case SQ_STRING: {
            if (debug) {
                if (ptrs)
                    fprintf(out, "0x%p=\"%s\"", val.str, val.str);
                else
                    fprintf(out, "\"%s\"", val.str);
            }
            else {
                fprintf(out, "%s", val.str);
            }
        }
        break;

        case SQ_ARRAY: {
            if (debug) {
                if (ptrs)
                    fprintf(out, "0x%p=[\n", val.arr.items);
                else
                    fprintf(out, "[\n");
                for (size_t i = 0; i < val.arr.len; i ++) {
                    if (i > 0)
                        fprintf(out, ",\n");
                    for (size_t j = 0; j < indent + 1; j ++)
                        fputs("  ", out);
                    sqoutput(val.arr.items[i], out, true, ptrs, indent + 1);
                }
                fputc('\n', out);
                for (size_t j = 0; j < indent; j ++)
                    fputs("  ", out);
                fputc(']', out);
            }
            else {
                fprintf(stderr, "Cannot print array! Try to run with `--debug`\n");
            }
        }
        break;
    }
}

void sqfree(SQValue val) {
    if (val.type == SQ_ARRAY)
        sqarr_free_rec(val.arr);
    else if (val.type == SQ_STRING)
        free(val.str);
}

SQValue sqdup(const SQValue val) {
    switch (val.type) {
        case SQ_NULL:
        case SQ_NUMBER:
            return val;

        case SQ_STRING:
            return SQVAL_STR(strdup(val.str));

        case SQ_ARRAY: {
            const SQArr res = sqarr_new(val.arr.len);
            for (size_t i = 0; i < val.arr.len; i ++)
                res.items[i] = sqdup(val.arr.items[i]);
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
            return strcmp(a.str, b.str) == 0;

        case SQ_ARRAY: {
            if (a.arr.len != b.arr.len)
                return false;
            bool eq = true;
            for (size_t i = 0; i < a.arr.len; i ++) {
                if (!sqeq(a.arr.items[i], b.arr.items[i])) {
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

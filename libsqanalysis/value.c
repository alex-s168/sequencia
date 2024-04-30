#include "analysis.h"
#include <stdlib.h>

SQValue sqa_analyze_value(const char *str, size_t info_line, size_t info_col, SQAErrors *dest) {
    char *end;
    char *copy = strdup(str);
    size_t len = strlen(copy);
    SQValue val = sqparse(copy, &end);
    
    if (*end != '\0') {
        SQAError err;
        err.line = info_line;
        err.col = info_col + (end - copy);
        err.len = len - (end - copy);
        err.msg = strdup("Unexpected token!");
        DynamicList_add(dest, &err);
    }
    else if (val.type == SQ_NULL) {
        SQAError err;
        err.line = info_line;
        err.col = info_col;
        err.len = len;
        err.msg = strdup("Invalid expression!");
        DynamicList_add(dest, &err);
    }

    free(copy);

    return val;
}

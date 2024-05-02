#include "analysis.h"

SQValue sqa_analyze_value(const char *str, size_t info_line, size_t info_col, SQAErrors *dest) {
    size_t len = strlen(str);
    size_t end;
    SQValue val = sqparse(zviewc(str), &end);
    
    if (end != len) {
        SQAError err;
        err.line = info_line;
        err.col = info_col + end;
        err.len = len - end;
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

    return val;
}

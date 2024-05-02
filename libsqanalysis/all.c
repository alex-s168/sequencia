#include "analysis.h"

void sqa_analyze(SQAScript script, SQAErrors *dest) {
    for (size_t i = 0; i < script.lines.len; i ++) {
        const char *line = *(const char **)FixedList_get(script.lines, i);

        char *op = strdup(line);
        size_t in = indent_ascii(op);
        op += in;

        size_t len = strlen(op);

        const char *comment = strchr(op, '#');
        if (comment != NULL) {
            SQAError err;
            err.line = i;
            err.col = comment - op;
            err.len = len - err.col;
            err.msg = strdup("Comments not yet supported!");
            DynamicList_add(dest, &err);
        }

        size_t op_len = len;
        size_t arg_len = 0;

        char *space = strchr(op, ' ');
        SQValue val = SQVAL_NULL();
        if (space != NULL) {
            op_len = space - op;
            *space = '\0';
            char *arg = space + 1;
            arg_len = len - op_len;
            val = sqa_analyze_value(arg, i, op_len + in, dest);
        }

        SQAOp *opp = sqa_op_lookup(op);
        if (opp == NULL) {
            SQAError err;
            err.line = i;
            err.col = in;
            err.len = op_len;
            err.msg = strdup("Invalid operation!");
            DynamicList_add(dest, &err);
        }
        else {
            if (!sqa_op_accepts(*opp, val)) {
                SQAError err;
                err.line = i;
                err.col = op_len + in;
                err.len = arg_len;
                err.msg = strdup("Invalid argument!");
                DynamicList_add(dest, &err);
            }
        }
    }
}

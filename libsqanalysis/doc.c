#include <string.h>
#include "analysis.h"

SQAOp *sqa_op_lookup(const char *name) {
    for (size_t i = 0; true; i ++) {
        SQAOp *op = &sqa_ops[i];

        if (op->name == NULL)
            return NULL;

        if (strcmp(name, op->name) == 0)
            return op;
    }
}

SQAOp sqa_ops[] = {
    (SQAOp) { "any", "any" },
    (SQAOp) { "append", "append" },
    (SQAOp) { "axis", "axis" },
    (SQAOp) { "contains", "contains" },
    (SQAOp) { "deshape", "deshape" },
    (SQAOp) { "falpha", "falpha fdigit falnum fpunct fwhite" },
    (SQAOp) { "fdigit", "falpha fdigit falnum fpunct fwhite" },
    (SQAOp) { "falnum", "falpha fdigit falnum fpunct fwhite" },
    (SQAOp) { "fpunct", "falpha fdigit falnum fpunct fwhite" },
    (SQAOp) { "fwhite", "falpha fdigit falnum fpunct fwhite" },
    (SQAOp) { "fnalpha", "fnalpha fndigit fnalnum fnpunct fnwhite" },
    (SQAOp) { "fndigit", "fnalpha fndigit fnalnum fnpunct fnwhite" },
    (SQAOp) { "fnalnum", "fnalpha fndigit fnalnum fnpunct fnwhite" },
    (SQAOp) { "fnpunct", "fnalpha fndigit fnalnum fnpunct fnwhite" },
    (SQAOp) { "fnwhite", "fnalpha fndigit fnalnum fnpunct fnwhite" },
    (SQAOp) { "filter", "filter" },
    (SQAOp) { "flatmap", "flatmap" },
    (SQAOp) { "flatten", "flatten" },
    (SQAOp) { "group", "group" },
    (SQAOp) { "invert", "invert" },
    (SQAOp) { "join", "join" },
    (SQAOp) { "len", "len" },
    (SQAOp) { "make", "make" },
    (SQAOp) { "map", "map" },
    (SQAOp) { "noempty", "noempty" },
    (SQAOp) { "parse", "parse" },
    (SQAOp) { "prepend", "prepend" },
    (SQAOp) { "ranges", "ranges" },
    (SQAOp) { "rev", "rev" },
    (SQAOp) { "run", "run" },
    (SQAOp) { "same", "same" },
    (SQAOp) { "select", "select" },
    (SQAOp) { "slit", "split" },
    (SQAOp) { "str", "str" },
    (SQAOp) { "tokens", "tokens" },
    (SQAOp) { "tolower", "tolower toupper" },
    (SQAOp) { "toupper", "tolower toupper" },
    (SQAOp) { "transform", "transform" },
    (SQAOp) { "use", "use" },
    (SQAOp) { "with", "with" },

    (SQAOp) { NULL, NULL },
};

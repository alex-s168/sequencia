#include <string.h>
#include "analysis.h"

SQAOp *sqa_op_lookup(const char *name) {
    size_t i = 0;
    while (true) {
        SQAOp *op = &sqa_ops[i];

        if (op->name == NULL)
            return NULL;

        if (strcmp(name, op->name) == 0)
            return op;

        i ++;
    }
}

SQAOp sqa_ops[] = {
    { .name = "any", .docPage = "any" },
    { .name = "append", .docPage = "append" },
    { .name = "axis", .docPage = "axis" },
    { .name = "contains", .docPage = "contains" },
    { .name = "deshape", .docPage = "deshape" },
    { .name = "falpha", .docPage = "falpha fdigit falnum fpunct fwhite" },
    { .name = "fdigit", .docPage = "falpha fdigit falnum fpunct fwhite" },
    { .name = "falnum", .docPage = "falpha fdigit falnum fpunct fwhite" },
    { .name = "fpunct", .docPage = "falpha fdigit falnum fpunct fwhite" },
    { .name = "fwhite", .docPage = "falpha fdigit falnum fpunct fwhite" },
    { .name = "fnalpha", .docPage = "fnalpha fndigit fnalnum fnpunct fnwhite" },
    { .name = "fndigit", .docPage = "fnalpha fndigit fnalnum fnpunct fnwhite" },
    { .name = "fnalnum", .docPage = "fnalpha fndigit fnalnum fnpunct fnwhite" },
    { .name = "fnpunct", .docPage = "fnalpha fndigit fnalnum fnpunct fnwhite" },
    { .name = "fnwhite", .docPage = "fnalpha fndigit fnalnum fnpunct fnwhite" },
    { .name = "filter", .docPage = "filter" },
    { .name = "flatmap", .docPage = "flatmap" },
    { .name = "flatten", .docPage = "flatten" },
    { .name = "group", .docPage = "group" },
    { .name = "invert", .docPage = "invert" },
    { .name = "join", .docPage = "join" },
    { .name = "len", .docPage = "len" },
    { .name = "make", .docPage = "make" },
    { .name = "map", .docPage = "map" },
    { .name = "noempty", .docPage = "noempty" },
    { .name = "parse", .docPage = "parse" },
    { .name = "prepend", .docPage = "prepend" },
    { .name = "ranges", .docPage = "ranges" },
    { .name = "rev", .docPage = "rev" },
    { .name = "run", .docPage = "run" },
    { .name = "same", .docPage = "same" },
    { .name = "select", .docPage = "select" },
    { .name = "slit", .docPage = "split" },
    { .name = "str", .docPage = "str" },
    { .name = "tokens", .docPage = "tokens" },
    { .name = "tolower", .docPage = "tolower toupper" },
    { .name = "toupper", .docPage = "tolower toupper" },
    { .name = "transform", .docPage = "transform" },
    { .name = "use", .docPage = "use" },
    { .name = "with", .docPage = "with" },

    { .name = NULL, .docPage = NULL },
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sequencia.h"

#include "exec/operations.h"
#undef OPERATION

SQValue sqexec_single(SQValue input, const char *command, SQCommand children, SQValue arg) {
#define OPERATION(name) if (strcmp(command, #name) == 0) \
    return sqop_##name(input, children, arg);

#include "exec/operations.h"
#undef OPERATION

    fprintf(stderr, "Unknown command \"%s\"!\n", command);
    sqfree(input);
    return SQVAL_NULL();
}

SQValue sqexec(SQValue input, SQCommand cmd, SQValue arg_override) {
    SQValue acc = input;
    SQITER(cmd, item, {
        if (item.cmd[0] == '\0')
            continue;

        char *argstart = strchr(item.cmd, ' ');
        char *args = "";
        if (argstart != NULL) {
            *argstart = '\0';
            args = argstart + 1;
        }
        const char *command = item.cmd;
        char *end;
        SQValue arg = sqparse(args, &end);
        if (arg.type == SQ_NULL)
            arg = arg_override;
        acc = sqexec_single(acc, command, item.children, arg);
    });
    for (size_t i = 0; i < cmd.lines_len; i ++) {
        free(cmd.lines[i]);
    }
    free(cmd.lines);
    return acc;
}

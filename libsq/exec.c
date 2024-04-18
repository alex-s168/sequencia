#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sequencia.h"

#include "exec/operations.h"
#include "../minilibs/filelib.h"
#undef OPERATION

SQValue sqexec_single(SQValue input, const char *command, SQCommand children, SQValue arg) {
    if (gDebug) {
        while (true) {
            printf("> ");
            char *op = readLine(stdin);

            if (op[0] == '\0')
                continue;

            if (strcmp(op, "step") == 0) {
                free(op);
                break;
            }
            else if (strcmp(op, "exit") == 0) {
                exit(0);
            }
            else if (strcmp(op, "val") == 0) {
                sqoutput(input, stdout, true, false, 0);
                fputc('\n', stdout);
            }
            else if (strcmp(op, "op") == 0) {
                puts(command);
                size_t ch = children.lines_len;
                if (ch > 3)
                    ch = 3;
                for (size_t i = 0; i < ch; i ++) {
                    printf("  %s\n", children.lines[i]);
                }
                if (children.lines_len > 3)
                    puts("  ...");
            }
            else if (strcmp(op, "finish") == 0) {
                gDebug = false;
                break;
            }
            else {
                puts("Available commands:");
                puts("  step");
                puts("  exit");
                puts("  val");
                puts("  op");
                puts("  finish");
            }

            free(op);
        }
    }

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

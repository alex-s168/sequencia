#include <stdlib.h>
#include <string.h>

#include "sequencia.h"
#include "../minilibs/utils.h"

SQCommand sqcommand_clone(const SQCommand cmd) {
    SQCommand new;
    new.lines = malloc(sizeof(char*) * cmd.lines_len);
    new.lines_len = cmd.lines_len;
    for (size_t i = 0; i < new.lines_len; i ++)
        new.lines[i] = strdup(cmd.lines[i]);
    return new;
}

SQCommandIterState SQCommandIterState_new(SQCommand cmd) {
    size_t indent = 0;
    if (cmd.lines_len == 0)
        goto ret;

    while (cmd.lines[0][indent] != '\0') {
        if (cmd.lines[0][indent] == ' ')
            indent ++;
        else
            break;
    }

    ret:
        return (SQCommandIterState) { .indent = indent, .lindex = 0 };
}


SQCommandIterItem sqcmd_next(const SQCommand command, SQCommandIterState *state) {
    if (state->lindex >= command.lines_len)
        return (SQCommandIterItem) { .cmd = NULL };

    SQCommandIterItem item;
    item.cmd = command.lines[state->lindex];

    size_t take = 0;
    size_t min_indent = 345345324;
    for (size_t i = state->lindex + 1; i < command.lines_len; i ++) {
        const char *line = command.lines[i];
        if (line[0] == '\0') // empty line
            continue;

        const size_t in = indent(line);
        if (in <= state->indent)
            break;
        if (in < min_indent)
            min_indent = in;

        take ++;
    }

    item.children.lines_len = take;
    item.children.lines = malloc(sizeof(char*) * take);

    for (size_t i = 0; i < take; i ++) {
        char *cmd = command.lines[state->lindex + 1 + i] + min_indent;
        item.children.lines[i] = cmd;
    }

    state->lindex += 1 + take;

    return item;
}

SQCommand sqparseheap(char *strin) {
    SQCommand cmd;
    cmd.lines = NULL;
    cmd.lines_len = 0;

    SPLITERATE(strin, "\n", line) {
        cmd.lines = realloc(cmd.lines, sizeof(char*) * (cmd.lines_len + 1));
        cmd.lines[cmd.lines_len ++] = strdup(line);
    }

    return cmd;
}

void sqdestroyheap(SQCommand cmd) {
    for (size_t i = 0; i < cmd.lines_len; i ++)
        free(cmd.lines[i]);
    free(cmd.lines);
}

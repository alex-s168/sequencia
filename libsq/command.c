#include "sequencia.h"
#include "../minilibs/utils.h"

#define UNI_LEN(cmd)    (cmd.views ? cmd.lines.views.fixed.len : cmd.lines.actual.fixed.len)
#define UNI_GET(cmd, i) (cmd.views ? ((SQStrView*)FixedList_get(cmd.lines.views.fixed, i))->fixed : ((SQStrView*)FixedList_get(cmd.lines.actual.fixed, i))->fixed)
#define UNI_VIEW(fix)   ((SQStrView) {.fixed=fix})

SQCommand sqcommand_clone(const SQCommand cmd) {
    SQCommand new;
    new.views = false;

    DynamicList_init(&new.lines.actual, sizeof(SQStr), gAlloc, UNI_LEN(cmd));
    for (size_t i = 0; i < UNI_LEN(cmd); i ++) {
        SQStrView src = UNI_VIEW(UNI_GET(cmd, i));
        SQStr *dst = FixedList_get(new.lines.actual.fixed, i);
        *dst = zdupv(src);
    }
    return new;
}

SQCommandIterState SQCommandIterState_new(SQCommand cmd) {
    size_t indent = 0;

    if (UNI_LEN(cmd) == 0)
        goto ret;

    indent = zvindent(UNI_VIEW(UNI_GET(cmd, 0)));

    ret:
        return (SQCommandIterState) { .indent = indent, .lindex = 0 };
}


SQCommandIterItem sqcmd_next(const SQCommand command, SQCommandIterState *state) {
    if (state->lindex >= UNI_LEN(command))
        return (SQCommandIterItem) { .cmd.fixed.len = 0 };

    SQCommandIterItem item;
    item.cmd = UNI_VIEW(UNI_GET(command, state->lindex));

    size_t take = 0;
    size_t min_indent = 345345324;
    for (size_t i = state->lindex + 1; i < UNI_LEN(command); i ++) {
        SQStrView line = UNI_VIEW(UNI_GET(command, i));
        if (line.fixed.len == 0)
            continue;

        const size_t in = zindent(line);
        if (in <= state->indent)
            break;
        if (in < min_indent)
            min_indent = in;

        take ++;
    }

    item.children.views = true;
    DynamicList_init(&item.children.lines.views, sizeof(SQStrView), gAlloc, take);
    for (size_t i = 0; i < take; i ++) {
        struct FixedList TYPES(char) cmd = UNI_GET(command, state->lindex + 1 + i);
        cmd.data += sizeof(char) * min_indent;
        cmd.len -= min_indent;
        SQStrView view;
        view.fixed = cmd;
        DynamicList_add(&item.children.lines.views, &view);
    }

    state->lindex += 1 + take;

    return item;
}

SQCommand sqparseheap(char *strin) {
    SQCommand cmd;
    cmd.views = false;
    DynamicList_init(&cmd.lines.actual, sizeof(SQStr), gAlloc, 0);

    SPLITERATE(strin, "\n", line) {
        if (line[0] == '\0')
            continue;
        SQStr zline = zdupc(line);
        DynamicList_add(&cmd.lines.actual, &zline);
    }

    return cmd;
}

void sqdestroyheap(SQCommand cmd) {
    if (cmd.views) {
        DynamicList_clear(&cmd.lines.views);
    } else {
        for (size_t i = 0; i < cmd.lines.actual.fixed.len; i ++) {
            SQStr str = *(SQStr*)FixedList_get(cmd.lines.actual.fixed, i);
            zfree(str);
        }
        DynamicList_clear(&cmd.lines.actual);
    }
}

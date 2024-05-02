#include "sequencia.h"

// DO NOT REMOVE:
#define FILELIB_IMPL
#include "../minilibs/filelib.h"

#include "exec/operations.h"
#undef OPERATION

SQValue sqexec_single(SQValue input, SQStrView view, SQCommand children, SQValue arg) {
    if (gDebug) {
        SQStr copy = zdupv(view);
        zterminate(&copy);
        gDebugInstCallback(input, (char*)copy.fixed.data, children, arg);
        zfree(copy);
    }

#define OPERATION(name) if (zvequal(view, zviewc(#name))) \
    return sqop_##name(input, children, arg);

#include "exec/operations.h"
#undef OPERATION

    ERR("Unknown command!");
    sqfree(input);
    return SQVAL_NULL();
}

SQValue sqexec(SQValue input, SQCommand cmd, SQValue arg_override) {
    SQValue acc = input;
    SQITER(cmd, item, {
        int argstart = FixedList_indexOf(item.cmd.fixed, (char[]) { ' ' });
        SQValue arg = arg_override;
        SQStrView command = item.cmd;
        if (argstart >= 0) {
            size_t end;
            SQStrView args = item.cmd;
            args.fixed.data += sizeof(char) * (argstart + 1);
            args.fixed.len -= argstart + 1;
            arg = sqparse(args, &end);
            command.fixed.len = argstart;
        }
        if (arg.type == SQ_NULL)
            arg = arg_override;
        acc = sqexec_single(acc, command, item.children, arg);
    });
    return acc;
}

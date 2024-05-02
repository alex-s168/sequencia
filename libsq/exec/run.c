#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"
#include "../../minilibs/filelib.h"

OPERATION(run) {
    if (arg.type != SQ_STRING) {
        ERR("\"run\" needs string argument!");
        sqfree(input);
        return SQVAL_NULL();
    }

    const char *prefix = "";
    SQStr to = sqstringify(input);
    zterminate(&to);
    {
        const char *tofile = ".sequencia.temp.a";
        FILE *f = fopen(tofile, "w");
        assert(f != NULL);
        fputs((char*)to.fixed.data, f);
        fclose(f);
        prefix = "cat .sequencia.temp.a | ";
    }
    zfree(to);

    sqfree(input);

    SQStr argCopy = zdup(arg.str);
    zterminate(&argCopy);

    static char cmd[256];
    sprintf(cmd, "%s%s > .sequencia.temp.b", prefix, (char*)argCopy.fixed.data);
    zfree(argCopy);
    (void) system(cmd);

    FILE *from = fopen(".sequencia.temp.b", "r");
    char *all = readFile(from);
    fclose(from);

    SQStr str = zdupc(all);
    free(all);
    return SQVAL_STR(str);
}

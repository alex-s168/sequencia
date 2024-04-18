#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"
#include "../../minilibs/filelib.h"

OPERATION(run) {
    if (arg.type != SQ_STRING) {
        fprintf(stderr, "Run command needs string argument!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    const char *prefix = "";
    const char *to = sqstringify(input);
    if (to != NULL) {
        const char *tofile = ".sequencia.temp.a";
        FILE *f = fopen(tofile, "w");
        assert(f != NULL);
        fputs(to, f);
        fclose(f);
        prefix = "cat .sequencia.temp.a | ";
    }

    sqfree(input);

    static char cmd[256];
    sprintf(cmd, "%s%s > .sequencia.temp.b", prefix, arg.str);
    (void) system(cmd);

    FILE *from = fopen(".sequencia.temp.b", "r");
    char *all = readFile(from);
    fclose(from);

    return SQVAL_STR(all);
}

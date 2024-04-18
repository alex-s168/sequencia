#include <stdio.h>
#include <string.h>

#include "sequencia.h"

#define CLI_IMPL
#include "minilibs/cli.h"
#define FILELIB_IMPL
#include "minilibs/filelib.h"

static const char *flag(const int argc, char **argv, const char *name1, const char *name2, const char *defaul) {
    Flag flag = getFlag(argc, argv, name1);
    if (!flagExist(flag))
        flag = getFlag(argc, argv, name2);
    const char *str = defaul;
    if (flag.str != NULL)
        str = flag.str[1];
    return str;
}

int main(const int argc, char **argv) {
    if (flagExist(getFlag(argc, argv, "--help")) ||
        flagExist(getFlag(argc, argv, "-h"))) {
        printf("\"%s\" - Sequencia utility interpreter\n", argv[0]);
        printf("  -S  --file [path]     REQUIRED: The path to a sequence script source file\n");
        printf("  -s  --script [script] ^ ALTERNATIVE: The script to evaluate\n");
        printf("  -I  --input [path]    The input file for data; default: \"-\"\n");
        printf("  -d  --debug           Enable script developer debug output\n");
        fputc('\n', stdout);
        return 0;
        }

    SQCommand cmd;

    const char *scriptFile = flag(argc, argv, "-S", "--file", NULL);
    if (scriptFile != NULL) {
        FILE *f = fopen(scriptFile, "r");
        if (f == NULL) {
            fprintf(stderr, "Script file not found!\n");
            return 1;
        }
        char *str = readFile(f);
        fclose(f);
        if (str == NULL) {
            fprintf(stderr, "Error reading script file!\n");
            return 1;
        }
        cmd = sqparseheap(str);
        free(str);
    } else {
        const char *scriptCode = flag(argc, argv, "-s", "--script", NULL);
        if (scriptCode == NULL) {
            fprintf(stderr, "Either script code or script file needs to be specified!\n");
            return 1;
        }
        char *str = strdup(scriptCode);
        cmd = sqparseheap(str);
        free(str);
    }

    const bool debug = flagExist(getFlag(argc, argv, "-d")) ||
                       flagExist(getFlag(argc, argv, "--debug"));

    const char *input = flag(argc, argv, "-I", "--input", "-");
    FILE *f;
    if (strcmp(input, "-") == 0) {
        f = stdin;
    } else {
        f = fopen(input, "r");
        if (f == NULL) {
            fprintf(stderr, "Could not open input file!\n");
            return 1;
        }
    }

    char *str = readFile(f);
    if (str == NULL)
        return 1;

    if (f != stdin)
        fclose(f);

    const SQValue res = sqexecs(SQVAL_STR(str), cmd);

    sqoutput(res, stdout, debug, false, 0);
    fputc('\n', stdout);

    return 0;
}

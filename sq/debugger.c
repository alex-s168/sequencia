#include <stdio.h>
#include <string.h>

#include "../libsq/sequencia.h"
#include "../minilibs/filelib.h"

static bool stepping = true;

static void debCallback(SQValue input, const char *command, SQCommand children, SQValue arg) {
    while (stepping) {
        printf("sq (dbg) ");
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

            for (size_t i = 0; i < ch; i ++)
                printf("  %s\n", children.lines[i]);
            
            if (children.lines_len > 3)
                puts("  ...");
        }
        else if (strcmp(op, "run") == 0) {
            stepping = false;
            break;
        }
        else {
            puts("Available commands:");
            puts("  step\tnext instruction");
            puts("  exit\tstop program");
            puts("  val \tshow current value");
            puts("  op  \tshow current operation(s)");
            puts("  run \trun until error");
        }

        free(op);
    }
}

static void errCallback(const char *str) {
    fprintf(stderr, "%s\n", str);
    if (gDebug) {
        stepping = true;
    }
}

void (*gDebugInstCallback)(SQValue input, const char *command, SQCommand children, SQValue arg) = debCallback;
bool gDebug;
void (*gErrCallback)(const char *) = errCallback;


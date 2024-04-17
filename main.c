#include <stdio.h>
#include <string.h>

#include "sequencia.h"

int main(void) {
    static char cmdstr[] =
             "split \" \""
        "\n" "filter"
        "\n" "  split \"\""
        "\n" "  map"
        "\n" "    make 3"
        "\n" "    map 1"
        "\n" "      fdigit"
        "\n" "    map 2"
        "\n" "      falpha"
        "\n" "    axis"
        "\n" "    map"
        "\n" "      same"
        "\n" "    any"
        "\n" "  same"
        "\n" "join \" \""
    ;

    const SQCommand cmd = sqparseheap(cmdstr);
    const SQValue res = sqexec(SQVAL_STR(strdup("h1ey he33 y(o ma11")), cmd);
    sqoutput(res, stdout, true, false, 0);
    sqfree(res);

    fputc('\n', stdout);
    
    return 0;
}

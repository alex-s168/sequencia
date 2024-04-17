#include "sequencia.h"

size_t indent(const char *str) {
    size_t in = 0;
    while (*str != '\0') {
        if (*str != ' ')
            break;
        in ++;
        str ++;
    }
    return in;
}

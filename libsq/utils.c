#include "sequencia.h"

size_t indent(const char *str) {
    size_t in = 0;
    for (; *str != '\0'; str++) {
        if (*str == ' ') {
            in ++;
        } else if (*str == '\t') {
            in += 8;
        } else {
            break;
        }
    }
    return in;
}

size_t indent_ascii(const char *str) {
    size_t in = 0;
    for (; *str != '\0'; str ++) {
        if (*str == ' ' || *str == '\t') {
            in ++;
        } else {
            break;
        }
    }
    return in;
}

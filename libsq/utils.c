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

size_t zvindent(SQStrView str) {
    size_t in = 0;
    for (size_t i = 0; i < str.fixed.len; i ++) {
        char c = *(char*)FixedList_get(str.fixed, i);
        if (c == ' ')
            in ++;
        else if (c == '\t')
            in += 8;
        else
            break;
    }
    return in;
}

void println(const SQStrView str) {
    SQStr copy = zdupv(str);
    zterminate(&copy);
    puts((char*)copy.fixed.data);
    zfree(copy);
}

void print(const SQStrView str) {
    SQStr copy = zdupv(str);
    zterminate(&copy);
    fputs((char*)copy.fixed.data, stdout);
    zfree(copy);
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


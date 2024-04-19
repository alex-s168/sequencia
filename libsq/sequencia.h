#ifndef SEQUENCIA_H
#define SEQUENCIA_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include "../kollektions/lists.h"

typedef enum {
    SQ_NULL = 0,
    SQ_NUMBER,
    SQ_ARRAY,
    SQ_STRING,
} SQType;

typedef char *SQStr;

typedef long long int SQNum;

struct SQValue_s;

typedef struct DynamicList SQArr;

typedef struct SQValue_s {
    SQType type;
    union {
        SQStr str;
        SQNum num;
        SQArr arr;
    };
} SQValue;

typedef struct {
    char   **lines;
    size_t   lines_len;
} SQCommand;

typedef struct {
    size_t lindex;
    size_t indent;
} SQCommandIterState;

size_t indent(const char *str);

SQArr sqarr_new(size_t len);
void sqarr_add(SQArr *arr, SQValue val);
void sqarr_reserve(SQArr *arr, size_t additional);
void sqarr_free_rec(SQArr arr);
void sqarr_free_norec(SQArr arr);
static SQValue *sqarr_at(SQArr arr, size_t i) {
    return (SQValue*)FixedList_get(arr.fixed, i);
}

SQCommand sqcommand_clone(SQCommand cmd);
SQCommandIterState SQCommandIterState_new(SQCommand cmd);
typedef struct {
    char *cmd;
    SQCommand children;
} SQCommandIterItem;
SQCommandIterItem sqcmd_next(SQCommand command, SQCommandIterState *state);
#define SQITER(cmdi,item,block) { \
    SQCommandIterState item##__iter = SQCommandIterState_new(cmdi); \
    SQCommandIterItem item; \
    while ((item = sqcmd_next((cmdi), &item##__iter)).cmd != NULL) block \
}
SQCommand sqparseheap(char *strin);
void sqdestroyheap(SQCommand cmd);

SQValue sqparse(char *str, char **end);
char *sqstringify(SQValue val);
void sqoutput(SQValue val, FILE *out, bool debug, bool ptrs, size_t indent);
void sqfree(SQValue val);
SQValue sqdup(const SQValue val);
bool sqeq(const SQValue a, const SQValue b);

SQValue sqexec(SQValue input, SQCommand cmd, SQValue arg_override);
SQValue sqexec_single(SQValue input, const char *command, SQCommand children, SQValue arg);

extern bool gDebug;

#define SQVAL_NULL()     ((SQValue) { .type = SQ_NULL })
#define SQVAL_NUM(numin) ((SQValue) { .type = SQ_NUMBER, .num = (numin) })
#define SQVAL_ARR(arrin) ((SQValue) { .type = SQ_ARRAY, .arr = (arrin) })
#define SQVAL_STR(strin) ((SQValue) { .type = SQ_STRING, .str = (strin) })

#define sqexecs(input,cmd) sqexec(input, cmd, SQVAL_NULL())

#endif //SEQUENCIA_H

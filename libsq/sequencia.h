#ifndef SEQUENCIA_H
#define SEQUENCIA_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include "../kollektions/lists/lists.h"

typedef enum {
    SQ_NULL = 0,
    SQ_NUMBER,
    SQ_ARRAY,
    SQ_STRING,
} SQType;

typedef struct DynamicList TYPES(char) SQStr;
typedef struct {
    struct FixedList fixed;
} SQStrView;

typedef long long int SQNum;

struct SQValue_s;

typedef struct DynamicList TYPES(struct SQValue_s) SQArr;

typedef struct SQValue_s {
    SQType type;
    union {
        SQStr str;
        SQNum num;
        SQArr arr;
    };
} SQValue;

typedef struct {
    bool views;
    union {
        struct DynamicList TYPES(SqStr) actual;
        struct DynamicList TYPES(SqStrView) views;
    } lines;
} SQCommand;

typedef struct {
    size_t lindex;
    size_t indent;
} SQCommandIterState;

size_t indent(const char *str);
size_t zvindent(SQStrView view);
#define zindent(str) zvindent(zview(str))

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
    SQStrView cmd;
    SQCommand children;
} SQCommandIterItem;
SQCommandIterItem sqcmd_next(SQCommand command, SQCommandIterState *state);
#define SQITER(cmdi,item,block) { \
    SQCommandIterState item##__iter = SQCommandIterState_new(cmdi); \
    SQCommandIterItem item; \
    while ((item = sqcmd_next((cmdi), &item##__iter)).cmd.fixed.len != 0) block \
}
SQCommand sqparseheap(char *strin);
void sqdestroyheap(SQCommand cmd);

SQValue sqparse(SQStrView view, size_t *used);
SQStr sqstringify(SQValue val);

void sqoutput(SQValue *val, FILE *out, bool debug, bool ptrs, size_t indent);
void sqfree(SQValue val);
SQValue sqdup(const SQValue val);
bool sqeq(const SQValue a, const SQValue b);

SQValue sqexec(SQValue input, SQCommand cmd, SQValue arg_override);
SQValue sqexec_single(SQValue input, SQStrView cmd, SQCommand children, SQValue arg);

extern Ally gAlloc;
extern bool gDebug;
extern void (*gErrCallback)(const char *msg);
extern void (*gDebugInstCallback)(SQValue input, const char *command, SQCommand children, SQValue arg);

static SQStr zdup(SQStr in) {
    SQStr res;
    DynamicList_copy(in, &res, gAlloc, 1);
    return res;
}
static SQStr zdupc(const char *in) {
    size_t len = strlen(in);
    SQStr res;
    DynamicList_init(&res, sizeof(char), gAlloc, len);
    DynamicList_addAll(&res, (void*) in, len);
    return res;
}
static void zterminate(SQStr *str) {
    DynamicList_add(str, (char[]) { '\0' });
}
static SQStr zempty() {
    SQStr res;
    DynamicList_init(&res, sizeof(char), gAlloc, 0);
    return res;
}
static bool zequal(SQStr a, SQStr b) {
    if (a.fixed.len != b.fixed.len)
        return false;
    return memcmp(a.fixed.data, b.fixed.data, a.fixed.len) == 0;
}
static bool zvequal(SQStrView a, SQStrView b) {
    if (a.fixed.len != b.fixed.len)
        return false;
    return memcmp(a.fixed.data, b.fixed.data, a.fixed.len) == 0;
}
static SQStr zdupv(SQStrView view) {
    SQStr res;
    FixedList_copy(view.fixed, &res, gAlloc, 1);
    return res;
}
static SQStrView zviewc(const char *str) {
    SQStrView view;
    view.fixed.stride = sizeof(char);
    view.fixed.len = strlen(str);
    view.fixed.data = (void*) str;
    return view;
}
#define zview(str) ((SQStrView) {.fixed=str.fixed})
#define zfree(str) DynamicList_clear(&str)

#define ERR(msg) gErrCallback(msg)

#define SQVAL_NULL()     ((SQValue) { .type = SQ_NULL })
#define SQVAL_NUM(numin) ((SQValue) { .type = SQ_NUMBER, .num = (numin) })
#define SQVAL_ARR(arrin) ((SQValue) { .type = SQ_ARRAY, .arr = (arrin) })
#define SQVAL_STR(strin) ((SQValue) { .type = SQ_STRING, .str = (strin) })

#define sqexecs(input,cmd) sqexec(input, cmd, SQVAL_NULL())

#endif //SEQUENCIA_H

#include <stdbool.h>

#include "../libsq/sequencia.h"
#include "../kollektions/dynamic_list/dynamic_list.h"

typedef struct {
    struct FixedList TYPES(const char *) lines;
} SQAScript;

typedef struct {
    size_t line;
    size_t col;
    size_t len;
    char* msg;
} SQAError;

typedef struct DynamicList TYPES(SQAError) SQAErrors;

void sqaerrors_clean(SQAErrors *errors);

SQValue sqa_analyze_value(const char *str, size_t info_line, size_t info_col, SQAErrors *dest);
void sqa_analyze(SQAScript script, SQAErrors *dest);

typedef struct {
    const char *name;
    const char *docPage;
} SQAOp;

// null terminated
extern SQAOp sqa_ops[];

SQAOp *sqa_ops_all();

SQAOp *sqa_op_lookup(const char *name);
bool sqa_op_accepts(SQAOp op, SQValue val);

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minilibs/utils.h"

typedef enum {
    SQ_NULL = 0,
    SQ_NUMBER,
    SQ_ARRAY,
    SQ_STRING,
} SQType;

typedef char *SQStr;

typedef long long int SQNum;

struct SQValue_s;

typedef struct {
    struct SQValue_s *items;
    size_t len;
} SQArr;

typedef struct SQValue_s {
    SQType type;
    union {
        SQStr str;
        SQNum num;
        SQArr arr;
    };
} SQValue;

SQArr sqarr_new(size_t len) {
    SQArr res;
    res.items = malloc(sizeof(SQValue) * len);
    memset(res.items, 0, sizeof(SQValue) * len);
    res.len = len;
    return res;
}

void sqarr_add(SQArr *arr, SQValue val) {
    arr->items = realloc(arr->items, sizeof(SQValue) * (arr->len + 1));
    arr->items[arr->len ++] = val;
}

void sqarr_free_rec(SQArr arr) {
    for (size_t i = 0; i < arr.len; i ++)
        if (arr.items[i].type == SQ_ARRAY)
            sqarr_free_rec(arr.items[i].arr);
    
    free(arr.items);
}

#define SQVAL_NULL()     ((SQValue) { .type = SQ_NULL })
#define SQVAL_NUM(numin) ((SQValue) { .type = SQ_NUMBER, .num = (numin) })
#define SQVAL_ARR(arrin) ((SQValue) { .type = SQ_ARRAY, .arr = (arrin) })
#define SQVAL_STR(strin) ((SQValue) { .type = SQ_STRING, .str = (strin) })

typedef struct {
    char   **lines;
    size_t   lines_len;
} SQCommand;

SQCommand sqcommand_clone(const SQCommand cmd) {
    SQCommand new;
    new.lines = malloc(sizeof(char*) * cmd.lines_len);
    new.lines_len = cmd.lines_len;
    for (size_t i = 0; i < new.lines_len; i ++)
        new.lines[i] = strdup(cmd.lines[i]);
    return new;
}

typedef struct {
    size_t lindex;
    size_t indent;
} SQCommandIterState;

SQCommandIterState SQCommandIterState_new(SQCommand cmd) {
    size_t indent = 0;
    if (cmd.lines_len == 0)
        goto ret;
    
    while (cmd.lines[0][indent] != '\0') {
        if (cmd.lines[0][indent] == ' ')
            indent ++;
        else
            break;
    }

ret:
    return (SQCommandIterState) { .indent = indent, .lindex = 0 };
}

typedef struct {
    char *cmd;
    SQCommand children;
} SQCommandIterItem;

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

SQCommandIterItem sqcmd_next(const SQCommand command, SQCommandIterState *state) {
    if (state->lindex >= command.lines_len)
        return (SQCommandIterItem) { .cmd = NULL };
    
    SQCommandIterItem item;
    item.cmd = command.lines[state->lindex];

    size_t take = 0;
    size_t min_indent = 345345324;
    for (size_t i = state->lindex + 1; i < command.lines_len; i ++) {
        const char *line = command.lines[i];
        if (line[0] == '\0') // empty line
            continue;

        const size_t in = indent(line);
        if (in <= state->indent)
            break;
        if (in < min_indent)
            min_indent = in;

        take ++;
    }

    item.children.lines_len = take;
    item.children.lines = malloc(sizeof(char*) * take);
    
    for (size_t i = 0; i < take; i ++) {
        char *cmd = command.lines[state->lindex + 1 + i] + min_indent;
        item.children.lines[i] = cmd;
    }

    state->lindex += 1 + take;

    return item;
}

SQCommand sqparseheap(const char *strin) {
    char *str = strdup(strin);
    
    SQCommand cmd;
    cmd.lines = NULL;
    cmd.lines_len = 0;

    SPLITERATE(str, "\n", line) {
        cmd.lines = realloc(cmd.lines, sizeof(char*) * (cmd.lines_len + 1));
        cmd.lines[cmd.lines_len ++] = strdup(line);
    }

    free(str);
    return cmd;
}

void sqdestroyheap(SQCommand cmd) {
    for (size_t i = 0; i < cmd.lines_len; i ++)
        free(cmd.lines[i]);
    free(cmd.lines);
}

#define SQITER(cmdi,item,block) { \
    SQCommandIterState item##__iter = SQCommandIterState_new(cmdi); \
    SQCommandIterItem item; \
    while ((item = sqcmd_next((cmdi), &item##__iter)).cmd != NULL) block \
    }

SQValue sqparse(char *str, char **end) {
    while (*str == ' ')
        str ++;
    
    if (*str == '[') {
        str ++;
        SQArr arr = sqarr_new(0);
        while (true) {
            char *temp;
            const SQValue val = sqparse(str, &temp);
            sqarr_add(&arr, val);
            str = temp;
            if (*str != ',')
                break;
            str ++;
        }
        if (*str == ']')
            str ++;
        *end = str;
        return SQVAL_ARR(arr);
    }

    if (*str == '"') {
        str ++;
        const char *start = str;
        while (*str != '\0') {
            if (*str == '"') {
                *str = '\0';
                str ++;
                break;
            }
            str ++;
        }
        const SQStr res = strdup(start);
        *end = str;
        return SQVAL_STR(res);
    }
     
    const SQNum num = strtol(str, end, 10);
    if (*end == str) {
        if (str[0] == 'n' && str[1] == 'l') {
            *end = str + 2;
            return SQVAL_STR(strdup("\n"));
        }
        if (str[0] == 't' && str[1] == 'a' && str[2] == 'b') {
            *end = str + 3;
            return SQVAL_STR(strdup("\t"));
        }
        if (str[0] == 'n' && str[1] == 'u' && str[2] == 'l' && str[3] == 'l') {
            *end = str + 4;
            return SQVAL_NULL();
        }
        return SQVAL_NULL();
    }

    return SQVAL_NUM(num);
}

char *sqstringify(SQValue val) {
    switch (val.type) {
        case SQ_NULL: {
            return NULL;
        }

        case SQ_NUMBER: {
            static char buf[256];
            sprintf(buf, "%lld", val.num);
            return strdup(buf);
        }

        case SQ_STRING: {
            return strdup(val.str);
        }

        case SQ_ARRAY: {
            fprintf(stderr, "Can't automatically join array!\n");
            return NULL;
        }
    }
    assert(false);
}

void sqoutput(SQValue val, FILE *out, bool debug, bool ptrs, size_t indent) {
    switch (val.type) {
        case SQ_NULL:
            if (debug) {
                fprintf(out, "(null)");
            }
            break;

        case SQ_NUMBER: {
            fprintf(out, "%lld", val.num);
        }
        break;

        case SQ_STRING: {
            if (debug) {
                if (ptrs)
                    fprintf(out, "0x%p=\"%s\"", val.str, val.str);
                else
                    fprintf(out, "\"%s\"", val.str);
            }
            else {
                fprintf(out, "%s", val.str);
            }
        }
        break;

        case SQ_ARRAY: {
            if (debug) {
                if (ptrs)
                    fprintf(out, "0x%p=[\n", val.arr.items);
                else
                    fprintf(out, "[\n");
                for (size_t i = 0; i < val.arr.len; i ++) {
                    if (i > 0)
                        fprintf(out, ",\n");
                    for (size_t j = 0; j < indent + 1; j ++)
                        fputs("  ", out);
                    sqoutput(val.arr.items[i], out, true, ptrs, indent + 1);
                }
                fputc('\n', out);
                for (size_t j = 0; j < indent; j ++)
                    fputs("  ", out);
                fputc(']', out);
            }
            else {
                fprintf(stderr, "Cannot print array! Try to run with `--debug`\n");
            }
        }
        break;
    }
}

void sqfree(SQValue val) {
    if (val.type == SQ_ARRAY)
        sqarr_free_rec(val.arr);
    else if (val.type == SQ_STRING)
        free(val.str);
}

SQValue sqdup(const SQValue val) {
    switch (val.type) {
        case SQ_NULL:
        case SQ_NUMBER:
            return val;

        case SQ_STRING:
            return SQVAL_STR(strdup(val.str));

        case SQ_ARRAY: {
            const SQArr res = sqarr_new(val.arr.len);
            for (size_t i = 0; i < val.arr.len; i ++)
                res.items[i] = sqdup(val.arr.items[i]);
            return SQVAL_ARR(res);
        }
        
        default:
            assert(false);
    }
}

bool sqeq(const SQValue a, const SQValue b) {
    if (a.type != b.type)
        return false;

    switch (a.type) {
        case SQ_NULL:
            return true;
        
        case SQ_NUMBER:
            return a.num == b.num;

        case SQ_STRING:
            return strcmp(a.str, b.str) == 0;

        case SQ_ARRAY: {
            if (a.arr.len != b.arr.len)
                return false;
            bool eq = true;
            for (size_t i = 0; i < a.arr.len; i ++) {
                if (!sqeq(a.arr.items[i], b.arr.items[i])) {
                    eq = false;
                    break;
                }
            }
            return eq;
        }
        
        default:
            assert(false);
    }
}

SQValue sqexec(SQValue input, SQCommand cmd);

static SQValue filterstr_cmd(SQValue input, int (*passes)(int c), bool invert) {
    if (input.type != SQ_STRING) {
        fprintf(stderr, "Expected string!\n");
        sqfree(input);
        return SQVAL_NULL();
    }

    const size_t len = strlen(input.str);

    size_t filtered_len = 0;
    for (size_t i = 0; i < len; i ++) {
        filtered_len += passes(input.str[i]);
    }

    char *new = malloc(len + 1);
    char *ptr = new;
    for (size_t i = 0; i < len; i ++) {
        if (passes(input.str[i]))
            *ptr++ = input.str[i];
    }
    *ptr = '\0';
    sqfree(input);
    return SQVAL_STR(new);
}

SQValue sqexec_single(SQValue input, const char *command, SQCommand children, SQValue arg) {
    if (strcmp(command, "split") == 0) {
        if (arg.type != SQ_STRING) {
            fprintf(stderr, "Can only split with strings!\n");
            goto err;
        }

        if (input.type != SQ_STRING) {
            fprintf(stderr, "Can only split strings!\n");
            goto err;
        }
        
        SQArr arr = sqarr_new(0);

        if (arg.str[0] == '\0') {
            char *str = input.str;
            while (*str != '\0') {
                static char buf[2];
                buf[0] = *str;
                buf[1] = '\0';
                sqarr_add(&arr, SQVAL_STR(strdup(buf)));
                str ++;
            }
        }
        else {
            SPLITERATE(input.str, arg.str, elem) {
                sqarr_add(&arr, SQVAL_STR(strdup(elem)));
            }
        }
        
        sqfree(input);
        return SQVAL_ARR(arr);
    }

    if (strcmp(command, "join") == 0) {
        if (arg.type != SQ_STRING) {
            fprintf(stderr, "Can only join with strings!\n");
            goto err;
        }

        size_t delim_len = strlen(arg.str);

        if (input.type != SQ_ARRAY) {
            fprintf(stderr, "Can only join array!\n");
            goto err;
        }

        char *res = malloc(1);
        size_t res_len = 0;

        for (size_t i = 0; i < input.arr.len; i ++) {
            char *b = sqstringify(input.arr.items[i]);
            if (b == NULL)
                continue;

            if (i > 0) {
                res = realloc(res, res_len + delim_len + 1);
                memcpy(res + res_len, arg.str, delim_len);
                res_len += delim_len;
            }
            
            const size_t len = strlen(b);
            res = realloc(res, res_len + len + 1);
            memcpy(res + res_len, b, len);
            res_len += len;
        }

        res[res_len] = '\0';

        sqfree(input);
        return SQVAL_STR(res);
    }

    if (strcmp(command, "select") == 0) {
        if (input.type != SQ_ARRAY) {
            fprintf(stderr, "Can only perform select on array!\n");
            goto err;
        }

        if (arg.type == SQ_ARRAY) {
            SQArr res = sqarr_new(0);

            for (size_t i = 0; i < arg.arr.len; i ++) {
                const SQValue index = arg.arr.items[i];
                if (index.type != SQ_NUMBER) {
                    fprintf(stderr, "Can only perform select with array of indecies!\n");
                    continue;
                }
                SQNum indexid = index.num;
                if (indexid < 0)
                    indexid = input.arr.len + indexid;
                if (indexid >= input.arr.len)
                    continue;
                sqarr_add(&res, sqdup(input.arr.items[indexid]));
            }

            sqfree(input);
            return SQVAL_ARR(res);
        }
        
        if (arg.type == SQ_NUMBER) {
            SQNum indexid = arg.num;
            if (indexid < 0)
                indexid = input.arr.len + indexid;
            SQValue val;
            if (indexid >= input.arr.len)
                val = SQVAL_NULL();
            else
                val = sqdup(input.arr.items[indexid]);

            sqfree(input);
            return val;
        }
        
        fprintf(stderr, "Can only perform select with array of indecies or number (index)!\n");
        goto err;
    }

    if (strcmp(command, "tokens") == 0) {
        if (input.type != SQ_STRING) {
            fprintf(stderr, "Can only tokenize string!\n");
            goto err;
        }

        SQArr res = sqarr_new(0);
        
        char *acc = malloc(1);
        acc[0] = '\0';
        size_t acc_len = 0;

        const char *src = input.str;
        while (*src != '\0') {
            bool add_later = false;
            char c = *src++;
            
            if (isspace(c))
                goto push;

            if (acc[0] == '\0')
                goto add;
            
            if (isalpha(acc[0])) {
                if (isalpha(c))
                    goto add;
                add_later = true;
                goto push;
            }
            
            if (isdigit(acc[0])) {
                if (isdigit(c))
                    goto add;
                add_later = true;
                goto push;
            }

            add_later = true;
            goto push;
            
            continue;
        add:
            acc = realloc(acc, acc_len + 2);
            acc[acc_len ++] = c;
            continue;
        push:
            acc[acc_len] = '\0';
            sqarr_add(&res, SQVAL_STR(acc));
            if (add_later) {
                acc = malloc(2);
                acc[0] = c;
                acc[1] = '\0';
                acc_len = 1;
            }
            else {
                acc = malloc(1);
                acc[0] = '\0';
                acc_len = 0;
            }
        }
        
        sqarr_add(&res, SQVAL_STR(acc));

        sqfree(input);
        return SQVAL_ARR(res);
    }

    if (strcmp(command, "parse") == 0) {
        if (input.type != SQ_STRING) {
            fprintf(stderr, "Can only parse string!\n");
            goto err;
        }

        int base = 10;
        if (arg.type == SQ_NUMBER) {
            base = arg.num;
        }

        char *end;
        const SQNum num = strtol(input.str, &end, base);
        if (*end)
            goto err; // null

        sqfree(input);
        return SQVAL_NUM(num);
    }

    if (strcmp(command, "str") == 0) {
        char *res = sqstringify(input);

        sqfree(input);
        return SQVAL_STR(res);
    }

    if (strcmp(command, "map") == 0) {
        if (input.type != SQ_ARRAY) {
            fprintf(stderr, "Can only map array!\n");
            goto err;
        }

        if (arg.type == SQ_NULL) {
            for (size_t i = 0; i < input.arr.len; i ++) {
                input.arr.items[i] = sqexec(input.arr.items[i], sqcommand_clone(children));
            }
        
            return input;
        }

        if (arg.type == SQ_ARRAY) {
            for (size_t i = 0; i < arg.arr.len; i ++) {
                if (arg.arr.items[i].type != SQ_NUMBER)
                    continue;
                
                SQNum index = arg.arr.items[i].num;
                if (index < 0)
                    index = input.arr.len + index;
                if (index < input.arr.len)
                    input.arr.items[index] = sqexec(input.arr.items[index], sqcommand_clone(children));
            }
            return input;
        }

        if (arg.type == SQ_NUMBER) {
            SQNum index = arg.num;
            if (index < 0)
                index = input.arr.len + index;
            if (index < input.arr.len)
                input.arr.items[index] = sqexec(input.arr.items[index], sqcommand_clone(children));
            return input;
        }

        fprintf(stderr, "Can only map index with index array or index num!\n");
        goto err;
    }

    if (strcmp(command, "transform") == 0) {
        const SQArr res = sqarr_new(2);
        res.items[0] = sqdup(input);
        res.items[1] = sqexec(sqdup(input), sqcommand_clone(children));
        sqfree(input);
        return SQVAL_ARR(res);
    }

    if (strcmp(command, "len") == 0) {
        if (input.type == SQ_ARRAY) {
            size_t len = input.arr.len;
            sqfree(input);
            return SQVAL_NUM(len);
        }
        if (input.type == SQ_STRING) {
            size_t len = strlen(input.str);
            sqfree(input);
            return SQVAL_NUM(len);
        }
        fprintf(stderr, "Can only get length of strings and arrays!\n");
        goto err;
    }

    if (strcmp(command, "invert") == 0) {
        if (input.type != SQ_NUMBER) {
            fprintf(stderr, "Can only invert numbers (bools)!\n");
            goto err;
        }
        return SQVAL_NUM(!input.num);
    }
    
    if (strcmp(command, "same") == 0) {
        if (input.type != SQ_ARRAY) {
            fprintf(stderr, "Same only operates on arrays!\n");
            goto err;
        }

        if (input.arr.len == 0) {
            sqfree(input);
            return SQVAL_NUM(1);
        }

        SQValue v0 = input.arr.items[0];
        bool eq = true;
        for (size_t i = 1; i < input.arr.len; i ++) {
            if (!sqeq(v0, input.arr.items[i])) {
                eq = false;
                break;
            }
        }
        sqfree(input);
        return SQVAL_NUM(eq ? 1 : 0);
    }

    if (strcmp(command, "any") == 0) {
        if (input.type != SQ_ARRAY) {
            fprintf(stderr, "Any only operates on arrays!\n");
            goto err;
        }

        if (input.arr.len == 0) {
            sqfree(input);
            return SQVAL_NUM(0);
        }

        bool any = false;
        for (size_t i = 0; i < input.arr.len; i ++) {
            if (input.arr.items[i].type == SQ_NUMBER && input.arr.items[i].num != 0) {
                any = true;
                break;
            }
        }
        sqfree(input);
        return SQVAL_NUM(any ? 1 : 0);
    }

    if (strcmp(command, "make") == 0) {
        if (arg.type != SQ_NUMBER) {
            fprintf(stderr, "Argument to make needs to be length of array!\n");
            goto err;
        }

        if (arg.num <= 0) {
            fprintf(stderr, "Invalid array length for make!\n");
            goto err;
        }
        
        size_t len = arg.num;
        
        SQArr arr = sqarr_new(len);
        arr.items[0] = input;
        for (size_t i = 1; i < len; i ++)
            arr.items[i] = sqdup(input);

        return SQVAL_ARR(arr);
    }

    if (strcmp(command, "axis") == 0) {
        if (input.type != SQ_ARRAY) {
            fprintf(stderr, "Can only apply axis on array!\n");
            goto err;
        }

        if (input.arr.len == 0) {
            fprintf(stderr, "Input to axis needs to have at least one element!\n");
            goto err;
        }

        SQValue axis = input.arr.items[0];

        SQArr res = sqarr_new(input.arr.len - 1);
        for (size_t i = 1; i < input.arr.len; i ++) {
            SQArr el = sqarr_new(2);
            el.items[0] = sqdup(axis);
            el.items[1] = input.arr.items[i];
            res.items[i - 1] = SQVAL_ARR(el);
        }

        sqfree(axis);
        free(input.arr.items);
        return SQVAL_ARR(res);
    }

    if (strcmp(command, "filter") == 0) {
        if (input.type != SQ_ARRAY) {
            fprintf(stderr, "Can only filter an array!\n");
            goto err;
        }

        SQArr res = sqarr_new(0);

        for (size_t i = 0; i < input.arr.len; i ++) {
            SQValue r = sqexec(sqdup(input.arr.items[i]), sqcommand_clone(children));
            if (r.type == SQ_NULL)
                continue;
            if (r.type != SQ_NUMBER) {
                fprintf(stderr, "Filter needs to return 0 for false and anything else for true!\n");
                sqarr_free_rec(res);
                goto err;
            }
            if (r.num != 0) {
                sqarr_add(&res, sqdup(input.arr.items[i]));
            }
        }
        
        sqfree(input);
        return SQVAL_ARR(res);
    }
    
    if (strcmp(command, "falpha") == 0)
        return filterstr_cmd(input, isalpha, false);
    
    if (strcmp(command, "fdigit") == 0)
        return filterstr_cmd(input, isdigit, false);
    
    if (strcmp(command, "falnum") == 0)
        return filterstr_cmd(input, isalnum, false);
    
    fprintf(stderr, "Unknown command \"%s\"!\n", command);
err:
    sqfree(input);
    return SQVAL_NULL();
}

SQValue sqexec(SQValue input, SQCommand cmd) {
    SQValue acc = input;
    SQITER(cmd, item, {
        if (item.cmd[0] == '\0')
            continue;
        
        char *argstart = strchr(item.cmd, ' ');
        char *args = "";
        if (argstart != NULL) {
            *argstart = '\0';
            args = argstart + 1;
        }
        const char *command = item.cmd;
        char *end;
        const SQValue arg = sqparse(args, &end);
        acc = sqexec_single(acc, command, item.children, arg);
    });
    for (size_t i = 0; i < cmd.lines_len; i ++) {
        free(cmd.lines[i]);
    }
    free(cmd.lines);
    return acc;
}

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
    const SQValue res = sqexec(SQVAL_STR(strdup("h1ey he33 y(o ma1")), cmd);
    sqoutput(res, stdout, true, false, 0);
    sqfree(res);
    
    return 0;
}

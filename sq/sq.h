#include <stddef.h>

typedef struct {
    const char *name;
    char       *text;
} DocEntry;

typedef struct {
    DocEntry *items;
    size_t len; 
} DocEntries;

DocEntries getDocEntries();
DocEntry *findDocEntry(const char *name);

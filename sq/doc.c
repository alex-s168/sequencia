#include <stdlib.h>
#include <string.h>

#include "sq.h"

extern void* doc_lut[][3];

static DocEntries entries = (DocEntries) { .items = NULL };

#include <stdio.h>

DocEntries getDocEntries() {
    if (entries.items != NULL)
        return entries;

    size_t count = 0;
    for (; doc_lut[count][0] != NULL; count ++) {}
   
    entries.len = count;
    entries.items = malloc(sizeof(DocEntry) * count);

    for (size_t i = 0; i < count; i ++) {
        entries.items[i].name = (const char *) (doc_lut[i][0]);
        size_t txt_len = *(unsigned int *) doc_lut[i][2]; 
        char *new = malloc(txt_len + 1);
        memcpy(new, doc_lut[i][1], txt_len);
        entries.items[i].text = new;
    }

    return entries;
}

DocEntry *findDocEntry(const char *name) {
    (void) getDocEntries();

    for (size_t i = 0; i < entries.len; i ++)
        if (strcmp(name, entries.items[i].name) == 0)
            return &entries.items[i];

    return NULL;
}

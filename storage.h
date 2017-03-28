#ifndef STORAGE
#define STORAGE

#include <stdlib.h>
#include "list.h"
#include "process.h"
#include "memory.h"

typedef struct storage_t Storage;
struct storage_t {
    // list of programs (process structure)
    List processes;
};

extern Storage *new_storage();

extern void load_to_storage(List *plist, Storage *s, int progress);
extern void storage_to_memory(Storage *s, Memory *m);
void memory_to_storage(Storage *s, Memory *m);

extern void print_storage(Storage *s, FILE *f);
extern void free_storage(Storage *s);

/*
extern int load_storage(Storage *s, Process *p);
extern void unload_storage(Storage *s, int pid);
extern int in_storage(Storage *s, int pid);
*/

#endif

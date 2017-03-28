#ifndef STORAGE
#define STORAGE

#include <stdlib.h>
#include "list.h"
#include "process.h"

typedef struct storage_t Storage;
struct storage_t {
    // list of programs (process structure)
    List processes;
};

extern Storage *new_storage();
extern Storage *storage_oldest_process(Storage *s);

extern void print_storage(Storage *s, FILE *f);

extern int load_storage(Storage *s, Process *p);
extern void unload_storage(Storage *s, int pid);
extern void free_storage(Storage *s);
extern int in_storage(Storage *s, int pid);

#endif

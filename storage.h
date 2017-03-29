/*=============================================================================
#     FileName: storage.h
#         Desc: declaration of storage functions
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 10:33:54
#      History:
=============================================================================*/
#ifndef STORAGE
#define STORAGE

#include <stdlib.h>
#include "list.h"
#include "advlist.h"
#include "process.h"
#include "memory.h"

typedef struct block_t Block;
struct block_t {
    Process *process;
    int arrival;
};

typedef struct storage_t Storage;
struct storage_t {
    // list of programs (process structure)
    List blocks;
};

extern Storage *new_storage();

extern void load_to_storage(List *plist, Storage *s, int progress);
extern void storage_to_memory(Storage *s, Memory *m, int arrival);
void memory_to_storage(Storage *s, Memory *m, int arrival);

extern void print_storage(Storage *s, FILE *f);
extern void free_storage(Storage *s);
extern void free_block(void *b);

#endif

#ifndef MEMORY
#define MEMORY

#include <stdlib.h>
#include "list.h"
#include "process.h"

typedef struct chunk_t Chunk;
struct chunk_t {
    int size, 
        taken;
};

typedef struct memory_t Memory;
struct memory_t {
    // list of memory chunks
    List chunks;

    // list of processes in queue
    List processes;

    // list of processes ids in arrivals order
    List arrivals;

    int size;
};

extern Chunk *new_chunk(int size);
extern Memory *new_memory(char* strategy, int size);
extern Process *oldest_process(Memory *m);

extern void print_memory(Memory *m, FILE *f);
extern int load_memory(Memory *m, Process *p);
extern void unload_memory(Memory *m, int pid);
extern void free_memory(Memory *m);
extern int in_memory(Memory *m, int pid);

#endif

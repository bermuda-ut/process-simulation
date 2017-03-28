#ifndef MEMORY
#define MEMORY

#define PROCESSED -1;
#define NOTHING_TO_PROCESS -2;

#define FREE_CHAR '-'
#define TAKEN_CHAR '+'
#define MEM_PRINT_LEN 40

#include <math.h>
#include <stdlib.h>
#include "list.h"
#include "process.h"
#include "advlist.h"

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

    // list of processes in arrivals order
    List arrivals;

    int size;
};

extern Chunk *new_chunk(int size);
extern Memory *new_memory(char* strategy, int size);
extern Process *oldest_process(Memory *m);

extern int process_memory_head(Memory *m, int quantum);
extern void requeue_memory_head(Memory *m);
extern void free_memory_head(Memory *m);
extern int load_to_memory(Memory *m, Process *p);
void merge_empty_slots(Memory *m);

//extern int load_memory(Memory *m, Process *p);
//extern void unload_memory(Memory *m, int pid);
//extern int in_memory(Memory *m, int pid);

extern void print_memory(Memory *m, FILE *f);
extern void free_memory(Memory *m);
extern void free_chunk(Chunk *c);

#endif

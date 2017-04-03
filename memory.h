/*=============================================================================
#     FileName: memory.h
#         Desc: declaration of required functions for memory simulation
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-04-03 13:31:21
=============================================================================*/
#ifndef MEMORY
#define MEMORY

#include <math.h>
#include <stdlib.h>
#include "list.h"
#include "process.h"
#include "advlist.h"

#define PROCESSED -1
#define NOTHING_TO_PROCESS -2

#define MEM_PRINT_LEN 40
#define FREE_CHAR '-'
#define TAKEN_CHAR '+'

typedef struct chunk_t Chunk;
struct chunk_t {
    int size, 
        taken;
};

typedef struct memory_t Memory;
struct memory_t {
    List chunks;    // list of memory chunks. Head = highest memory address
    List processes; // list of processes in queue
    List arrivals;  // list of processes in arrivals order
    int size;       // size of the chunk of memory
    int (*fit_strategy)(Memory*, Process*); // strategy to fit memory chunks in
};

/**
 * Check C file for function details
 */
extern Chunk *new_chunk(int size);
extern Memory *new_memory(int (*fit_strategy)(Memory*, Process*), int size);
extern Process *oldest_process(Memory *m);

extern int process_memory_head(Memory *m, int quantum);
extern void requeue_memory_head(Memory *m, Process* p);
extern void free_memory_head(Memory *m);
extern int load_to_memory(Memory *m, Process *p);
void merge_empty_slots(Memory *m);

extern int process_count(Memory *m);
extern int hole_count(Memory *m);
extern int usage_calc(Memory *m);

extern void print_memory(Memory *m, FILE *f);
extern void free_memory(Memory *m);
extern void free_chunk(void *c);

#endif


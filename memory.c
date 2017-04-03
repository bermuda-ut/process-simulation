/*=============================================================================
#     FileName: memory.c
#         Desc: functions required for memory operations
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-04-03 13:43:37
=============================================================================*/
#include "memory.h"

/**
 * Free memory, its chunks and processes and arrivals.
 */
void free_memory(Memory *m) {
    if(m->processes) {
        free_list(m->processes);
        free_list_data(free_process, &(m->processes));
    }

    if(m->chunks) {
        free_list(m->chunks);
        free_list_data(free_chunk, &(m->processes));
    }

    // processes are already free here
    if(m->arrivals)
        free_list(m->arrivals);

    free(m);
}

/**
 * Simple free chunk
 */
void free_chunk(void *c) {
    free(c);
}

/**
 * malloc and return a new chunk of memory
 */
Chunk *new_chunk(int size) {
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->size = size;
    chunk->taken = 0;

    return chunk;
}

/**
 * malloc and set new memory for usage
 */
Memory *new_memory(int (*fit_strategy)(Memory*, Process*), int size) {
    Memory *mem = malloc(sizeof(Memory));
    Chunk *init = new_chunk(size);

    mem->chunks = push(NULL, init);
    mem->arrivals = NULL;
    mem->processes = NULL;
    mem->fit_strategy = fit_strategy;
    mem->size = size;
    return mem;
}

/**
 * return oldest process in the memory
 */
Process *oldest_process(Memory *m) {
    return (m->arrivals->data);
}

/**
 * go through the memory chunks and merge any two empty slots that
 * appear consecutively
 */
void merge_empty_slots(Memory *m) {
    List curr = m->chunks;
    List next = curr->next;

    while(curr && next) {
        Chunk *c = (Chunk*)curr->data;
        Chunk *n = (Chunk*)next->data;
        //fprintf(stderr, "curr %p, next %p\n", curr, next);
        //fprintf(stderr, "c %p, n %p\n", c, n);
        //fprintf(stderr, "ct %d, nt %d\n", c->taken, n->taken);

        if(c && n && c->taken == 0 && n->taken == 0) {
            int total = c->size + n->size;
            Chunk *toadd = new_chunk(total);

            // remove curr's chunk then point into new one
            free_chunk(c);
            curr->data = toadd;

            // completely remove next
            free_chunk(n);
            curr->next = next->next;
            free(next);

            next = curr->next;
        } else {
            if(next && next->next) {
                curr = next;
                next = next->next;
            } else {
                break;
            }
        }
    }
}

/**
 * load a process to memory
 */
int load_to_memory(Memory *m, Process *p) {
    return m->fit_strategy(m, p);
}

/**
 * print memory for debugging and visualization
 */
void print_memory(Memory *m, FILE *f) {
    List node = m->chunks;
    int sum_taken = 0;

    //----------------------------------------------------------------------
    // visualize memory
    while(node) {
        Chunk *chunk = (Chunk*) node->data;
        char rep = FREE_CHAR;
        int repeat = round(1.0f * MEM_PRINT_LEN / m->size * chunk->size);

        if(chunk->taken) {
            rep = TAKEN_CHAR;
            sum_taken += chunk->size;
        }

        if(repeat == 0)
            repeat = 1;

        for(int i = 0; i < repeat; i++) {
            if(i == 0)
                fprintf(f, "%d", chunk->taken);
            else
                fprintf(f, "%c", rep);
        }

        fprintf(f, "|");

        node = node->next;
    }

    fprintf(f, "\n");
    fprintf(f, "%4d MB / %d MB  |  %.2f%% Used\n",
            sum_taken, m->size, 100.0f * sum_taken / m->size);

    //----------------------------------------------------------------------
    // print arrival list
    List arr = m->arrivals;
    fprintf(f, "Arrival [ ");
    while(arr) {
        Process *p = (Process*)arr->data;
        fprintf(f, "%d:%d, ", p->pid, p->elapsed);
        arr = arr->next;
    }
    fprintf(f, " ]\n");

    //----------------------------------------------------------------------
    // print process queue
    List prc = m->processes;
    fprintf(f, "Processes [ ");
    while(prc) {
        Process *p = (Process*)prc->data;
        fprintf(f, "%d:%d, ", p->pid, p->elapsed);
        prc = prc->next;
    }
    fprintf(f, " ]\n");


}

/**
 * process the first in the head of the queue of process
 * and return the progress or remaining time left
 */
int process_memory_head(Memory *m, int quantum) {
    List plist = m->processes;

    if(!plist)
        return NOTHING_TO_PROCESS;

    Process *p = (Process*)plist->data;
    p->elapsed += quantum;

    int diff;

    // finished processing, return remaining time left
    if((diff = p->elapsed - p->burst) >= 0)
        return diff;
    // Processed but not finish
    return PROCESSED;
}

/**
 * requeue head process of the queue to the tail
 */
void requeue_memory_head(Memory *m, Process *p) {
    if(m->processes->data == p)
        head_to_tail(&(m->processes));
} 

/**
 * free the head process of the queue
 */
void free_memory_head(Memory *m) {
    // remove from processes;
    Process *p = pop(&(m->processes));

    // remove from arrivals
    del(process_eq, p, &(m->arrivals));

    // remove from memory
    List node = m->chunks;
    while(node) {
        Chunk *c = node->data;
        if(c->taken == p->pid) {
            c->taken = 0;
            break;
        }
        node = node->next;
    }

    free_process(p);
    merge_empty_slots(m);
}

/**
 * count number of processes in memory
 */
int process_count(Memory *m) {
    int total = 0;
    List alist = m->arrivals;
    while(alist) {
        total++;
        alist = alist->next;
    }
    return total;
}

/**
 * count number of holes in memory
 */
int hole_count(Memory *m) {
    int total = 0;
    List clist = m->chunks;

    while(clist) {
        Chunk *c = clist->data;
        if(c->taken == 0)
            total++;
        clist = clist->next;
    }
    return total;
}

/**
 * calculate memory usage % in whole number
 */
int usage_calc(Memory *m) {
    int total = 0;
    List clist = m->chunks;

    while(clist) {
        Chunk *c = clist->data;
        if(c->taken)
            total += c->size;
        clist = clist->next;
    }

    // round up
    // floored value + whether it fits perfectly or not
    return (100 * total) / m->size + !!((100 * total) % m->size);

    // alternative
    //return (100 * total + m->size - 1) / m->size;
}


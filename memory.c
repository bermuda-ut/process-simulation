/*=============================================================================
#     FileName: memory.c
#         Desc: functions required for memory operations
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 10:30:32
=============================================================================*/
#include "memory.h"

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

void free_chunk(void *c) {
    free(c);
}

Chunk *new_chunk(int size) {
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->size = size;
    chunk->taken = 0;

    return chunk;
}

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

Process *oldest_process(Memory *m) {
    return (m->arrivals->data);
}

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

int load_to_memory(Memory *m, Process *p) {
    return m->fit_strategy(m, p);
}

void print_memory(Memory *m, FILE *f) {
    List node = m->chunks;
    int sum_taken = 0;

    while(node) {
        Chunk *chunk = (Chunk*) node->data;
        char rep = FREE_CHAR;
        int repeat = round(1.0f * MEM_PRINT_LEN / m->size * chunk->size);

        if(chunk->taken) {
            rep = TAKEN_CHAR;
            sum_taken += chunk->size;
        }

        for(int i = 0; i < repeat; i++)
            fprintf(f, "%c", rep);

        node = node->next;
    }

    fprintf(f, "\n");
    fprintf(f, "%4d MB / %d MB  |  %.2f%% Used\n", sum_taken, m->size, 100.0f * sum_taken / m->size);

    List arr = m->arrivals;
    fprintf(f, "[ ");
    while(arr) {
        Process *p = (Process*)arr->data;
        fprintf(f, "%d:%d, ", p->pid, p->elapsed);
        arr = arr->next;
    }
    fprintf(f, " ]\n");
}

int process_memory_head(Memory *m, int quantum) {
    // res = -1 = did not finish processing, need to requeue process
    List plist = m->processes;

    if(!plist)
        return NOTHING_TO_PROCESS;

    Process *p = (Process*)plist->data;
    p->elapsed += quantum;

    int diff;
    if((diff = p->elapsed - p->burst) >= 0)
        return diff;

    return PROCESSED;
}

void requeue_memory_head(Memory *m) {
    // res = -1 = there is nothing to process in memory!
    head_to_tail(&(m->processes));
} 

void free_memory_head(Memory *m) {
    // res = 0,1,2... = finished processing, need to delete process
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

int process_count(Memory *m) {
    int total = 0;
    List alist = m->arrivals;
    while(alist) {
        total++;
        alist = alist->next;
    }
    return total;
}

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

int usage_calc(Memory *m) {
    int total = 0;
    List clist = m->chunks;

    while(clist) {
        Chunk *c = clist->data;
        if(c->taken)
            total += c->size;
        clist = clist->next;
    }

    return (int)ceil(100.0f * total / m->size);
}

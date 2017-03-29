/*=============================================================================
#     FileName: storage.c
#         Desc: functions required for storage operation
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 10:32:33
=============================================================================*/
#include "storage.h"

/*
 * Create and return new allocated storage
 * */
Storage *new_storage() {
    Storage *s = malloc(sizeof(Storage));
    s->blocks = NULL;
    return s;
}

/**
 * nlogn n^2
 * Bubble sort according to priority and process id
 * */
void sort_storage(Storage *s) {
    List blist = s->blocks;
    void* endpoint = NULL;

    //int iter = 0;
    while(blist != endpoint) {
        //fprintf(stderr, "iter %d blist %p end %p\n", iter, blist, endpoint);

        while(blist) {
            if(blist->next == endpoint) {
                endpoint = blist;
                break;
            }

            Block* cb = (Block*)blist->data;
            Block* nb = (Block*)blist->next->data;

            if(cb->arrival > nb->arrival) {
                // 1. compare arrival time
                swap_list_position(&blist);
            } else if (cb->arrival == nb->arrival) {
                // 2. compare pid
                Process* cp = cb->process;
                Process* np = nb->process;
                if(cp->pid > np->pid) {
                    swap_list_position(&blist);
                }
            }

            blist = blist->next;
        }

        blist = s->blocks;
        //iter++;
    }
}

void add_process_storage(Process *p, Storage* s, int arrival) {
    Block* block = malloc(sizeof(Block));
    block->process = p;
    block->arrival = arrival;

    if(s->blocks) {
        insert(block, &(s->blocks));
    } else {
        s->blocks = push(NULL, block);
    }

    sort_storage(s);
    //fprintf(stderr, "adding %d\n", p->pid);
}

void load_to_storage(List *plist, Storage *s, int progress) {
    List curr = *plist;

    while(curr) {
        // load plist to storage IF head plist arrival >= progress
        Process *p = (Process*)(curr->data);

        if(progress >= p->arrival) {
            add_process_storage(pop(plist), s, p->arrival);
            fprintf(stderr, "(PID:%d) plist > storage\n", p->pid);
            curr = *plist;
        } else {
            break;
        }
        //curr = curr->next;
    }
    sort_storage(s);
}

void memory_to_storage(Storage *s, Memory *m, int arrival) {
    if(!m->arrivals) {
        fprintf(stderr, "[CRITICAL ERROR] Nothing in arrival!\n");
        fprintf(stderr, "                 Processes %p\n", m->processes);
        exit(EXIT_FAILURE);
    }

    Process *oldest = pop(&(m->arrivals));

    // insert into storage
    add_process_storage(oldest, s, arrival);

    // delete from process queue
    del(process_eq, oldest, &(m->processes));

    // delete from memory
    List node = m->chunks;
    //fprintf(stderr, "--\n");
    while(node) {
        Chunk *c = (Chunk*) node->data;
        //fprintf(stderr, "%d %d\n", c->taken, oldest->pid);
        if(c->taken == oldest->pid) {
            c->taken = 0;
            break;
        }
        node = node->next;
    }

    fprintf(stderr, "(PID:%d) memory > storage\n", oldest->pid);

    merge_empty_slots(m);
    sort_storage(s);
}

void storage_to_memory(Storage *s, Memory *m, int arrival) {
    // from storage, load oldest process to m
    if(!s->blocks)
        return;

    Block *b = pop(&(s->blocks));
    Process *p = b->process;
    //fprintf(stderr, "p %d\n", p->memsize);
    int res;
    while(!(res = load_to_memory(m, p))) {
        memory_to_storage(s, m, arrival);
    }
    fprintf(stdout,
            "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n", 
            arrival, p->pid, process_count(m), hole_count(m), usage_calc(m));
}

void print_storage(Storage *s, FILE *f) {
    List blist = s->blocks;
    fprintf(f, "Storage { ");
    while(blist) {
        Block *b = (Block*)blist->data;
        Process *p = b->process;
        fprintf(f, "[%d]%d:%d, ", b->arrival, p->pid, p->elapsed);
        blist = blist->next;
    }
    fprintf(f, " } \n");
}

void free_storage(Storage *s) {
    free_list_data(free_block, &(s->blocks));
    free(s);
}

void free_block(void *b) {
    Block *bp = b;

    if(bp->process)
        free_process(bp->process);

    free(bp);
}

/*=============================================================================
#     FileName: strategy.c
#         Desc: memory fit strategies
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 11:01:21
=============================================================================*/
#include "strategy.h"

int first_fit(Memory *m, Process *p) {
    List node = m->chunks;

    while(node) {
        Chunk *chunk = node->data;
        int remain = chunk->size - p->memsize;
        // first fit! 
        if(chunk->taken == 0 && remain >= 0) {
            chunk->taken = p->pid;
            chunk->size = p->memsize;

            // create new chunk
            if(remain > 0) {
                Chunk *temp = new_chunk(remain);
                List second = push(NULL, temp);
                List third = node->next;
                second->next = third;
                node->next = second;
            }

            merge_empty_slots(m);

            // add to arrival list
            if(m->arrivals)
                insert(p, &(m->arrivals));
            else
                m->arrivals = push(NULL, p);

            // add to process queue
            if(m->processes)
                insert(p, &(m->processes));
            else
                m->processes = push(NULL, p);

            return 1;
        }

        node = node->next;
    }
    return 0;
}

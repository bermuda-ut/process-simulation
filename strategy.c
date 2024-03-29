/*=============================================================================
#     FileName: strategy.c
#         Desc: memory fit strategies
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 14:28:35
=============================================================================*/
#include "strategy.h"

int first_fit(Memory *m, Process *p) {
    List node = m->chunks;

    while(node) {
        Chunk *chunk = node->data;
        int remain = chunk->size - p->memsize;
        // first fit! 
        if(chunk->taken == 0 && remain >= 0) {
            fit_process_in(m, p, node);
            return 1;
        }

        node = node->next;
    }

    // failed to fit
    return 0;
}

int worst_fit(Memory *m, Process *p) {
    List chosenNode = NULL;

    int maxsize = 0;

    List node = m->chunks;

    while(node) {
        Chunk *chunk = node->data;
        int remain = chunk->size - p->memsize;

        if(chunk->taken == 0 && remain >= 0 && chunk->size > maxsize) {
            chosenNode = node;
            maxsize = chunk->size;
        }

        node = node->next;
    }

    if(chosenNode) {
        // worst fit! 
        fit_process_in(m, p, chosenNode);
        return 1;
    }

    // failed to fit
    return 0;
}

int best_fit(Memory *m, Process *p) {
    List chosenNode = NULL;
    int maxsize = -1;

    List node = m->chunks;

    while(node) {
        Chunk *chunk = node->data;
        int remain = chunk->size - p->memsize;

        if(chunk->taken == 0 && remain >= 0 && (chunk->size < maxsize || maxsize == -1)) {
            chosenNode = node;
            maxsize = chunk->size;
        }

        node = node->next;
    }

    if(chosenNode) {
        // best fit! 
        fit_process_in(m, p, chosenNode);
        return 1;
    }

    // failed to fit
    return 0;
}

void fit_process_in(Memory *m, Process *p, List node) {
    Chunk *chunk = node->data;
    int remain = chunk->size - p->memsize;

    chunk->taken = p->pid;
    chunk->size = p->memsize;

    // create new chunk and add to the list appropriately
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
}

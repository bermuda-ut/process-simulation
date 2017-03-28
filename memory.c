#include <math.h>
#include "memory.h"

#define FREE_CHAR '-'
#define TAKEN_CHAR '+'
#define MEM_PRINT_LEN 40

void free_memory(Memory *m) {
    free(m);
}

void free_chunk(Chunk *c) {
    free(c);
}

Chunk *new_chunk(int size) {
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->size = size;
    chunk->taken = 0;

    return chunk;
}

Memory *new_memory(char* strategy, int size) {
    Memory *mem = malloc(sizeof(Memory));
    Chunk *init = new_chunk(size);

    mem->list = push(NULL, init);
    mem->arrivals = NULL;
    mem->size = size;
    return mem;
}

int in_memory(Memory *m, int pid) {
    List node = m->list;

    while(node) {
        Chunk *chunk = node->data;
        if(chunk->taken == pid)
            return 1;

        node = node->next;
    }

    return 0;
}

Process *oldest_process(Memory *m) {
    return (m->arrivals->data);
}

int load_memory(Memory *m, Process *p) {
    List node = m->list;

    while(node) {
        Chunk *chunk = node->data;
        int remain = chunk->size - p->memsize;
        // can insert
        if(chunk->taken == 0 && remain >= 0) {
            chunk->taken = p->pid;
            chunk->size = p->memsize;

            if(remain > 0) {
                Chunk *temp = new_chunk(remain);
                List second = push(NULL, temp);
                List third = node->next;
                second->next = third;
                node->next = second;
            }

            if(m->arrivals)
                insert(p, &(m->arrivals));
            else
                m->arrivals = push(NULL, p);

            return 1;
        }

        node = node->next;
    }
    return 0;
}

bool tempest_eq(void *aim, void *node) {
    Process *p = (Process*)node;
    int pid = *((int*)aim);
    return(p->pid == pid);

}

void unload_memory(Memory *m, int pid) {
    List node = m->list;

    while(node) {
        Chunk *chunk = node->data;
        // found to remove
        if(chunk->taken == pid) {
            chunk->taken = 0;
            Chunk *nextChunk = node->next->data;

            // need to merge
            if(nextChunk->taken == 0) {
                Chunk *merged = new_chunk(chunk->size + nextChunk->size);
                free_chunk(node->data);
                free_chunk(nextChunk);
                node->data = merged;
                node->next = node->next->next;
            }

            // remove from arrivals list
            del(tempest_eq, &pid, &(m->arrivals));

            return;
        }

        node = node->next;
    }
}

void print_memory(Memory *m, FILE *f) {
    List node = m->list;
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
        fprintf(f, "%d, ", p->pid);
        arr = arr->next;
    }
    fprintf(f, " ]\n");
}

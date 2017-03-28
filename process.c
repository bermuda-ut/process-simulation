#include "process.h"

bool process_eq(void *target, void* node) {
    Process *t = target;
    Process *n = node;
    return (n->pid == t->pid);
}

Process *new_process(int arrival, int pid, int memsize, int burst) {
    Process* p = malloc(sizeof(Process));
    p->arrival = arrival;
    p->pid = pid;
    p->memsize = memsize;
    p->burst = burst;
    p->elapsed = 0;

    return p;
}

void free_process(Process *p) {
    free(p);
}


/*=============================================================================
#     FileName: process.c
#         Desc: implementation of process
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 10:41:26
=============================================================================*/
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

void free_process(void *p) {
    free(p);
}


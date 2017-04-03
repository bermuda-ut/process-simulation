/*=============================================================================
#     FileName: process.c
#         Desc: implementation of process
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-04-03 13:45:16
=============================================================================*/
#include "process.h"

/**
 * Equal function used to check of two processes are the same
 */
bool process_eq(void *target, void* node) {
    Process *t = target;
    Process *n = node;
    return (n->pid == t->pid);
}

/**
 * malloc new process and return pointer to it
 */
Process *new_process(int arrival, int pid, int memsize, int burst) {
    Process* p = malloc(sizeof(Process));
    p->arrival = arrival;
    p->pid = pid;
    p->memsize = memsize;
    p->burst = burst;
    p->elapsed = 0;

    return p;
}

/**
 * free process
 */
void free_process(void *p) {
    free(p);
}


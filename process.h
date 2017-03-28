/*=============================================================================
#     FileName: process.h
#         Desc: decleration of required operations for process simulation
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 10:41:39
=============================================================================*/
#ifndef PROCESS
#define PROCESS

#include <stdlib.h>
#include "list.h"

typedef struct process_t Process;
struct process_t {
    int arrival,    // time of arrival according to input
        pid,        // process ID
        memsize,    // memory size it takes up
        burst,      // required time to finish processing
        elapsed;    // time spent so far on this process to process
};

extern Process *new_process(int arrival, int pid, int memsize, int burst);
extern void free_process(void *p);
extern bool process_eq(void *target, void* node);

#endif

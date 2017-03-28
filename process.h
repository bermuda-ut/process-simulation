#ifndef PROCESS
#define PROCESS

#include <stdlib.h>
#include "list.h"

typedef struct process_t Process;
struct process_t {
    int arrival, 
        pid, 
        memsize, 
        burst,
        elapsed;
};

extern Process *new_process(int arrival, int pid, int memsize, int burst);
//extern Process *oldest_pid(List plist);
extern void free_process(Process *p);
extern bool process_eq(void *target, void* node);

//extern int process_pid(int pid, int elapse, List *plist);

#endif

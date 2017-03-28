#include "process.h"

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

/*
#define OLDEST_ARRIVAL 9999
Process *oldest_pid(List plist) {
    List curr = plist;
    int arrival = OLDEST_ARRIVAL;
    Process *p = NULL;

    while(curr) {
        Process *cp = (Process*)curr->data;
        if(cp->arrival < arrival) {
            arrival = cp->arrival;
            p = cp;
        }
        curr = curr->next;
    }

    return p;
}

bool del_process(void *aim, void *node) {
    Process *nodep = (Process*) ((List)node)->data;
    Process *aimp = (Process*) ((List)aim)->data;
    if(aimp->pid == nodep->pid)
        return 0;
    return 1;
}
*/


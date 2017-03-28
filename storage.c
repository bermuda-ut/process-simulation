#include "storage.h"

Storage *new_storage() {
    Storage *s = malloc(sizeof(Storage));
    s->processes = NULL;
    return s;
}


void load_to_storage(List *plist, Storage *s, int progress) {
    //TODO: PRIORITY
    List curr = *plist;
    while(curr) {
        // load plist to storage IF head plist arrival >= progress
        Process *p = (Process*)(curr->data);

        if(progress >= p->arrival) {
            if(s->processes) {
                insert(pop(plist), &(s->processes));
            } else {
                s->processes = push(NULL, pop(plist));
            }
        } else {
            break;
        }
        curr = curr->next;
    }
}

void memory_to_storage(Storage *s, Memory *m) {
    // from memory, load oldest process to s
    // delete from arrivals queue
    // arrival is emplty for some reason
    if(!m->arrivals) {
        //fprintf(stderr, "nothing in arrival!\n");
        //fprintf(stderr, "processes %p\n", m->processes);
        return;
    }
    Process *oldest = pop(&(m->arrivals));

    // insert into storage
    if(s->processes)
        insert(oldest, &(s->processes));
    else
        s->processes = push(NULL, oldest);

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

    merge_empty_slots(m);
}

void storage_to_memory(Storage *s, Memory *m) {
    // from storage, load oldest process to m
    if(!s->processes)
        return;

    Process *p = pop(&(s->processes));
    //fprintf(stderr, "p %d\n", p->memsize);
    int res;
    while(!(res = load_to_memory(m, p))) {
        memory_to_storage(s, m);
    }
}

void print_storage(Storage *s, FILE *f) {
    List slist = s->processes;
    fprintf(f, "Storage { ");
    while(slist) {
        Process *p = (Process*)slist->data;
        fprintf(f, "%d:%d, ", p->pid, p->elapsed);
        slist = slist->next;
    }
    fprintf(f, " } \n");
}

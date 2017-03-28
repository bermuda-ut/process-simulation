/* Read a number and determine if it is prime.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "memory.h"
#include "list.h"

#define INPUT_SIZE 4
#define READ_SIZE 4

void simulate_process(List plist, Memory *memory, int quantum);
void print_plist_node(FILE *f, void* data);
void print_inputs(FILE *f, char **inputs);
FILE *open_file(char* file, char* format);
List generate_process_list(FILE *fp);
void load_from_disk(Memory *memory, List *plist, List *currPlist, int progress);
int process_recent(int elapse, List *plist, Memory *memory);
bool process_eq(void *aim, void *node);

int main(int argc, char **argv) {
    // pointer declare

    // set relations
    const char *flags[] = { "-f", "-a", "-m", "-q" };
    char **inputs = malloc(sizeof(char*) * INPUT_SIZE);

    // read input
    for(int i = 0; i < argc; i++) {
        for(int j = 0; j < INPUT_SIZE; j++) {
            if(strcmp(argv[i], flags[j]) == 0) {
                inputs[j] = argv[i + 1];
            }
        }
    }

    // output what has been read
    print_inputs(stderr, inputs);

    // attempt to open file
    FILE *fp = open_file(inputs[0], "r");

    // read lines and get integer values
    List plist = generate_process_list(fp);

    fclose(fp);

    // initiate memory
    Memory *m = new_memory(inputs[1], atoi(inputs[2]));

    // start processing!
    simulate_process(plist, m, atoi(inputs[3]));

    free(inputs);
    free_list(plist);
    free_memory(m);

    exit(EXIT_SUCCESS);
}


void simulate_process(List plist, Memory *memory, int quantum) {
    int progress = 0;
    bool requeue = 0;
    // assert always start at 0 with first processes
    List currPlist = NULL; // ie currently loaded shit
    //void load_from_disk(Memory *memory, List *plist, List *currPlist, int progress) {
    
    while(plist || currPlist) {
        // load oldest, highest priority to back of the queue
        load_from_disk(memory, &plist, &currPlist, progress);
        fprintf(stderr, "plist: %p\n", plist);

        // put the processed process from last iteration to the back of the queue
        if(requeue) {
            Process *todo = (Process*) currPlist->data;
            pop(&currPlist);
            insert(todo, &currPlist);
        }

        // process the head of the queue
        int remain = 0;
        if(currPlist) {
            // head of the queue process
            Process *computing = (Process*) currPlist->data;
            // segfault here
            int pid = computing->pid;

            // process the head, 
            // if finshed, remove from queue and memory
            //fprintf(stderr, "Processed %d\n", pid);
            remain = process_recent(quantum, &currPlist, memory);

            fprintf(stderr, "= MEM QUEUE:\n");
            print_list(print_plist_node, stderr, currPlist);

            // have not finishe processing,
            // next iteration, need to move this process to the end
            if(remain == -1) {
                requeue = 1;
                remain = 0;
                fprintf(stderr, "Proessed %d\n", pid);
            } else {
                fprintf(stderr, "Finshed %d\n", pid);
            }

            progress += quantum - remain;
            fprintf(stderr, "Elapsed %d, Remain %d\n", progress, remain);
        } else {
            requeue = 0;
            progress += 1;
        }
    }

    fprintf(stderr, "\n\n");
    print_memory(memory, stderr);
    fprintf(stderr, "DONE %d\n", progress);
}

void load_from_disk(Memory *memory, List *plist, List *currPlist, int progress) {
    if(*plist || *currPlist) {
        fprintf(stderr, "\n\n");

        // swap
        if(plist && ((Process*)((*plist)->data))->arrival <= progress) {
            Process *nextProcess = (*plist)->data;
            Process *upcoming = (*plist)->next->data;

            // chose lowest pid
            if(upcoming->arrival == nextProcess->arrival) {
                List temp = *plist;
                int lowest = ((Process*)(temp->data))->pid;
                while(((Process*)(temp->data))->arrival == nextProcess->arrival) {
                    int cmp = ((Process*)(temp->data))->pid;
                    if(lowest > cmp) {
                        lowest = cmp;
                    }
                    temp = temp->next;
                }

                List prev = NULL;
                temp = *plist;
                while(((Process*)(temp->data))->arrival == nextProcess->arrival) { 
                    int pid = ((Process*)(temp->data))->pid;
                    if(pid == lowest) {
                        if(prev)
                            prev->next = temp->next;

                        Process *data = temp->data;
                        *plist = push(*plist, data);
                        nextProcess = data;
                        free(temp);
                        break;
                    }
                    prev = temp;
                    temp = temp->next;
                }
            }

            if(currPlist == NULL)
                *currPlist = push(NULL, (*plist)->data);
            else
                insert(nextProcess, currPlist);

            fprintf(stderr, "Loaded %d\n", nextProcess->pid);
            fprintf(stdout, "time %d, %d loaded\n", progress, nextProcess->pid);
            pop(plist);

            while(!in_memory(memory, nextProcess->pid)) {
                int success = load_memory(memory, nextProcess);
                List temp = NULL;

                if(!success) {
                    Process *oldest = oldest_process(memory);
                    del(process_eq, oldest, currPlist);

                    fprintf(stderr, "Memory full! Unloaded PID:%d\n", oldest->pid);
                    unload_memory(memory, oldest->pid);

                    if(temp == NULL)
                        temp = push(NULL, oldest);
                    else
                        insert(oldest, &temp);
                }

                append(temp, plist);
            }

            print_memory(memory, stderr);
            fprintf(stderr, "= DISK: \n");
            print_list(print_plist_node, stderr, *plist);
        }
    }
}

void print_plist_node(FILE *f, void* data) {
    Process *p = (Process*)data;
    fprintf(f, "A:%3d\tPID:%d\tsize:%3d\tburst:%3d\telapsed:%d\n", p->arrival, p->pid, p->memsize, p->burst, p->elapsed);
}

void print_inputs(FILE *f, char **inputs) {
    fprintf(f, "FILENAME : %s\n", inputs[0]);
    fprintf(f, "ALGO     : %s\n", inputs[1]);
    fprintf(f, "MEMSIZE  : %s\n", inputs[2]);
    fprintf(f, "QUANT    : %s\n", inputs[3]);
}

FILE *open_file(char* file, char* format) {
    FILE *fp = fopen(file, format);
    if (fp == NULL) {
        fprintf(stderr, "Could not read file. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

List generate_process_list(FILE *fp) {
    List plist = NULL;
    int* lineRead = malloc(sizeof(int) * READ_SIZE);
    while (fscanf(fp, "%d %d %d %d", lineRead, lineRead+1, lineRead+2, lineRead+3) == READ_SIZE) {
        Process *data = new_process(lineRead[0], lineRead[1], lineRead[2], lineRead[3]);

        if(plist)
            insert(data, &plist);
        else
            // first insertion ;)
            plist = push(NULL, data);
    }
    free(lineRead);
    return plist;
}

/*
 * Process the first one for X seconds
 * returns remaining elapse time
 * if still elapsed < burst, puts to the back of the queue
 * */
int process_recent(int elapse, List *plist, Memory *memory) {
    Process *todo = (*plist)->data;
    todo->elapsed += elapse;

    // finished
    if(todo->burst <= todo->elapsed) {
        int diff = todo->elapsed - todo->burst;
        unload_memory(memory, todo->pid);
        free_process(todo);
        pop(plist);

        return diff;
    }

    // put back since have not finished processing
    // insert((void *)todo, plist);
    return -1;
}

bool process_eq(void *aim, void *node) {
    Process *aimP = (Process*) aim;
    Process *nodeP = (Process*) node;
    return aimP->pid == nodeP->pid;
}


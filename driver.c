/*=============================================================================
#     FileName: driver.c
#         Desc: driver program for the simulation
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 10:36:52
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "process.h"
#include "storage.h"
#include "memory.h"
#include "strategy.h"

#define INPUT_SIZE 4
#define READ_SIZE 4

List generate_process_list(FILE *fp);
void simulate_process(List plist, Memory *memory, Storage *s, int quantum);

void print_inputs(FILE *f, char **inputs);
void print_plist_node(FILE *f, void* data);
FILE *open_file(char* file, char* format);

int main(int argc, char **argv) {
    // set relations
    const char *flags[] = { "-f", "-a", "-m", "-q" };
    char **inputs = malloc(sizeof(char*) * INPUT_SIZE);

    // read input
    for(int i = 0; i < argc; i++)
        for(int j = 0; j < INPUT_SIZE; j++)
            if(strcmp(argv[i], flags[j]) == 0)
                inputs[j] = argv[i + 1];

    // output what has been read
    print_inputs(stderr, inputs);

    // attempt to open file
    FILE *fp = open_file(inputs[0], "r");

    // read lines and get integer values
    List plist = generate_process_list(fp);

    fclose(fp);

    // initiate memory
    Memory *m;
    if(strcmp(inputs[1], FIRST_FIT) == 0) {
        m = new_memory(first_fit, atoi(inputs[2]));

    } else if(strcmp(inputs[1], WORST_FIT) == 0) {
        m = new_memory(worst_fit, atoi(inputs[2]));

    } else if(strcmp(inputs[1], BEST_FIT) == 0) {
        m = new_memory(best_fit, atoi(inputs[2]));

    } else {
        fprintf(stderr, "Invalid strategy defined\n");
        exit(EXIT_FAILURE);
    }
    Storage *s = new_storage();

    // start processing!
    simulate_process(plist, m, s, atoi(inputs[3]));

    // clean up
    free(inputs);
    free_list(plist);
    free_memory(m);
    free_storage(s);

    exit(EXIT_SUCCESS);
}


void simulate_process(List plist, Memory *m, Storage *s, int quantum) {
    int progress = 0;
    bool requeue = 0;

    // while there are stuff in arrivals or plist
    while(s->blocks || plist || m->arrivals) {
        fprintf(stderr, "\n\n\t\t\tPROGRESSS %d \n", progress);

        // load plist to storage IF head plist arrival >= progress
        load_to_storage(&plist, s, progress);

        // from storage, load oldest process to m
        storage_to_memory(s, m, progress);

        // from prev interation, requeue if needed
        if(requeue) {
            requeue_memory_head(m);
            requeue = 0;
            fprintf(stderr, "Requeued unfinished process from last iteration\n");
        }

        // print status
        print_list(print_plist_node, stderr, plist);
        print_storage(s, stderr);
        print_memory(m, stderr);

        // add elapsed the head process of the memory
        int res = process_memory_head(m, quantum);

        if(res == PROCESSED) {
            // res = -1 = did not finish processing, need to requeue process
            // fprintf(stderr, "back to queue\n");
            requeue = 1;
            progress += quantum;
        } else if(res == NOTHING_TO_PROCESS) {
            fprintf(stderr, "Nothing to process!\n");
            // res = -2 = nothing to process, kill time
            progress += 1;
        } else {
            fprintf(stderr, "Finished processing PID:%d\n", ((Process*)(m->processes->data))->pid);
            // res = 0,1,2... = finished processing, need to delete process
            free_memory_head(m);
            progress += quantum - res;
        }
    }

    fprintf(stdout, "time %d, simulation finished.\n", progress);
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


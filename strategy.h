/*=============================================================================
#     FileName: strategy.h
#         Desc:  
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-04-03 13:58:39
=============================================================================*/
#include "memory.h"
#include "process.h"

#define FIRST_FIT "first"
#define WORST_FIT "worst"
#define BEST_FIT "best"

/**
 * Check C file for function details
 */
extern int first_fit(Memory *m, Process *p);
extern int worst_fit(Memory *m, Process *p);
extern int best_fit(Memory *m, Process *p);

void fit_process_in(Memory *m, Process *p, List node);

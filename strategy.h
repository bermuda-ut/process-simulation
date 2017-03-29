/*=============================================================================
#     FileName: strategy.h
#         Desc:  
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 11:04:44
=============================================================================*/
#include "memory.h"
#include "process.h"

#define FIRST_FIT "first"
#define WORST_FIT "worst"
#define BEST_FIT "best"

extern int first_fit(Memory *m, Process *p);
extern int worst_fit(Memory *m, Process *p);
extern int best_fit(Memory *m, Process *p);

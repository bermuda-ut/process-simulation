/*=============================================================================
#     FileName: advlist.h
#         Desc: Extension of existing list libarary
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-29 10:23:27
=============================================================================*/
#ifndef ADVLIST
#define ADVLIST

#include "list.h"

extern void free_list_data(void (*freefunc)(void *data), List *list);
extern void head_to_tail(List *list);
extern void swap_list_position(List *list);
#endif

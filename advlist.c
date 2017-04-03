/*=============================================================================
#     FileName: advlist.c
#         Desc: implementation of extended list functions
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-04-03 13:31:26
=============================================================================*/
#include "advlist.h"

/**
 * move head of the list to the tail
 */
void head_to_tail(List *list) {
    void* data = pop(list);
    insert(data, list);
}

/**
 * swap data with the next element's data
 */
void swap_list_position(List *list) {
    // 'pretends' to swap, just swaps the data
    void *d1 = (*list)->data;
    void *d2 = (*list)->next->data;
    (*list)->data = d2;
    (*list)->next->data = d1;
}

/**
 * free all the data in the list with the specified free function
 */
void free_list_data(void (*freefunc)(void *data), List *list) {
    List node = *list;
    while(node) {
        if(node->data)
            freefunc(node->data);
        node = node->next;
    }
}

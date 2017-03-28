#include "advlist.h"

void head_to_tail(List *list) {
    void* data = pop(list);
    insert(data, list);
}

void swap_list_position(List *list) {
    // 'pretends' to swap, just swaps the data
    void *d1 = (*list)->data;
    void *d2 = (*list)->next->data;
    (*list)->data = d2;
    (*list)->next->data = d1;
}

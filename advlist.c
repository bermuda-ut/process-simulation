#include "advlist.h"

void head_to_tail(List *list) {
    void* data = pop(list);
    insert(data, list);
}

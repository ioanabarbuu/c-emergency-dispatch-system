#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include "types.h"

circular_ll *create_cll(unsigned int data_size);
int deep_copy_incident(incident *dst, incident *src);
cll_node *cll_add_node(circular_ll *list, void* new_data);
cll_node *cll_remove_nth_node(circular_ll *list, int n);
void cll_free(circular_ll **list);

#endif
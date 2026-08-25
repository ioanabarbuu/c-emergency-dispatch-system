#include <stdlib.h>
#include <string.h>
#include "circular_list.h"

//function that creates a linked list with a santinel and
//creates the santinel depending on what the list stores
circular_ll *create_cll(unsigned int data_size)
{
	circular_ll *list = malloc(sizeof(circular_ll));
	if (!list) {
		return NULL;
	}
	cll_node *santinel = malloc(sizeof(cll_node));
	if (!santinel) {
		free(list);
		return NULL;
	};
	santinel->data = malloc(data_size);
	if (!santinel->data) {
		free(santinel);
		free(list);
		return NULL;
	};
	//initialize the santinel!
	list->head = santinel;
	list->head->next = santinel;
	list->head->prev = santinel;
	//the real list size is calculated without the santinel
	list->size = 0;
	list->data_size = data_size;
    return list;
}


//function that deep copies an incident into a node
int deep_copy_incident(incident *dst, incident *src) {
	dst->id = src->id;
	strcpy(dst->priority, src->priority);
	strcpy(dst->status, src->status);
	dst->description = malloc(strlen(src->description) + 1);
	if (!dst->description) {
		return 0;
	}
	strcpy(dst->description, src->description);
	return 1;
}

//function that adds a node at the end of a circular linked list
//with a santinel
cll_node *cll_add_node(circular_ll *list, void* new_data)
{
	cll_node *node = malloc(sizeof(cll_node));
	if (!node) {
		return NULL;
	};
	node->data = malloc(list->data_size);
	if (!node->data) {
		free(node);
		return NULL;
	};
	//a shallow copy is enough if the node is not an incident
	if (list->data_size == sizeof(incident)) {
		if(!deep_copy_incident((incident*)node->data, new_data)) {
			free(node->data);
			free(node);
			return NULL;
		}
	} else {
		memcpy(node->data, new_data, list->data_size);
	}
	node->next = list->head;
	node->prev = list->head->prev;
	list->head->prev->next = node;
	list->head->prev = node;
	list->size++;
	return node;
}

//function that removes a node at the end of a circular linked list
//with a santinel
cll_node *cll_remove_nth_node(circular_ll *list, int n)
{
	if (list->size == 0) {
		return NULL;
	};
	cll_node *removed;
	cll_node *curr = list->head->next;
	int position = 0;
	while (position < n - 1 && (unsigned int)position < list->size - 1) {
		curr = curr->next;
		position++;
	};
	if (n) {
		removed = curr->next;
	} else {
		removed = curr;
		curr = list->head;
	}
	removed->next->prev = curr;
	curr->next = removed->next;
	list->size--;
	return removed;
}

//function that frees a list
void cll_free(circular_ll **list)
{   
	int first = 1;
    cll_node *curr = (*list)->head;
    while(curr != (*list)->head || first) {
		if (first) {
			first = 0;
		};
		if((*list)->data_size == sizeof(incident)) {
			free(((incident*)curr->data)->description);
		}
        cll_node *next = curr->next;
		free(curr->data);
        free(curr);
        curr = next;
    };
    free(*list);
    *list = NULL;
}
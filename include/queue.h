#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

queue *create_queue(unsigned int max_size);
void *dequeue(queue *q);
int enqueue(queue *q, void *new_data);
int enqueue_to_start(queue *q, void *new_data);
void queue_free(queue *q);

#endif
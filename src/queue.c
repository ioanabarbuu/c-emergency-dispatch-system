#include <stdlib.h>
#include "queue.h"

//function that creates a queue
//the queue is implemented as a circular array
queue *create_queue(unsigned int max_size)
{	
	queue *q = malloc(sizeof(struct queue));
	if (!q) {
		return NULL;
	};
	q->max_size = max_size;
	q->size = 0;
	q->read_idx = 0;
	q->write_idx = 0;
	q->buff = malloc(max_size * sizeof(void*));
	if (!q->buff) {
		free(q);
		return NULL;
	};
	return q;
}

//function that removes an element from a queue
//and returns a pointer to the removed element
void *dequeue(queue *q)
{
	if(!q->size) {
		return NULL;
	};
	void *p = q->buff[q->read_idx];
	q->read_idx++;
	if(q->read_idx > q->max_size - 1) {
		q->read_idx = 0;
	};
	q->size--;
	return p;
}

//function that adds an element to the end of a queue
int enqueue(queue *q, void *new_data)
{
	if(q->size == q->max_size) {
		return 0;
	};
	q->buff[q->write_idx] = new_data;
	q->size++;
	q->write_idx++;
	if(q->write_idx > q->max_size - 1) {
		q->write_idx = 0;
	};
	return 1;
}

//function that adds an element to the start of a queue
int enqueue_to_start(queue *q, void *new_data)
{
	if(q->size == q->max_size) {
		return 0;
	};
	q->size++;
	if(q->read_idx == 0) {
		q->read_idx = q->max_size;
	};
	q->read_idx--;
	q->buff[q->read_idx] = new_data;
	return 1;
}

//function that frees a queue
void queue_free(queue *q)
{
	free(q->buff);
	free(q);
}
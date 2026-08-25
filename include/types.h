//Copyright Barbu Ioana-Steliana, 2026
#ifndef TYPES_H
#define TYPES_H

//define unit structure
typedef struct unit
{
	int id;
	char type;
	int availability;
} unit;

//define incident structure
typedef struct incident
{
	int id;
	char priority[7];
	char *description;
	char status[11];
} incident;

//define intervention structure
typedef struct intervention
{
	struct incident *incident;
	struct unit *unit;
} intervention;

//define system structure
typedef struct system
{
	struct circular_ll *units;
	struct circular_ll *incidents;
	struct circular_ll *interventions;
} syst;

//define node of a linked list
typedef struct cll_node
{
	void *data;
	struct cll_node *prev, *next; 
} cll_node;

//define circular linked list
typedef struct circular_ll
{
	struct cll_node *head;
	unsigned int data_size;
	unsigned int size;
} circular_ll;

//define queue
typedef struct queue
{
	unsigned int max_size;
	unsigned int size;
	unsigned int read_idx;
	unsigned int write_idx;
	void **buff;
} queue;

//define a struct that contains all the queues
typedef struct all_queues
{
	struct queue *high;
	struct queue *medium;
	struct queue *low;
	struct queue *available_units;
} all_queues;

//define a stack that stores the history of the interventions
typedef struct stack
{
	struct circular_ll *interventions_stack;
} stack;

#endif
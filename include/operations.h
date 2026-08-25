#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include "types.h"

void dispatch(struct system **sys, struct all_queues **q,
				  struct stack **s, FILE *out);
cll_node *turn_incident_into_node(struct incident *incid, struct system **sys);
cll_node *turn_unit_into_node(struct unit *uni, struct system **sys);
void undo_dispatch(struct system **sys, struct all_queues **q,
				   struct stack **s, FILE *out);

#endif
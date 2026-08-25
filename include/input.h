#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include "types.h"

void read_data(struct system **sys, struct all_queues **q,
			   struct stack **s, FILE *in, FILE *out);

#endif
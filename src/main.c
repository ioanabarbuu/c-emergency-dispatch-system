#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "circular_list.h"
#include "input.h"

int main(void)
{
	FILE *in = fopen("tema1.in", "r");
	FILE *out = fopen("tema1.out", "wt");
	if(!in) {
		return 1;
	};
	syst *sys = malloc(sizeof(struct system));
	if (!sys) {
		return 1;
	};
	all_queues *q = malloc(sizeof(struct all_queues));
	if (!q) {
		return 1;
	};
	stack *s = malloc(sizeof(struct stack));
	if (!s) {
		return 1;
	};
	s->interventions_stack = create_cll(sizeof(struct intervention*));
	if (!s->interventions_stack) {
		return 1;
	};
	*(intervention**)s->interventions_stack->head->data = NULL;
	read_data(&sys, &q, &s, in, out);
	//freeeees
	free(sys);
	free(q);
	cll_free(&s->interventions_stack);
	free(s);
	fclose(in);
	fclose(out);
	return 0;
}
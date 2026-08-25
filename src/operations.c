#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "circular_list.h"
#include "operations.h"

//function that dispatches an unit to an incident
//and returns a pointer to the intervention added
void dispatch(struct system **sys, struct all_queues **q,
				   struct stack **s, FILE *out) {
	if (!(*q)->available_units->size) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	};
	intervention *new_intervention = malloc(sizeof(struct intervention));
	if (!new_intervention) {
		return;
	};

	//find the highest priority incident
	if ((*q)->high->size) {
		new_intervention->incident =
		(incident*)((cll_node*)dequeue((*q)->high))->data;
	} else if ((*q)->medium->size) {
		new_intervention->incident =
		(incident*)((cll_node*)dequeue((*q)->medium))->data;
	} else if ((*q)->low->size) {
		new_intervention->incident =
		(incident*)((cll_node*)dequeue((*q)->low))->data;
	} else {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		free(new_intervention);
		return;
	};

	//dequeue available unit and assign the intervention
	//then push the intervention onto the stack
	new_intervention->unit =
	(unit*)((cll_node*)dequeue((*q)->available_units))->data;
	new_intervention->unit->availability = 0;
	strcpy(new_intervention->incident->status, "intervened");
	cll_node *new_node = cll_add_node((*sys)->interventions, new_intervention);
	intervention *get_adress = ((intervention*)new_node->data);
	new_node = cll_add_node((*s)->interventions_stack, &get_adress);
	free(new_intervention);
}

//function that finds the node in the linked list associated
//with a given incident by matching ids
cll_node *turn_incident_into_node(struct incident *incid, struct system **sys)
{
	//next to avoid the santinel?
	cll_node *curr = (*sys)->incidents->head->next;
	for (unsigned int i = 1; i <= (*sys)->incidents->size; i++) {
		if(((incident*)curr->data)->id == incid->id) {
			break;
		};
		curr = curr->next;
	};
	if (((incident*)curr->data)->id != incid->id) {
		return NULL;
	}
	return curr;
}

//function that finds the node in the linked list associated
//with a given unit by matching ids
cll_node *turn_unit_into_node(struct unit *uni, struct system **sys) {
	//head->next is the first node to avoid the santinel
	cll_node *curr = (*sys)->units->head->next;
	for (unsigned int i = 1; i <= (*sys)->units->size; i++) {
		if(((unit*)curr->data)->id == uni->id) {
			break;
		};
		curr = curr->next;
	};
	if(((unit*)curr->data)->id != uni->id) {
		return NULL;
	};
	return curr;
}

//function that undoes the last dispatch
void undo_dispatch(struct system **sys, struct all_queues **q,
				   struct stack **s, FILE *out) {
	if(!(*s)->interventions_stack->size) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	};

	//the stack has a santinel
	//find the last intervention not solved in the stack and remove it
	cll_node *curr = (*s)->interventions_stack->head->prev;
	unsigned int position = 1;
	while (strncmp((*(intervention**)curr->data)->incident->status, "int", 3)
		   && position <= (*s)->interventions_stack->size) {
		curr = curr->prev;
		position++;
	};
	if (strncmp((*(intervention**)curr->data)->incident->status, "int", 3)) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	};
	position = (*s)->interventions_stack->size - position;
	cll_node *removed = cll_remove_nth_node((*s)->interventions_stack,
											position);

	curr = (*sys)->interventions->head->next;
	//find the intervention in the list as well and remove it
	position = 0;
	for (unsigned int i = 1; i <= (*sys)->interventions->size; i++) {
		if((intervention*)curr->data == *(intervention**)removed->data) {
			break;
		};
		curr = curr->next;
		position++;
	};
	cll_node *removed2 = cll_remove_nth_node((*sys)->interventions, position);
	incident *changed_incident = (*(intervention**)removed->data)->incident;

	//put the incident back in its queue, but to the start
	strcpy((*(intervention**)removed->data)->incident->status, "queued");
	if (!strcmp(changed_incident->priority, "high")) {
		enqueue_to_start((*q)->high,
						 turn_incident_into_node(changed_incident, sys));
	} else if (!strcmp(changed_incident->priority, "medium")) {
		enqueue_to_start((*q)->medium,
						 turn_incident_into_node(changed_incident, sys));
	} else if (!strcmp(changed_incident->priority, "low")) {
		enqueue_to_start((*q)->low,
						 turn_incident_into_node(changed_incident, sys));
	};

	//put the unit back in the available queue
	unit *changed_unit = (*(intervention**)removed->data)->unit;
	changed_unit->availability = 1;
	enqueue((*q)->available_units, turn_unit_into_node(changed_unit, sys));
	free(removed->data);
	free(removed2->data);
	free(removed);
	free(removed2);
}
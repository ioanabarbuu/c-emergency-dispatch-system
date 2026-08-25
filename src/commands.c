#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entities.h"
#include "queue.h"
#include "circular_list.h"
#include "operations.h"
#include "commands.h"

//function that interprets a command
void command_interpretor(char *command, struct system **sys,
						 struct all_queues **q, struct stack **s, FILE *out)
{	
	char *word = strtok(command, " \n");
	if (!strcmp(word, "ADD_INCIDENT")) {
		//parse the command
		word = strtok(NULL, " ");
		unsigned int id = atoi(word);
		word = strtok(NULL, " ");
		char priority[7];
		strcpy(priority, word);
		word = strtok(NULL, "\"\n");

		incident *new_incident = create_incident(id, priority, word, "queued");
		cll_node *new_node = cll_add_node((*sys)->incidents, new_incident);
		if (!strcmp(priority, "high")) {
			enqueue((*q)->high, new_node);
		} else if (!strcmp(priority, "medium")) {
			enqueue((*q)->medium, new_node);
		} else if (!strcmp(priority, "low")) {
			enqueue((*q)->low, new_node);
		};
		free(new_incident->description);
		free(new_incident);

	} else if (!strncmp(word, "CHECK", 5)) {
		int result = (*q)->available_units->size;
		fprintf(out, "Number of available units: %d\n", result);
	} else if (!strncmp(word, "DISPAT", 6)) {
		dispatch(sys, q, s, out);
	} else if (!strncmp(word, "UNDO", 4)) {
		undo_dispatch(sys, q, s, out);
	} else if (!strcmp(word, "SOLVED_INCIDENT")) {
		word = strtok(NULL, " "); 
		int id = atoi(word);
		cll_node *curr = (*sys)->incidents->head->next;
		//find the node in the linked list of incidents
		for (unsigned int i = 1; i <= (*sys)->incidents->size; i++) {
			if(((incident*)curr->data)->id == id) {
				break;
			};
			curr = curr->next;
		};

		if (((incident*)curr->data)->id != id ||
			strcmp(((incident*)curr->data)->status, "intervened")) {
			fprintf(out, "INVALID OPERATION! ERROR 404\n");
		} else {
			strcpy(((incident*)curr->data)->status, "solved");
			curr = (*sys)->interventions->head->next;
			//find the node in the linked list of interventions
			for (unsigned int i = 1; i <= (*sys)->interventions->size; i++) {
				if(((intervention*)curr->data)->incident->id == id) {
					break;
				};
				curr = curr->next;
			};
			((intervention*)curr->data)->unit->availability = 1;
			enqueue((*q)->available_units,
					turn_unit_into_node(((intervention*)curr->data)->unit,
										sys));
		};
	} else if (!strcmp(word,  "SHOW_UNIT")) {
		word = strtok(NULL, " ");
		int id = atoi(word);
		//find the node in the linked list of units
		cll_node *curr = (*sys)->units->head->next;
		for (unsigned int i = 1; i <= (*sys)->units->size; i++) {
			if(((unit*)curr->data)->id == id) {
				break;
			};
			curr = curr->next;
		};

		if (((unit*)curr->data)->id != id) {
			fprintf(out, "INVALID OPERATION! ERROR 404\n");
		} else if (((unit*)curr->data)->availability) {
			fprintf(out, "Unit %d is type %c and is available\n", 
					id, ((unit*)curr->data)->type);
		} else {
			fprintf(out, "Unit %d is type %c and is unavailable\n",
					id, ((unit*)curr->data)->type);
		};

	} else if (!strcmp(word, "SHOW_INCIDENT")) {
		word = strtok(NULL, " ");
		int id = atoi(word);
		//find the node in the linked list of units
		cll_node *curr = (*sys)->incidents->head->next;
		for (unsigned int i = 1; i <= (*sys)->incidents->size; i++) {
			if(((incident*)curr->data)->id == id) {
				break;
			};
			curr = curr->next;
		};

		if (((incident*)curr->data)->id != id) {
			fprintf(out, "INVALID OPERATION! ERROR 404\n");
		} else {
			fprintf(out, "Incident %d has %s priority, the following "
					"description: \"%s\" and is %s\n",
					id, ((incident*)curr->data)->priority,
					((incident*)curr->data)->description,
					((incident*)curr->data)->status);
		};

	} else if (!strncmp(word, "SHOW_INTER", 10)) {
		cll_node *curr = (*sys)->interventions->head->next;
		if (!(*sys)->interventions->size) {
			fprintf(out, "No intervention has been initiated\n");
		} else {
			//show all interventions data
			for (unsigned int i = 1; i <= (*sys)->interventions->size; i++) {
				intervention *interv = (intervention*)curr->data;
				fprintf(out, "Incident %d was assigned to unit %d, and has "
						"the following status: \"%s\"\n",
					    ((incident *)interv->incident)->id,
					    ((unit *)interv->unit)->id,
					    ((incident *)interv->incident)->status);
				curr = curr->next;
			};
		};
	};
}
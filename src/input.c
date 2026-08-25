#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entities.h"
#include "queue.h"
#include "circular_list.h"
#include "commands.h"
#include "input.h"

//function that reads data from the input file
void read_data(struct system **sys, struct all_queues **q,
			   struct stack **s, FILE *in, FILE *out)
{
	int units_count, unit_id, comm_count;
	char unit_type;
	fscanf(in, "%d ", &units_count);
	(*sys)->units = create_cll(sizeof(struct unit));

	//initialize the santinel of unit list
	((unit*)(*sys)->units->head->data)->id = 0;
	((unit*)(*sys)->units->head->data)->availability = 0;
	((unit*)(*sys)->units->head->data)->type = 'a';
	(*sys)->incidents = create_cll(sizeof(struct incident));

	//initialize the santinel of incident list
	strcpy(((incident*)(*sys)->incidents->head->data)->status, "solved");
	strcpy(((incident*)(*sys)->incidents->head->data)->priority, "low");
	((incident*)(*sys)->incidents->head->data)->description = malloc(14);
	strcpy(((incident*)(*sys)->incidents->head->data)->description,
		   "test incident");
	((incident*)(*sys)->incidents->head->data)->id = 0;
	(*sys)->interventions = create_cll(sizeof(struct intervention));

	//initialize the santinel of interventions list
	((intervention*)(*sys)->interventions->head->data)->incident = NULL;
	((intervention*)(*sys)->interventions->head->data)->unit = NULL;
	(*q)->available_units = create_queue(units_count);

	for (int i = 1; i <= units_count; i++) {
		fscanf(in, "%d %c", &unit_id, &unit_type);
		unit *new_unit = create_unit(unit_id, unit_type, 1);
		cll_node *new_node = cll_add_node((*sys)->units, new_unit);
		enqueue((*q)->available_units, new_node);
		free(new_unit);
	};

	fscanf(in, "%d ", &comm_count);
	(*q)->high = create_queue(comm_count);
	(*q)->medium = create_queue(comm_count);
	(*q)->low = create_queue(comm_count);

	//read the commands dynamically
	for (int i = 1; i <= comm_count; i++) {
		char *command = malloc(2);
		if (!command) {
			return;
		};
		command[0] = '\0';
		char curr_char = 'a';
		//i introduced a dummy char,
		// read until an endline is read
		while(curr_char != '\n' && fscanf(in, "%c", &curr_char) == 1) {
			char *aux_command = realloc(command, strlen(command) + 2);
			if (!aux_command) {
				return;
			};
			command = aux_command;
			int comm_length = strlen(command);
			command[comm_length] = curr_char;
			command[comm_length + 1] = '\0';
		};
		command_interpretor(command, sys, q, s, out);
		free(command);
	};
	cll_free(&(*sys)->units);
	cll_free(&(*sys)->incidents);
	cll_free(&(*sys)->interventions);
	queue_free((*q)->available_units);
	queue_free((*q)->high);
	queue_free((*q)->medium);
	queue_free((*q)->low);

}
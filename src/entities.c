#include <stdlib.h>
#include <string.h>
#include "entities.h"

//function that creates a pointer to an incident
incident *create_incident(int id, char *priority, char *description,
						  char *status)
{
	incident *new_incident = malloc(sizeof(struct incident));
	if(!new_incident) {
		return NULL;
	};
	new_incident->id = id;
	strcpy(new_incident->priority, priority);
	new_incident->description = malloc(strlen(description) + 1);
	if(!new_incident->description) {
		free(new_incident);
		return NULL;
	};
	strcpy(new_incident->description, description);
	strcpy(new_incident->status, status);
	return new_incident;
}

//function that creates a pointer to an unit
unit *create_unit(int id, char type, int availability)
{
	unit *new_unit = malloc(sizeof(struct unit));
	if(!new_unit) {
		return NULL;
	};
	new_unit->id = id;
	new_unit->type = type;
	new_unit->availability = availability;
	return new_unit;
}
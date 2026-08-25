#ifndef ENTITIES_H
#define ENTITIES_H

#include "types.h"

incident *create_incident(int id, char *priority, char *description,
						  char *status);
unit *create_unit(int id, char type, int availability);

#endif
#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include "types.h"

void command_interpretor(char *command, struct system **sys,
						 struct all_queues **q, struct stack **s, FILE *out);

#endif
# Emergency Dispatch System

This project implements a simplified emergency dispatch system in C that
manages units, incidents and interventions. It uses custom data structures to
prioritize incidents, assign available units, track intervention history and
support undo operations.

## 1. Reading Commands

> To read commands, I opted for an implementation that reads from the input
file each character and builds the current command string via reallocations
each time a new character (that is not and endline) is read(the `read_data`
function). Then, the command is passed to the `command_interpretor` function
for parsing and execution. I chose this approach for memory efficiency as no
maximum command length was specified in the problem text and the fact that the
description could have various lengths was especially problematic. The function
`command_interpretor` separates a command by using strtok with an empty space
as the separator, but to separate the description the use of a different
separating char was needed (the ").

## 2. Data Structures Used

> The system is built with three main data structures, as the task suggested:
a circular doubly linked list with a santinel node, a queue implemented with
a circular buffer, and a stack implemented as a circular linked list.
>
> The circular linked list is built with a santinel, and the nodes can store
any type of data (I used pointers to void). When computing the size of
the list, the santinel is not counted. The circular linked list is used to
store the units, the incidents, as well as the interventions. I chose this
for a more uniform approach of storing data. A challenge faced during its
implementation was the fact that an incident structure needs to store a
dynamically allocated string (the description), so my initial solution of just
using a memcopy to the new data(a unit type, an incident type, or an
intervention type) was problematic when trying to free the incident. This is
why I chose to do a deep copy function for incidents that is called when adding
an incident to the list (`deep_copy_incident`).
>
> The queue is implemented as a circular array of pointers to void, with
separate read and write indices that wrap around when they reach the end of
the buffer. Four queues are used and are stored in an all_queues structure,
again, for a more uniform code: one for high-priority incidents, one for
medium-priority incidents, one for low-priority incidents, and one for
available units. Other than the classic queue functions from the laboratory,
an additional `enqueue_to_start` function is used to insert incidents at the
front of their queue when an UNDO_LAST_DISPATCH is performed.
>
> The stack uses a circular linked list used to record the history of
interventions in order and is needed for implementing the undo. Each node in
the stack stores a pointer to the corresponding intervention.

## 4. Functionality

> The `dispatch` function first checks that both an available unit and at
least one queued incident exist before proceeding, printing the error message.
This ordering is important to avoid dequeueing a unit from the queue before
confirming an incident is available (an issue I had in a prior version of
the code). The highest-priority incident is selected by checking the high,
medium, and low queues in order. A new intervention struct is allocated with
pointers to the unit's and incident's data inside the lists, and then copied
into the linked list of interventions. A pointer to the intervention's copy
inside the list is then pushed onto the stack of interventions.
>
> The `undo_dispatch` function searches the stack from the top looking for
the last intervention whose associated incident still has status "intervened",
skipping over any that have already been solved. Once found, the intervention
node is removed from the stack and from the linked list, the incident has its
status reset to "queued" and is inserted at the front of its priority queue
through `enqueue_to_start`, the unit's availability is changed  and it is
enqueued into`available_units`. 
>
> The SOLVED_INCIDENT command finds the incident by ID in the incidents list
and marks its status "solved". It then finds the corresponding intervention
in the list to retrieve the unit involved, change the unit's availability and
enqueues the unit node into the `available_units` queue.

## Concepts Used

- dynamic memory allocation
- pointers and `void *`
- structures
- circular doubly linked lists
- santinel nodes
- circular queues
- stacks
- priority-based processing
- deep and shallow copying
- string parsing with `strtok`
- file input and output
- command parsing
- undo operations
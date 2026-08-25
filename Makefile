CC = gcc

CFLAGS = -Wall -Wextra -std=c99 -Iinclude

TARGETS = emergency_dispatch

build: $(TARGETS)

emergency_dispatch: src/main.c src/queue.c src/entities.c src/circular_list.c src/operations.c src/commands.c src/input.c
	$(CC) $(CFLAGS) src/main.c src/queue.c src/entities.c src/circular_list.c src/operations.c src/commands.c src/input.c -o emergency_dispatch

run: build
	./emergency_dispatch

clean:
	rm -f $(TARGETS)

.PHONY: build run clean
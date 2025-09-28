# Makefile for compiling multiple programs

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Executables
PROG1 = prog1
PROG2 = prog2
PROG3 = my3proc

all: $(PROG1) $(PROG2) $(PROG3)

$(PROG1): main.c
	$(CC) $(CFLAGS) -o $(PROG1) main.c

$(PROG2): main2.c
	$(CC) $(CFLAGS) -o $(PROG2) main2.c

$(PROG3): main3.c
	$(CC) $(CFLAGS) -o $(PROG3) main3.c

# Handy shortcut: build & run my3proc
run: $(PROG3)
	./$(PROG3)

# Clean up all executables
clean:
	rm -f $(PROG1) $(PROG2) $(PROG3)

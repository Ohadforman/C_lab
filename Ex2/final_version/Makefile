CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = my_grep.o print_control.o search_logic.o args_parser.o
HEADERS = print_control.h search_logic.h args_parser.h

my_grep: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o my_grep

my_grep.o: my_grep.c $(HEADERS)
	$(CC) $(CFLAGS) -c my_grep.c

print_control.o: print_control.c print_control.h
	$(CC) $(CFLAGS) -c print_control.c

search_logic.o: search_logic.c search_logic.h
	$(CC) $(CFLAGS) -c search_logic.c

args_parser.o: args_parser.c args_parser.h
	$(CC) $(CFLAGS) -c args_parser.c

.PHONY: clean all test

clean:
	@-rm -f *.o my_grep

all: my_grep

test:
	/specific/a/home/cc/students/csguests/nimrodav/grep_tests/run_all.sh

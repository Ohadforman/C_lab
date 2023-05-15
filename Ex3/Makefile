CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = my_grep.o print_control.o search_logic.o args_parser.o
HEADERS = lb_back_end.h 

my_grep: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ex3_lb

my_grep.o: my_grep.c $(HEADERS)
	$(CC) $(CFLAGS) -c ex3_lb.c

lb_back_end.o: lb_back_end.c lb_back_end.h
	$(CC) $(CFLAGS) -c lb_back_end.c



.PHONY: clean all test

clean:
	@-rm -f *.o ex3_lb

all: ex3_lb

test:
	/specific/a/home/cc/students/csguests/nimrodav/socket_ex/test.sh

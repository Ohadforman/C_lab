CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = ex3_lb.o lb_back_end.o  
HEADERS = lb_back_end.h 

ex3_lb: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ex3_lb

ex3_lb.o: ex3_lb.c $(HEADERS)
	$(CC) $(CFLAGS) -c ex3_lb.c

lb_back_end.o: lb_back_end.c lb_back_end.h
	$(CC) $(CFLAGS) -c lb_back_end.c



.PHONY: clean all test

clean:
	@-rm -f *.o ex3_lb

all: ex3_lb

test:
	/specific/a/home/cc/students/csguests/nimrodav/socket_ex/test.sh

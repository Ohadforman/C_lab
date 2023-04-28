/* nfa.h - Header file for NFA implementation */

#ifndef NFA_H
#define NFA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int state;

typedef struct transition {
    int symbol;
    state to;
    struct transition *next;
} transition;

typedef struct nfa {
    int num_states;
    transition **transitions;
    state start;
    state end;
} nfa;

nfa *regex_to_nfa_new(char *regex);
int nfa_is_substring(nfa *machine, char *input);

#endif /* NFA_H */

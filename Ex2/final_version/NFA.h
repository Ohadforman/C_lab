#ifndef NFA_H
#define NFA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int **transitions; // Transition table
    int num_states; // Number of states
    int start_state; // Starting state
    int *accept_states; // Accepting states
    int num_accept_states; // Number of accepting states
} NFA;

void nfa_init(NFA *nfa);
int nfa_add_state(NFA *nfa);
void nfa_set_start_state(NFA *nfa, int state);
void nfa_add_accept_state(NFA *nfa, int state);
void nfa_add_transition(NFA *nfa, int from_state, int to_state, char input_symbol);
void free_nfa(NFA *nfa);
bool nfa_accepts(NFA *nfa, char *input);
bool nfa_accepts_line(NFA *nfa, char *line, bool substrings);

#endif

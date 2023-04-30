#include "nfa.h"

// Initialize an empty NFA with no states or transitions
void nfa_init(NFA *nfa) {
    nfa->num_states = 0;
    nfa->start_state = -1;
    nfa->num_accept_states = 0;
    nfa->transitions = NULL;
    nfa->accept_states = NULL;
}

// Add a new state to the NFA and return its ID
int nfa_add_state(NFA *nfa) {
    int state = nfa->num_states;
    nfa->num_states++;
    nfa->transitions = realloc(nfa->transitions, sizeof(int*) * nfa->num_states);
    nfa->transitions[state] = calloc(256, sizeof(int));
    return state;
}

// Set the starting state of the NFA
void nfa_set_start_state(NFA *nfa, int state) {
    nfa->start_state = state;
}

// Add an accepting state to the NFA
void nfa_add_accept_state(NFA *nfa, int state) {
    nfa->accept_states = realloc(nfa->accept_states, sizeof(int) * (nfa->num_accept_states + 1));
    nfa->accept_states[nfa->num_accept_states] = state;
    nfa->num_accept_states++;
}

// Add a transition from one state to another on a given input symbol
void nfa_add_transition(NFA *nfa, int from_state, int to_state, char input_symbol) {
    nfa->transitions[from_state][(int)input_symbol] = to_state;
}

//free the memory allocated for the NFA
void free_nfa(NFA *nfa) {
    // Free transitions
    if (nfa->transitions != NULL) {
        for (int i = 0; i < nfa->num_states; i++) {
            free(nfa->transitions[i]);
        }
        free(nfa->transitions);
        nfa->transitions = NULL;
    }

    // Free accept_states
    if (nfa->accept_states != NULL) {
        free(nfa->accept_states);
        nfa->accept_states = NULL;
    }

    // Reset other fields
    nfa->num_states = 0;
    nfa->start_state = -1;
    nfa->num_accept_states = 0;
}

// Test if the NFA accepts a given input string
bool nfa_accepts(NFA *nfa, char *input) {
    int current_states[nfa->num_states];
    int num_current_states = 1;
    current_states[0] = nfa->start_state;

    // For each input symbol, compute the set of reachable states
    for (int i = 0; input[i] != '\0'; i++) {
        int next_states[nfa->num_states];
        int num_next_states = 0;

        // Compute the set of states reachable from the current states on the current input symbol
        for (int j = 0; j < num_current_states; j++) {
            int state = current_states[j];
            int next_state = nfa->transitions[state][(int)input[i]];
            if (next_state != 0) {
                next_states[num_next_states] = next_state;
                num_next_states++;
            }
        }

        // Set the current states to be the set of reachable states
        num_current_states = num_next_states;
        for (int j = 0; j < num_current_states; j++) {
            current_states[j] = next_states[j];
        }
    }

    // Check if any of the current states are accepting states
    for (int i = 0; i < num_current_states; i++) {
        int state = current_states[i];
        for (int j = 0; j < nfa->num_accept_states; j++) {
            if (state == nfa->accept_states[j]) {
                return true;
            }
        }
    }

    return false;
}

// Test if the NFA accepts the entire line or a substring of the line
bool nfa_accepts_line(NFA *nfa, char *line, bool substrings) {
    int line_len = strlen(line);

    // Check if the entire line is accepted by the NFA
    if (nfa_accepts(nfa, line)) {
        return true;
    }

    // Check if any substring of the line is accepted by the NFA
    if (substrings) {
        for (int i = 0; i < line_len; i++) {
            for (int j = i + 1; j <= line_len; j++) {
                char substring[j-i+1];
                strncpy(substring, line+i, j-i);
                substring[j-i] = '\0';
                if (nfa_accepts(nfa, substring)) {
                    return true;
                }
            }
        }
    }

    // If neither the entire line nor any substring is accepted, return false
    return false;
}


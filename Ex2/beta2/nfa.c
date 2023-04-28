#include "nfa.h"

/* Create a NFA from regex*/




/* Create a new state in the NFA */
state state_new(nfa *nfa) {
    state s = nfa->num_states++;
    nfa->transitions[s] = NULL;
    return s;
}

/* Create a transition between two states in the NFA */
void nfa_transition(nfa *nfa, state from, state to, int symbol) {
    transition *t = malloc(sizeof(transition));
    t->symbol = symbol;
    t->to = to;
    t->next = nfa->transitions[from];
    nfa->transitions[from] = t;
}

/* Create a new state and add a transition to it from another state in the NFA */
state nfa_state(nfa *nfa) {
    state s = state_new(nfa);
    nfa_transition(nfa, s, state_new(nfa), -1);
    return s;
}

/* Check if a character is a metacharacter that needs to be escaped */
int is_metacharacter(char c) {
    return c == '(' || c == ')' || c == '|' || 
     c == '[' || c == ']' || c == '\\' || 
     c == '{' || c== '}'|| c== '-'; 
}

/* Recursively build an NFA for a regular expression using Thompson's algorithm */
state regex_to_nfa(nfa *nfa, char *regex, int *i) {
    state start = nfa_state(nfa);
    state end = start;
    while (regex[*i] != '\0' && regex[*i] != ')') {
        if (regex[*i] == '(') {
            (*i)++;
            state s = regex_to_nfa(nfa, regex, i);
            nfa_transition(nfa, end, s, -1);
            end = s;
        } else if (regex[*i] == '|') {
            (*i)++;
            state s = regex_to_nfa(nfa, regex, i);
            nfa_transition(nfa, start, s, -1);
            nfa_transition(nfa, end, s, -1);
            end = s;
        } else if (regex[*i] == '*') {
            (*i)++;
            nfa_transition(nfa, end, start, -1);
        } else if (regex[*i] == '+') {
            (*i)++;
            state s = regex_to_nfa(nfa, regex, i);
            nfa_transition(nfa, end, s, -1);
            nfa_transition(nfa, s, start, -1);
            end = s;
        } else if (regex[*i] == '?') {
            (*i)++;
            nfa_transition(nfa, end, start, -1);
            end = start;
        } else if (regex[*i] == '[') {
            (*i)++;
            int negate = 0;
            if (regex[*i] == '^') {
                negate = 1;
                (*i)++;
            }
            int symbol = regex[*i];
            (*i)++;
            if (regex[*i] == '-') {
                (*i)++;
                symbol = regex[*i];
                (*i)++;
            }
            while (regex[*i] != '\0' && regex[*i] != ']') {
                if (regex[*i] == '-') {
                    (*i)++;
                    symbol = regex[*i];
                    (*i)++;
                }
                nfa_transition(nfa, end, state_new(nfa), symbol);
                end = nfa->num_states - 1;
                symbol = regex[*i];
                (*i)++;
            }
            if (negate) {
                for (int i = 0; i < 256; i++)
                    if (i != symbol)
                        nfa_transition(nfa, end, state_new(nfa), i);
            }
            (*i)++;
        } else if (regex[*i] == '\\') {
            (*i)++;
            int symbol = regex[*i];
            (*i)++;
            nfa_transition(nfa, end, state_new(nfa), symbol);
            end = nfa->num_states - 1;
        } else {
            int symbol = regex[*i];
            (*i)++;
            if (is_metacharacter(regex[*i])) {
                nfa_transition(nfa, end, state_new(nfa), symbol);
                end = nfa->num_states - 1;
            } else {
                nfa_transition(nfa, end, state_new(nfa), symbol);
                end = nfa->num_states - 1;
            }
        }
    }
    return start;
}

/* Create an NFA for a regular expression */
nfa *regex_to_nfa_new(char *regex) {
    nfa *nfa = malloc(sizeof(nfa));
    nfa->num_states = 0;
    nfa->transitions = malloc(sizeof(transition *) * 1000);
    nfa->start = regex_to_nfa(nfa, regex, &(int){0});
    nfa->end = nfa->num_states - 1;
    return nfa;
}






/* Use NFA to match a string */

int nfa_is_substring(nfa *machine, char *input) {
    int i, j, k;
    int *current_states = malloc(sizeof(int) * 1000);
    current_states[0] = machine->start;
    int *next_states = malloc(sizeof(int) * 1000);
    int num_current_states = 1;
    int num_next_states = 0;
    int input_len = strlen(input);

    /* Perform the matching process */
    for (i = 0; i < input_len; i++) {

        /* Compute the next set of states */
        for (j = 0; j < num_current_states; j++) {
            state s = current_states[j];
            transition *t = machine->transitions[s];
            while (t != NULL) {
                if (t->symbol == -1 || t->symbol == input[i]) {
                    int already_in_list = 0;
                    for (k = 0; k < num_next_states; k++) {
                        if (next_states[k] == t->to) {
                            already_in_list = 1;
                            break;
                        }
                    }
                    if (!already_in_list) {
                        next_states[num_next_states++] = t->to;
                    }
                }
                t = t->next;
            }
        }

        /* Check if we have reached the end state */
        for (j = 0; j < num_next_states; j++) {
            if (next_states[j] == machine->end) {
                free(current_states);
                free(next_states);
                return 1;
            }
        }

        /* Stop if there are no more states to explore */
        if (num_next_states == 0) {
            break;
        }

        /* Swap the current and next states */
        int *temp = current_states;
        current_states = next_states;
        next_states = temp;
        num_current_states = num_next_states;
        num_next_states = 0;
    }

    free(current_states);
    free(next_states);
    return 0;
}

int main() {
    char *regex = "[a-z]";
    char *input = "a.pdf";

    nfa *machine = regex_to_nfa_new(regex);

    if (nfa_is_substring(machine, input)) {
        printf("Input matches the regular expression\n");
    } else {
        printf("Input does not match the regular expression\n");
    }

    return 0;
}

#NFA from regex
1. Define a data structure to represent the NFA. You can use a struct to represent each state of the NFA, where each state has a set of transitions to other states.

2. Define a function to create a new state in the NFA. This function should return the ID of the new state and add it to the list of states in the NFA.

3. Define a function to add a transition between two states in the NFA. This function should take the IDs of the two states and the symbol that triggers the transition.

4. Define a function to convert a regular expression into a postfix notation. This function should take the regular expression as input and return a list of symbols in postfix notation.

5. Define a function to create an NFA for a given symbol. This function should create two new states in the NFA and add a transition between them using the symbol.

6. Define a function to create an NFA for a given Kleene star (*). This function should take the NFA for the subexpression as input and add two new states to the NFA. One state should have an epsilon transition to the start state of the subexpression, and the end state of the subexpression should have an epsilon transition to the other new state. Finally, the new start state should have an epsilon transition to the new end state.

7. Define a function to create an NFA for a given concatenation (ab). This function should take the NFAs for the subexpressions as input and connect the end state of the first NFA to the start state of the second NFA using an epsilon transition.

8. Define a function to create an NFA for a given alternation (a|b). This function should take the NFAs for the subexpressions as input and add two new states to the NFA. One state should have epsilon transitions to the start states of both sub-NFAs, and the end states of both sub-NFAs should have epsilon transitions to the other new state.

9. Define a function to parse the postfix notation and build the NFA. This function should use a stack to keep track of the NFAs for each symbol in the postfix notation. When a symbol is encountered, the corresponding NFA is pushed onto the stack. When a Kleene star or concatenation is encountered, the appropriate NFA functions are called and the resulting NFA is pushed onto the stack. When an alternation is encountered, the appropriate NFA function is called with the two top elements of the stack as input, and the resulting NFA is pushed onto the stack.

10. Define a function to clean up the NFA. This function should remove any states that are not reachable from the start state, and any epsilon transitions.

# Search from NFA
1. If the flag is set to match the entire expression, start at the initial state of the NFA and traverse it by consuming characters from the input string one by one until you reach an accepting state. If you reach an accepting state, then the entire expression has been matched.

2. If the flag is set to match a substring, you will need to modify the NFA to allow partial matching. One way to do this is to add an epsilon transition from every state to the initial state, effectively allowing the NFA to start matching at any point in the input string. Then, start at each position in the input string and traverse the modified NFA as before. If you reach an accepting state, then a match has been found.

3. If you need to return the matched substring(s), you can keep track of the starting and ending positions of each match found during the traversal and return the corresponding substrings.

4. If you need to handle capturing groups in the regular expression, you can modify the NFA to keep track of the positions of the start and end of each captured group during the traversal.
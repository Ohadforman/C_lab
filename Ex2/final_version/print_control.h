#ifndef PRINT_CONTROL_H
#define PRINT_CONTROL_H
#include "args_parser.h"
#include "search_logic.h"

// Print a line from the file according to the specified print and line commands
void print_line(LineInfo* line, grep_args* args);

// Prints the "--" separator line
void print_seperator_line(LineInfo** lines, int lines_num, int i);

void free_line(LineInfo* line);

#endif /* PRINT_CONTROL_H */

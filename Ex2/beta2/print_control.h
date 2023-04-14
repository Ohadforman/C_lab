#ifndef PRINTCONTROL_H
#define PRINTCONTROL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Print a line from the file according to the specified print and line commands
void print_line(FILE* file, char* line, char* print_command, char* line_command);

// Print the specified line and num_lines after it, according to the specified print and line commands
void print_num_lines(FILE* file, char* line, int num_lines, char* print_command, char* line_command);

#endif /* PRINTCONTROL_H */

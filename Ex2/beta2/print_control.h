#ifndef PRINTCONTROL_H
#define PRINTCONTROL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
    char* line_ptr;
    int line_num;
    int bytes_until_line;
} LineInfo;

// Print a line from the file according to the specified print and line commands
void print_line(char* line,int line_number,int bytes_number, char* print_command, char* byte_command);

// Print the specified line and num_lines after it, according to the specified print and line commands
void print_num_lines(FILE* file, LineInfo info, int num_lines, char* print_command, char* byte_command);

#endif /* PRINTCONTROL_H */

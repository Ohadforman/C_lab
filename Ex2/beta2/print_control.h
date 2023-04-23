#ifndef PRINTCONTROL_H
#define PRINTCONTROL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"

typedef enum print_type {
    ONLY_LINE_NUM,
    ONLY_LINE,
    NUM_DASH_LINE,
    NUM_COLON_LINE,
} print_type;

// Print a line from the file according to the specified print and line commands
void print_line(LineInfo* line, print_type print_type, int print_bytes_until_line);

// Print the specified line and num_lines after it, according to the specified print and line commands
void print_num_lines(FILE* file, LineInfo* info, int num_lines, print_type print_type, int print_bytes_until_line);

#endif /* PRINTCONTROL_H */

#ifndef FILEACTIONS_H
#define FILEACTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Get the number of bytes in the file until the specified line
int get_bytes_until_line(FILE* file, char* line);

// Get the line number of a specified line in the file
int get_line_number(FILE* file, char* line);

// Print a line from the file according to the specified print and line commands
void print_line(FILE* file, char* line, char* print_command, char* line_command);

// Print the specified line and num_lines after it, according to the specified print and line commands
void print_num_lines(FILE* file, char* line, int num_lines, char* print_command, char* line_command);

#endif /* FILEACTIONS_H */

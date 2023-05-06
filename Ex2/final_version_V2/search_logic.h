#ifndef SEARCH_LOGIC_H
#define SEARCH_LOGIC_H

#include "arg2command.h"
// Struct to hold parsed command line arguments


// Struct to hold line info
typedef struct LineInfo {
    char* line_ptr;
    int line_num;
    int bytes_until_line;
    char seperator;
} LineInfo;

// This function reads a file line by line, and applies a search command to each line to determine if it matches
// a given expression. If a line matches, a tuple containing the line number, byte offset, and a pointer to the
// line string is added to an array. 
// The search command can be either "exact match", "with expression", or "without expression",
// and can be case-sensitive or case-insensitive.
int control_get_lines(grep_args* args, LineInfo*** results);

#endif /* SEARCH_LOGIC_H */

#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Struct to hold parsed command line arguments
typedef struct grep_args {
    char* pattern;
    char* file_name;
    int c_flag;
    int n_flag;
    int i_flag;
    int v_flag;
    int x_flag;
    int e_flag;
    int b_flag;
    int a_flag;
    int a_num;
} grep_args;

// Struct to hold line info
typedef struct LineInfo {
    char* line_ptr;
    int line_num;
    int bytes_until_line;
} LineInfo;

// Function to parse command line arguments and populate grep_args struct
void parse_args(int argc, char* argv[], grep_args* args);

// This function reads a file line by line, and applies a search command to each line to determine if it matches
// a given expression. If a line matches, a tuple containing the line number, byte offset, and a pointer to the
// line string is added to an array. 
// The search command can be either "exact match", "with expression", or "without expression",
// and can be case-sensitive or case-insensitive.

int control_get_lines(grep_args* args, LineInfo** results);
// This function searches for a pattern within a given string using a regular expression-like syntax
// and returns an array of two integers: the first indicates if an exact match was found, and the second
// indicates if the pattern was found at all. It uses a stack-based approach to handle grouping and alternation.

int* search_pattern(const char* line, const char* pattern, int case_sensitive);

#endif /* PARSE_H */

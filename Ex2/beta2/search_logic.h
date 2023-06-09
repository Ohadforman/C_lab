#ifndef SEARCH_LOGIC_H
#define SEARCH_LOGIC_H

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
    char seperator;
} LineInfo;

// This function reads a file line by line, and applies a search command to each line to determine if it matches
// a given expression. If a line matches, a tuple containing the line number, byte offset, and a pointer to the
// line string is added to an array. 
// The search command can be either "exact match", "with expression", or "without expression",
// and can be case-sensitive or case-insensitive.
int control_get_lines(grep_args* args, LineInfo*** results);

/**
 * @brief Searches for the pattern in the line
 * 
 * @param line 
 * @param pattern 
 * @param case_sensitive 
 * @param is_regex 
 * @return int[2] = {match_found, is_exact_match}
 */
int* search_pattern(char* line, char* pattern, int case_sensitive, int is_regex);

#endif /* SEARCH_LOGIC_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"

/* Read the lines into an array */
int control_get_lines(grep_args* args, LineInfo*** results) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;
    size_t total_read = 0;
    int line_count = 1;
    int result_count = 0;
    int* search_result;
    int match_flag, match_flag1, match_flag2, match_flag3;
    int found_match_in_line;
    int line_equal_pattern;
    int num_rows_after = 0;

    // Input flags
    int case_sensitive = !args->i_flag;
    int only_row_num = args->c_flag;
    int rows_not_contain_pattern = args->v_flag;
    int rows_only_contain_pattern = args->x_flag;

    // Either read input file or stdin
    if ( args->file_name != NULL ) {
        fp = fopen(args->file_name, "r");
        if (fp == NULL) {
            printf("Error opening file: %s\n", args->file_name);
            exit(1);
        }
    }
    else {
        fp = stdin;
    }
    
    // Search each line for the pattern
    while ( (read = getline(&line, &len, fp)) != -1) {
        LineInfo* info = malloc(sizeof(LineInfo));
        info->line_ptr = malloc(strlen(line) + 1);
        strcpy(info->line_ptr, line);
        info->line_num = line_count;
        info->bytes_until_line = total_read;

        total_read += read;
        line_count ++;

        search_result = search_pattern(line, args->pattern, case_sensitive, args->e_flag);
        found_match_in_line = search_result[0];
        line_equal_pattern = search_result[1];

        // Check if we got relevant lines for our search type
        match_flag1 = (rows_only_contain_pattern) && (line_equal_pattern);
        match_flag2 = (!rows_not_contain_pattern) && (!rows_only_contain_pattern) && (found_match_in_line);
        match_flag3 = (rows_not_contain_pattern) && (!found_match_in_line);
        match_flag = match_flag1 || match_flag2 || match_flag3;

        if ( match_flag || (num_rows_after > 0) ) {
            result_count++;

            if ( match_flag == 1 ) {
                info->seperator = ':';
                num_rows_after = args->a_num;
            } else {
                info->seperator = '-';
                num_rows_after--;
            }

            *results = realloc(*results, result_count * sizeof(LineInfo*));
            (*results)[result_count - 1] = info;
        }
        else {
            free(info->line_ptr);
            free(info);
        }

        free(search_result);
    }
    
    free(line); 

    if ( fp != stdin ){
        fclose(fp);
    }
    
    return result_count;
}


/* Search for pattern/regex in the line */
int* search_pattern(char* line, char* pattern, int case_sensitive, int is_regex) {
    int* result = malloc(2*sizeof(int));
    int pattern_len = strlen(pattern);
    int line_len = strlen(line);
    int match = 0; 
    int skip = 0;
    result[0] = 0;
    result[1] = 0;

    for (int i = 0; i <= (line_len - pattern_len); i++) {
        // Check if the substring starting at position i matches the pattern
        match = 1;
        for (int j = 0; j < pattern_len; j++) {
            char p = pattern[j];
            char l = line[i+j-skip];
            if (!case_sensitive) {
                p = tolower(p);
                l = tolower(l);
            }
            if (is_regex && (p == '\\') && (j < pattern_len - 1) && (pattern[j+1] == '.')) {
                // Escape character followed by a dot, check for literal dot in line
                if (l != '.') {
                    match = 0;
                    break;
                }
                j++;    // Skip the dot in the pattern
                skip++; // Fix the skip in the line
            } else if (p != l) { // Regular character, check for match
                match = 0;
                break;
            }        
        }
        
        if ( match == 1 ) { // Found a match
            result[0] =  1;
        }
    }
    
    if ( (result[0] == 1) && (line_len-pattern_len <= 1) ) { // Check exact
        if ( (line[line_len-1] == '\n') || (line_len-pattern_len == 0) ) {
            result[1] = 1;
        }
    }

    return result; 
}
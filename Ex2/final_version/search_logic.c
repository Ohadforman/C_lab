#define _POSIX_C_SOURCE 200809L // Needed for getline() without warnings 
#define _XOPEN_SOURCE 700       // Needed for getline() without warnings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include "search_logic.h"

/* Either read input file and return its fp or return stdin*/
FILE* get_input_file_pointer(grep_args* args) {
    FILE* fp;
    if ( args->file_name != NULL ) {
        fp = fopen(args->file_name, "r");
        if (fp == NULL) {
            printf("Error opening file: %s\n", args->file_name);
            exit(1);
        }
        return fp;
    }
    else {
        return stdin;
    }
}

/* Creates a LineInfo object for the line read*/
LineInfo* create_lineinfo_object(char* line, int line_count, int total_read) {
    LineInfo* info = malloc(sizeof(LineInfo));
    info->line_ptr = malloc(strlen(line) + 1);
    strcpy(info->line_ptr, line);
    info->line_num = line_count;
    info->bytes_until_line = total_read;

    return info;
}

/* Checks if we need to print the current row based on our search parameters*/
int is_row_relevant(int* search_result, grep_args* args) {
    int match_flag1, match_flag2, match_flag3;
    int found_match_in_line = search_result[0];
    int line_equal_pattern = search_result[1];
    int rows_not_contain_pattern = args->v_flag;
    int rows_only_contain_pattern = args->x_flag;

    // Check if we got relevant lines for our search type
    match_flag1 = (rows_only_contain_pattern) && (line_equal_pattern);
    match_flag2 = (!rows_not_contain_pattern) && (!rows_only_contain_pattern) && (found_match_in_line);
    match_flag3 = (rows_not_contain_pattern) && (!found_match_in_line);
    
    return  (match_flag1 || match_flag2 || match_flag3);
}

/* Read the lines into an array */
int control_get_lines(grep_args* args, LineInfo*** results) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read, total_read = 0;
    int is_relevant, line_count = 1, result_count = 0, num_rows_after = 0;
    int* search_result;
    
    fp = get_input_file_pointer(args);
    
    // Search each line for the pattern
    while ( (read = getline(&line, &len, fp)) != -1) {
        LineInfo* info = create_lineinfo_object(line, line_count, total_read);
        total_read += read;
        line_count ++;

        search_result = search_pattern(line, args->pattern, !args->i_flag, args->e_flag);
        is_relevant = is_row_relevant(search_result, args);

        if ( is_relevant || (num_rows_after > 0) ) {
            result_count++;

            if ( is_relevant == 1 ) {
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
    int match, skip;
    char p, l; 
    result[0] = 0;
    result[1] = 0;
    //printf("Lengths: %d %d\n", pattern_len, line_len);
    for (int i = 0; i < line_len; i++) {
        // Check if the substring starting at position i matches the pattern
        match = 1;
        skip = 0;
        for (int j = 0; j < pattern_len; j++) {
            //printf("%d %d\n", j, i);
            p = pattern[j];
            l = line[i+j-skip];
            
            if ( p == '\\' ) {      // Escape character
                p = pattern[++j];   // Skip escape char
                skip++;             // Fix the skip in the line
            }
            
            if ( !case_sensitive ) {
                p = tolower(p);
                l = tolower(l);
            }
            
            //printf("%d %d %c %c %d\n", j, i, p, l, p==l);
            if (p != l) { // Regular character, check for match
                match = 0;
                break;
            }        
        }
        
        if ( match == 1 ) { // Found a match
            result[0] =  1;
            break;
        }
    }
    
    if ( (result[0] == 1) && (line_len-pattern_len <= 1) ) { // Check exact
        if ( (line[line_len-1] == '\n') || (line_len-pattern_len == 0) ) {
            result[1] = 1;
        }
    }

    return result; 
}
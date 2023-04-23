#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"

/* Parse the command line arguments */
void parse_args(int argc, char* argv[], grep_args* args) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            args->b_flag = 1;
        } else if (strcmp(argv[i], "-A") == 0) {
            args->a_flag = 1;
            i++;
            args->a_num = atoi(argv[i]);
        } else if (strcmp(argv[i], "-v") == 0) {
            args->v_flag = 1;
        } else if (strcmp(argv[i], "-x") == 0) {
            args->x_flag = 1;
        } else if (strcmp(argv[i], "-i") == 0) {
            args->i_flag = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            args->n_flag = 1;
        } else if (strcmp(argv[i], "-c") == 0) {
            args->c_flag = 1;
        } else if (strcmp(argv[i], "-E") == 0) {
            args->e_flag = 1;
        } else if (args->pattern == NULL) {
            args->pattern = malloc(strlen(argv[i]) + 1);
            strcpy(args->pattern, argv[i]);
        } else if (args->file_name == NULL) {
            args->file_name = malloc(strlen(argv[i]) + 1);
            strcpy(args->file_name, argv[i]);
        } else {
            fprintf(stderr, "Invalid argument: %s\n", argv[i]);
            exit(1);
        }
    }

    if (args->pattern == NULL) {
        fprintf(stderr, "Missing pattern argument\n");
        exit(1);
    }
}


/* Read the lines into an array */
int control_get_lines(grep_args* args, LineInfo** results) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;
    size_t total_read = 0;
    int line_count = 0;
    int result_count = 0;
    int* search_result;
    int match_flag1;
    int match_flag2;
    int match_flag3;
    int found_match_in_line;
    int line_equal_pattern;

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
        info->line_ptr = line;
        info->line_num = line_count;
        info->bytes_until_line = total_read;

        total_read += read;
        line_count ++;

        search_result = search_pattern(line, args->pattern, case_sensitive);
        line_equal_pattern = search_result[0];
        found_match_in_line = search_result[1];

        // Check if we got relevant lines for our search type
        match_flag1 = (rows_only_contain_pattern) && (line_equal_pattern);
        match_flag2 = (!rows_not_contain_pattern) && (!rows_only_contain_pattern) && (found_match_in_line);
        match_flag3 = (rows_not_contain_pattern) && (!found_match_in_line);
        
        if ( match_flag1 || match_flag2 || match_flag3 ) {
            result_count++;
            results = realloc(results, result_count * sizeof(LineInfo));
            results[result_count - 1] = info;
        }
        free(search_result);
    }
    
    //free(line); //TODO: Is needed?
    
    if ( fp != stdin ){
        fclose(fp);
    }
    
    return result_count;
}


/* Search for pattern/regex in the line */
int* search_pattern(const char* line, const char* pattern, int case_sensitive) {
    int i, j = 0;
    int line_len = strlen(line);
    int pattern_len = strlen(pattern);
    int* result = (int*) calloc(2, sizeof(int));

    for (i = 0; i < line_len && j < pattern_len; i++) {
        char line_char = case_sensitive ? line[i] : tolower(line[i]);
        char pattern_char = case_sensitive ? pattern[j] : tolower(pattern[j]);
        if ( (line_char == pattern_char) || (pattern_char == '.') ) {
            j++;
            if (j == pattern_len) {
                result[1] = 1;
                if (i + 1 == line_len || line[i + 1] == ' ') { //TODO: doesn't seem correct
                    result[0] = 1;
                }
            }
        } else if (pattern_char == '(' || pattern_char == '[' || pattern_char == '{') {
            int k = j;
            while (pattern[k] != ')' && pattern[k] != ']' && pattern[k] != '}') {
                k++;
            }
            j = k + 1;
            if (j > pattern_len) {
                break;
            }
            if ((pattern_char == '(' && line_char != ')') ||
                (pattern_char == '[' && line_char != ']') ||
                (pattern_char == '{' && line_char != '}')) {
                break;
            }
        } else {
            j = 0;
        }
    }
    return result;
}



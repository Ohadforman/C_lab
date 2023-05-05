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
    int match, skip, got_escape;
    char p, l, x = 'x', y = 'y'; 
    result[0] = 0;
    result[1] = 0;
    
    for (int i = 0; i < line_len; i++) {
        // Check if the substring starting at position i matches the pattern
        match = 1;
        skip = 0;
        for (int j = 0; j < pattern_len; j++) {
            got_escape = 0;
            p = pattern[j];
            l = line[i+j-skip];
            //printf("%c %c\n", p, l);
            if ( (is_regex == 1) && (p == '.') ) { // We always match
                continue;
            }

            if ( (is_regex == 1) && (p == '[') ) { // Regex [x-y]
                x = pattern[j+1];
                y = pattern[j+3];
                j += 4;         // To skip x-y]
                skip += 4;      // Fix the skip in the line    
            }

            // TODO = add case sensitive
            if ( (is_regex == 1) && (p == '(') ) { // Regex (str1|str2)
                char* new_start = pattern + j;
                char* close_paren = strchr(new_start, ')');
                char* pipe = strchr(new_start, '|');
                int str1_len = pipe-new_start-1;
                int str2_len = close_paren-pipe-1;
                char* str1 = (char*)malloc(str1_len + 1);
                char* str2 = (char*)malloc(str2_len + 1);
                strncpy(str1, new_start+1, str1_len);
                strncpy(str2, pipe+1, str2_len); 
                //printf("%s %s\n", str1, str2);
                int line_index = i+j-skip;
                int check1 = 0;
                int check2 = 0;
                if (line_index+str1_len-1 < line_len) {
                    for (int k=0; k<str1_len; k++){
                        char p1 = case_sensitive ? line[line_index+k] : tolower(line[line_index+k]);
                        char p2 = case_sensitive ? str1[k] : tolower(str1[k]);
                        if ( p1 != p2 ) {
                            check1++;
                            break;
                        }
                    } 
                } else {
                    check1++;
                }
                if (line_index+str2_len-1 < line_len) {
                    for (int k=0; k<str2_len; k++){
                        char p1 = case_sensitive ? line[line_index+k] : tolower(line[line_index+k]);
                        char p2 = case_sensitive ? str2[k] : tolower(str2[k]);
                        if ( p1 != p2 ) {
                            check2++;
                            break;
                        }
                    } 
                } else {
                    check2++;
                }
                if ( (check1 > 0) && (check2 > 0) ) {
                    match = 0;
                    break; 
                }
                
                j += str1_len + str2_len + 2;
                skip += str1_len + str2_len + 2;
                i += (check1==0) ? (str1_len-1) : (str2_len-1);
                continue;  
            }

            if ( p == '\\' ) {      // Escape character
                p = pattern[++j];   // Skip escape char
                skip++;             // Fix the skip in the line
                got_escape = 1;
            }
            
            if ( !case_sensitive ) {
                p = tolower(p);
                l = tolower(l);
                x = tolower(x);
                y = tolower(y);
            }

            if ( is_regex == 1 ) {
                //printf("P: %c, L: %c, X: %c, Y: %c E=%d\n", p, l, x, y, got_escape);
                if ( (got_escape==0) && (p == '[') && !((x<=l)&&(l<=y)) ) {
                    match = 0;
                    break; 
                } else if ( ((got_escape==1) || (p != '[')) && (p != l) ){
                    match = 0;
                    break; 
                }
            } else if ( p != l ) {
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
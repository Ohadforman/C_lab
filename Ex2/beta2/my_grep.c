#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search_logic.h"
#include "print_control.h"

void free_line(LineInfo* line) {
    free(line->line_ptr);
    free(line);
}

void free_args(grep_args* args) {
    free(args->pattern);
    free(args->file_name);
    free(args);
}

void initialize_grep_args(grep_args** args) {
    (*args)->pattern = NULL;
    (*args)->file_name = NULL;
    (*args)->c_flag = 0;
    (*args)->n_flag = 0;
    (*args)->i_flag = 0;
    (*args)->v_flag = 0;
    (*args)->x_flag = 0;
    (*args)->e_flag = 0;
    (*args)->b_flag = 0;
    (*args)->a_flag = 0;
    (*args)->a_num = 0;
}

void parse_cmd_args(int argc, char* argv[], grep_args* args) {
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

int main(int argc, char* argv[]) {
    int rows_to_print = 0;
    LineInfo** lines = malloc(sizeof(LineInfo*));
    grep_args* args = malloc(sizeof(grep_args));
    initialize_grep_args(&args);

    // Fill the args structure with the input
    parse_cmd_args(argc, argv, args);      

    // Get the lines to print (matches and additional rows from -A flag)
    rows_to_print = control_get_lines(args, &lines);
    
    // Only print number of matches
    if ( args->c_flag == 1 ) {
        printf("%d\n", rows_to_print);
    }

    // Print and free all relevant rows
    for (int i=0; i<rows_to_print; i++) {
        if ( args->c_flag == 0 ) {
            print_line(lines[i], args);
        }
        free_line(lines[i]);
    }
    
    // Free main data structures
    free_args(args);
    free(lines);
       
    return 0;
}



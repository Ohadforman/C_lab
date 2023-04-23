#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
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

int main(int argc, char* argv[]) {
    int rows_to_print = 0;
    LineInfo** lines = malloc(sizeof(LineInfo*));
    grep_args* args = malloc(sizeof(grep_args));
    args->pattern = NULL;
    args->file_name = NULL;

    // Fill the args structure with the input
    parse_args(argc, argv, args);      

    // Get the lines to print (matches and additional rows from -A flag)
    rows_to_print = control_get_lines(args, &lines);
    
    // Print and free all relevant rows
    for (int i=0; i<rows_to_print; i++) {
        print_line(lines[i], args);
        free_line(lines[i]);
    }
    
    // Free main data structures
    free_args(args);
    free(lines);
       
    return 0;
}



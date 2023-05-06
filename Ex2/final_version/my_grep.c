#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "args_parser.h"
#include "search_logic.h"
#include "print_control.h"



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
            print_seperator_line(lines, rows_to_print, i);
        }
        free_line(lines[i]);
    }
    
    // Free main data structures
    free_args(args);
    free(lines);
       
    return 0;
}

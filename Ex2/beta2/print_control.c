#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_control.h"
#include "search_logic.h"

void print_line(LineInfo* current_line, grep_args* args) {
    char* line = current_line->line_ptr;
    int num_to_print = current_line->line_num;

    if ( args->b_flag == 1 ) {
        num_to_print = current_line->bytes_until_line;
    }

    if ( args->c_flag == 1 ) { // Print: Num
        printf("%d", num_to_print);
    } 
    else if ( (args->n_flag == 1) || (args->b_flag == 1) ) { // Print: Num+Line
        printf("%d%c%s", num_to_print, current_line->seperator, line);
    } 
    else { // Print: Line
        printf("%s", line);
    }
}
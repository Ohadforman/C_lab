#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_control.h"
#include "parse.h"

void print_line(LineInfo* current_line, grep_args* args) {
    char* line = current_line->line_ptr;
    int line_number = current_line->line_num;
    int bytes_number = current_line->bytes_until_line;
    char seperator = current_line->seperator;

    if ( args->b_flag == 1 ) { // Print number of bytes untill row
        printf("%d ", bytes_number);
    }

    if ( args->c_flag == 1 ) { // Print only row number
        printf("%d", line_number);
    } else if ( args->n_flag == 1 ) { // Print row number
        printf("%d%c%s", line_number, seperator, line);
    } else {
        printf("%s", line);
    }
}
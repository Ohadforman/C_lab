#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_control.h"
#include "parse.h"

void print_line(LineInfo* current_line, print_type print_type, int print_bytes_until_line) {
    char* line = current_line->line_ptr;
    int line_number = current_line->line_num;
    int bytes_number = current_line->bytes_until_line;

    if ( print_bytes_until_line == 1 ) {
        printf("%d ", bytes_number);
    }

    if ( print_type == NUM_COLON_LINE ) {
        printf("%d:%s", line_number,line);
    } else if ( print_type == NUM_DASH_LINE ) {
        printf("%d-%s", line_number, line);
    } else if ( print_type == ONLY_LINE_NUM ) {
        printf("%d", line_number);
    }else if ( print_type == ONLY_LINE ) {
        printf("%s", line);
    }
}

void print_num_lines(FILE* file, LineInfo* info, int num_lines, print_type print_type, int print_bytes_until_line) {
    if (num_lines == 0) {
        print_line(info, print_type, print_bytes_until_line);
        printf("--\n");
        free(info->line_ptr);
        return;
    }

    int lines_printed = 0;
    int lines_to_print = num_lines;
    char* line = info->line_ptr;

    // Print the specified line and num_lines after it
    while (line && lines_printed < num_lines) {
        if (lines_printed == 0 && num_lines > 1) {
            print_line(info, print_type, print_bytes_until_line);
        } else if (lines_printed > 0) {
            print_line(info, print_type, print_bytes_until_line);
        }

        lines_printed++;

        // Read the next line
        char* next_line = NULL;
        size_t len = 0;
        size_t read = getline(&next_line, &len, file);
        if (read == -1) {
            // End of file
            break;
        }

        // Free the previous line and update the current line pointer and line info
        free(line);
        line = next_line;
        //TODO: info = get_line_info(line, file);

        // Stop printing if we've printed the specified number of lines
        if (lines_printed == num_lines) {
            lines_to_print = 1;
            break;
        }
    }

    if (lines_to_print > 1) {
        printf("--\n");
    }

    free(line);
}








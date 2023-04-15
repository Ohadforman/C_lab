#include "print_control.h"
#include "parse.h"

void print_line(char* line,int line_number,int bytes_number, char* print_command, char* byte_command) {
    if (strcmp(byte_command, "print_bytes_until_line") == 0) {
        printf("%d ", bytes_number);
    }
    if (strcmp(print_command, "print_line_num_colon") == 0) {
        printf("%d:%s", line_number,line);
    } else if (strcmp(print_command, "print_line_num_dash") == 0) {
        printf("%d-%s", line_number, line);
    } else if (strcmp(print_command, "print_line_num_only") == 0) {
        printf("%d", line_number);
    }else if (strcmp(print_command, "print_without_number") == 0) {
        printf("%s", line);
    }
}

void print_num_lines(FILE* file, LineInfo info, int num_lines, char* print_command, char* byte_command) {
    if (num_lines == 0) {
        print_line(info.line_ptr, info.line_num, info.bytes_until_line, print_command, byte_command);
        printf("--\n");
        free(info.line_ptr);
        return;
    }

    int lines_printed = 0;
    int lines_to_print = num_lines;
    char* line = info.line_ptr;

    // Print the specified line and num_lines after it
    while (line && lines_printed < num_lines) {
        if (lines_printed == 0 && num_lines > 1) {
            print_line(line, info.line_num, info.bytes_until_line, print_command, byte_command);
        } else if (lines_printed > 0) {
            print_line(line, info.line_num, info.bytes_until_line, print_command, byte_command);
        }

        lines_printed++;

        // Read the next line
        char* next_line = NULL;
        size_t len = 0;
        ssize_t read = getline(&next_line, &len, file);
        if (read == -1) {
            // End of file
            break;
        }

        // Free the previous line and update the current line pointer and line info
        free(line);
        line = next_line;
        info = get_line_info(line, file);

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








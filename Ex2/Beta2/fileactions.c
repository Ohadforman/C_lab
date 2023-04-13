#include "fileactions.h"

int get_bytes_until_line(FILE* file, char* line) {
    int bytes = 0;
    char* current_line = NULL; 
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&current_line, &len, file)) != -1) {
        if (strcmp(current_line, line) == 0) {
            free(current_line);
            return bytes;
        }
        bytes += read;
    }

    free(current_line);
    return -1;
}

int get_line_number(FILE* file, char* line) {
    int line_number = 1;
    char* current_line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&current_line, &len, file)) != -1) {
        if (strcmp(current_line, line) == 0) {
            free(current_line);
            return line_number;
        }
        line_number++;
    }

    free(current_line);
    return -1;
}

void print_line(FILE* file, char* line, char* print_command, char* line_command) {

    if (strcmp(print_command, "print_bytes_until_line") == 0) {
        int bytes_until_line = get_bytes_until_line(file, line);
        printf("%d ", bytes_until_line);
    }

    if (strcmp(print_command, "print_line_num_colon") == 0) {
        printf("%d:", get_line_number(file, line));
    } else if (strcmp(print_command, "print_line_num_dash") == 0) {
        printf("%d-", get_line_number(file, line));
    } else if (strcmp(print_command, "print_line_num_only") == 0) {
        printf("%d", get_line_number(file, line));
    }

    if (strcmp(line_command, "print_line") == 0) {
        printf("%s", line);
    }

}

void print_num_lines(FILE* file, char* line, int num_lines, char* print_command, char* line_command) {
    int line_num = get_line_number(file, line);
    int lines_printed = 0;

    // Print the specified line and num_lines after it
    while (line && lines_printed < num_lines) {
        print_line(file, line, print_command, line_command);
        lines_printed++;

        // Read the next line
        char* next_line = NULL;
        size_t len = 0;
        ssize_t read = getline(&next_line, &len, file);
        if (read == -1) {
            // End of file
            break;
        }
        // Free the previous line and update the current line pointer
        free(line);
        line = next_line;

        line_num++;
        // Stop printing if we've printed the specified number of lines
        if (lines_printed == num_lines) {
            break;
        }
    }
    free(line);
}





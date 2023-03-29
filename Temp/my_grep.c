#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check_expression.h"
#include "my_grep.h"

int my_grep(char *filename, char *expression) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open the file '%s'\n", filename);
        return -1;
    }
    char *line = NULL;
    size_t line_length = 0;
    int line_number = 1;
    while (getline(&line, &line_length, file) != -1) {
        if (line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        if (check_expression(line, expression) == 1) {
            printf("%s\n", line);
        }
        line_number++;
    }
    free(line);
    fclose(file);
}

int main(int argc, char *argv[]) {
    char* expression;
    char* file_name;
    FILE* file;

    if ( argc != 3 ){
        printf("Error: Wrong number of arguments, expected 3 but got %d \n", argc);
        exit(1);
    }

    expression = argv[0];
    file_name = argv[1];
    file = fopen(file_name, "r");
    if ( file == NULL ) {
        printf("Error: Unable to open the file '%s' \n", file_name);
        exit(1);
    }

    find_expression_in_file(expression, file);
}

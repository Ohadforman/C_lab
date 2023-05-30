#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_grep.h"


void find_expression_in_file(char* expression, FILE* file){
    char* line = NULL;
    size_t line_length = 0;
    
    while ( getline(&line, &line_length, file) != -1 ) {
        if ( strstr(line, expression) != NULL ){
            printf("%s", line);
        }
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
    
    expression = argv[1];
    file_name = argv[2];
    file = fopen(file_name, "r");
    if ( file == NULL ) {
        printf("Error: Unable to open the file '%s' \n", file_name);
        exit(1);
    }

    find_expression_in_file(expression, file);
}

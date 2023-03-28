#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "check_expression.h"

int check_expression(char *line, char *expression) {
    int line_length = strlen(line);
    int expression_length = strlen(expression);
    char *lowercase_line = malloc(line_length + 1);
    char *lowercase_expression = malloc(expression_length + 1);
    int i;
    for (i = 0; i < line_length; i++) {
        lowercase_line[i] = tolower(line[i]);
    }
    lowercase_line[i] = '\0';
    for (i = 0; i < expression_length; i++) {
        lowercase_expression[i] = tolower(expression[i]);
    }
    lowercase_expression[i] = '\0';
    char *result = strstr(lowercase_line, lowercase_expression);
    free(lowercase_line);
    free(lowercase_expression);
    if (result != NULL) {
        return 1;  
    } else {
        return 0;   
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

int parse_args(int argc, char* argv[], grep_args* args) {
    args->pattern = NULL;
    args->file_name = NULL;
    args->num_lines = 0;
    args->c_flag = 0;
    args->n_flag = 0;
    args->i_flag = 0;
    args->v_flag = 0;
    args->x_flag = 0;
    args->e_flag = 0;
    args->b_flag = 0;
    args->a_flag = 0;
    args->a_num = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            args->b_flag = 1;
        } else if (strcmp(argv[i], "-A") == 0) {
            args->a_flag = 1;
            i++;
            args->a_num = atoi(argv[i]);
        } else if (strcmp(argv[i], "-v") == 0) {
            args->v_flag = 1;
        } else if (strcmp(argv[i], "-x") == 0) {
            args->x_flag = 1;
        } else if (strcmp(argv[i], "-i") == 0) {
            args->i_flag = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            args->n_flag = 1;
        } else if (strcmp(argv[i], "-c") == 0) {
            args->c_flag = 1;
        } else if (strcmp(argv[i], "-E") == 0) {
            args->e_flag = 1;
        } else if (args->pattern == NULL) {
            args->pattern = argv[i];
        } else if (args->file_name == NULL) {
            args->file_name = argv[i];
        } else {
            fprintf(stderr, "Too many arguments: %s\n", argv[i]);
            return 1;
        }
    }

    if (args->pattern == NULL) {
        fprintf(stderr, "Missing pattern argument\n");
        return 1;
    }

    if (args->file_name == NULL) {
        fprintf(stderr, "Missing file argument\n");
        return 1;
    }

    return 0;
}


char* remove_escape_char(char* pattern) {
    char* new_pattern = (char*) malloc(sizeof(char) * strlen(pattern));
    int j = 0;
    for (int i = 0; pattern[i] != '\0'; i++) {
        if (pattern[i] == '\\') {
            i++;
        }
        new_pattern[j] = pattern[i];
        j++;
    }
    new_pattern[j] = '\0';
    return new_pattern;
}
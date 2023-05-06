#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args_parser.h"



void free_args(grep_args* args) {
    free(args->pattern);
    free(args->file_name);
    free(args);
}

void initialize_grep_args(grep_args** args) {
    (*args)->pattern = NULL;
    (*args)->file_name = NULL;
    (*args)->c_flag = 0;
    (*args)->n_flag = 0;
    (*args)->i_flag = 0;
    (*args)->v_flag = 0;
    (*args)->x_flag = 0;
    (*args)->e_flag = 0;
    (*args)->b_flag = 0;
    (*args)->a_flag = 0;
    (*args)->a_num = 0;
}

void parse_cmd_args(int argc, char* argv[], grep_args* args) {
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
            args->pattern = malloc(strlen(argv[i]) + 1);
            strcpy(args->pattern, argv[i]);
        } else if (args->file_name == NULL) {
            args->file_name = malloc(strlen(argv[i]) + 1);
            strcpy(args->file_name, argv[i]);
        } else {
            fprintf(stderr, "Invalid argument: %s\n", argv[i]);
            exit(1);
        }
    }

    if (args->pattern == NULL) {
        fprintf(stderr, "Missing pattern argument\n");
        exit(1);
    }
}
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



line_tuple* control_get_lines(const char* filename, const char* expression, const char* command, int* num_lines) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    line_tuple* results = NULL;
    int result_count = 0;
    int i;
    long int byte_offset = 0;
    int line_number = 1;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }
    int case_sensitive = (command[1] == 'c') ? 1 : 0; // check if case-sensitive flag is set
    while ((read = getline(&line, &len, fp)) != -1) {
        int* search_result = search_pattern(line, expression, case_sensitive);
        if (command[0] == 'e') { // exact match
            if (search_result[0]) {
                result_count++;
                results = realloc(results, result_count * sizeof(line_tuple));
                results[result_count - 1].line_ptr = strdup(line);
                results[result_count - 1].byte_offset = byte_offset;
                results[result_count - 1].line_number = line_number;
            }
        } else if (command[0] == 'w') { // with expression
            if (search_result[1]) {
                result_count++;
                results = realloc(results, result_count * sizeof(line_tuple));
                results[result_count - 1].line_ptr = strdup(line);
                results[result_count - 1].byte_offset = byte_offset;
                results[result_count - 1].line_number = line_number;
            }
        } else if (command[0] == 'o') { // without expression
            if (!search_result[1]) {
                result_count++;
                results = realloc(results, result_count * sizeof(line_tuple));
                results[result_count - 1].line_ptr = strdup(line);
                results[result_count - 1].byte_offset = byte_offset;
                results[result_count - 1].line_number = line_number;
            }
        }
        byte_offset += read;
        line_number++;
        free(search_result);
    }
    free(line);
    fclose(fp);
    *num_lines = result_count;
    return results;
}

int* search_pattern(const char* line, const char* pattern, int case_sensitive) {
    int i, j, k;
    int line_len = strlen(line);
    int pattern_len = strlen(pattern);
    int* stack = malloc(sizeof(int) * pattern_len);
    int top = -1;
    int* result = malloc(sizeof(int) * 2);
    result[0] = 0; // initialize exact match flag to 0
    result[1] = 0; // initialize pattern found flag to 0
    for (i = 0; i < line_len; i++) {
        char line_char = line[i];
        char pattern_char = pattern[j];
        if (!case_sensitive) {// convert to lowercase for case-insensitive search     
            line_char = tolower(line_char);
            pattern_char = tolower(pattern_char);
        }
        if (line_char == pattern_char || pattern_char == '.') {
            if (pattern_char == '(' || pattern_char == '[' || pattern_char == '{' || pattern_char == '|') {
                top++;
                stack[top] = j;
            } else if (pattern_char == ')' || pattern_char == ']' || pattern_char == '}') {
                if (top == -1) {
                    free(stack);
                    return result;
                }
                k = stack[top];
                top--;
                if ((pattern[k] == '(' && pattern_char != ')') ||
                    (pattern[k] == '[' && pattern_char != ']') ||
                    (pattern[k] == '{' && pattern_char != '}')) {
                    free(stack);
                    return result;
                }
            }
            j++;
            if (j == pattern_len) {
                result[1] = 1; // pattern found
                if (i + 1 == line_len || line[i + 1] == ' ') {
                    result[0] = 1; // exact match found
                }
                free(stack);
                return result;
            }
        } else {
            if (top != -1) {
                j = stack[top];
                top--;
            } else {
                j = 0;
            }
        }
    }
    free(stack);
    return result;
}


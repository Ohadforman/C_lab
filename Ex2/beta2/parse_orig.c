#include "parse.h"

void parse_args(int argc, char* argv[], grep_args* args) {
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
            fprintf(stderr, "Invalid argument: %s\n", argv[i]);
            exit(1);
        }
    }
    if (args->pattern == NULL) {
        fprintf(stderr, "Missing pattern argument\n");
        exit(1);
    }
    if (args->file_name == NULL) {
        fprintf(stderr, "Missing file argument\n");
        exit(1);
    }
}



LineInfo get_line_info(char* line, FILE* file) {
    LineInfo info = { line, 1, ftell(file) };
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;

    // Read each line of the file until we reach the line we want
    while ((read = getline(&buffer, &len, file)) != -1) {
        if (buffer == line) {
            break;
        }
        info.bytes_until_line += read;
        info.line_num++;
    }

    free(buffer);
    return info;
}

LineInfo* control_get_lines(const char* filename, const char* expression, const char* command) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    LineInfo* results = NULL;
    int result_count = 0;
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
                results = realloc(results, result_count * sizeof(LineInfo));
                results[result_count - 1] = get_line_info(line, fp);
            }
        } else if (command[0] == 'w') { // with expression
            if (search_result[1]) {
                result_count++;
                results = realloc(results, result_count * sizeof(LineInfo));
                results[result_count - 1] = get_line_info(line, fp);
            }
        } else if (command[0] == 'o') { // without expression
            if (!search_result[1]) {
                result_count++;
                results = realloc(results, result_count * sizeof(LineInfo));
                results[result_count - 1] = get_line_info(line, fp);
            }
        }
        free(search_result);
    }
    free(line);
    fclose(fp);
    return results;
}


int* search_pattern(const char* line, const char* pattern, int case_sensitive) {
    int i, j = 0;
    int line_len = strlen(line);
    int pattern_len = strlen(pattern);
    int* result = (int*) calloc(2, sizeof(int));
    for (i = 0; i < line_len && j < pattern_len; i++) {
        char line_char = case_sensitive ? line[i] : tolower(line[i]);
        char pattern_char = case_sensitive ? pattern[j] : tolower(pattern[j]);
        if (line_char == pattern_char || pattern_char == '.') {
            j++;
            if (j == pattern_len) {
                result[1] = 1;
                if (i + 1 == line_len || line[i + 1] == ' ') {
                    result[0] = 1;
                }
            }
        } else if (pattern_char == '(' || pattern_char == '[' || pattern_char == '{') {
            int k = j;
            while (pattern[k] != ')' && pattern[k] != ']' && pattern[k] != '}') {
                k++;
            }
            j = k + 1;
            if (j > pattern_len) {
                break;
            }
            if ((pattern_char == '(' && line_char != ')') ||
                (pattern_char == '[' && line_char != ']') ||
                (pattern_char == '{' && line_char != '}')) {
                break;
            }
        } else {
            j = 0;
        }
    }
    return result;
}



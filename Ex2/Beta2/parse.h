#ifndef PARSE_H
#define PARSE_H

// Struct to hold parsed command line arguments
typedef struct {
    char* pattern;
    char* file_name;
    int num_lines;
    int c_flag;
    int n_flag;
    int i_flag;
    int v_flag;
    int x_flag;
    int e_flag;
    int b_flag;
    int a_flag;
    int a_num;
} grep_args;

// Function to parse command line arguments and populate grep_args struct
int parse_args(int argc, char* argv[], grep_args* args);

// Function to remove escape characters from pattern string
char* remove_escape_char(char* pattern);

#endif /* PARSE_H */

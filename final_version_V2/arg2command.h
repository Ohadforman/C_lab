#ifndef ARG2COMMAND_H
#define ARG2COMMAND_H


typedef struct grep_args {
    char* pattern;
    char* file_name;
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

void free_args(grep_args* args);
void initialize_grep_args(grep_args** args);
void parse_cmd_args(int argc, char* argv[], grep_args* args); 
#endif /* ARG2COMMAND_H */
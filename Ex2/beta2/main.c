#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

int main(int argc, char* argv[]) {
    grep_args args;// Use args.pattern, args.file_name, and other arguments as needed 
    int ret = parse_args(argc, argv, &args);
    if (ret != 0) {
        exit(1);
    }

    char* file_name = args.file_name;// Transform file name to pointer to file
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", file_name);
        exit(1);
    }

    //Todo-send to control_get_lines with commands and get a list of the lines to print (a int of number of line in the list)
    // Call control_get_lines function to get list of lines that match the search pattern

    //int num_lines = 0;
    //line_tuple* lines = control_get_lines(fp, args.pattern, , &num_lines);
    
    //Todo-send to print_line or to print_num_lines(also with number of lines) with commands and depends on command 
    
    fclose(fp);
    args.file_name = file_name;    
    return 0;
}



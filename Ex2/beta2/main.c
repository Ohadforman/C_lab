#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "print_control.h"

int main(int argc, char* argv[]) {
    char print_command[30],byte_command[30];
    char command_search[2];
    grep_args args;// Use args.pattern, args.file_name, and other arguments as needed 
    parse_args(argc, argv, &args);
    char* file_name = args.file_name;// Transform file name to pointer to file
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", file_name);
        exit(1);
    }
    //set commands
    if(args.a_flag)
        strcpy(print_command, "print_line_num_colon");
    else
        strcpy(print_command, "print_line_num_colon");
    if(args.b_flag)
        strcpy(print_command, "print_line_num_only");
    if(args.c_flag)
        strcpy(print_command, "print_bytes_until_line");
    if(args.i_flag)
        command_search[1] = 'c';    
    if(!args.n_flag)
        strcpy(print_command, "print_without_number");
    if(args.v_flag)
        command_search[0] = 'o'; 
    if(args.x_flag)
        command_search[0] = 'e'; 
    if(command_search[0]!='o'&&command_search[0]!='e')
        command_search[0] = 'w';
    //run search and print
    LineInfo* lines = control_get_lines(file_name, args.pattern, command_search);
    // Iterate over the lines and print the line number and bytes until the next line
    int i = 0;
    while (lines[i].line_ptr != NULL) {
        LineInfo current_line = lines[i];
        if(args.a_flag)
            print_num_lines(fp,current_line,args.a_num,print_command,byte_command);
        else
            print_line(current_line.line_ptr,current_line.line_num,current_line.bytes_until_line, print_command, byte_command);
        i++;
    }
    free(lines);
    print_control(lines, print_command, byte_command);
    free(lines);

    fclose(fp);
    args.file_name = file_name;    
    return 0;
}



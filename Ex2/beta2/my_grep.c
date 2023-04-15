#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "print_control.h"

int main(int argc, char* argv[]) {
    char print_command[30];
    char byte_command[30];
    char command_search[2];
    grep_args args;
    FILE* fp;          
    
    // Fill the args structure with the input
    parse_args(argc, argv, &args);      
    
    // Get the lines array
    LineInfo* lines = control_get_lines(args);

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
       
    return 0;
}



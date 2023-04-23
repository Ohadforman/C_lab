#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "print_control.h"

int main(int argc, char* argv[]) {
    int num_of_results = 0;
    print_type print_command;
    char command_search[2];
    FILE* fp;
    grep_args* args = malloc(sizeof(grep_args));
    LineInfo* current_line = malloc(sizeof(LineInfo));
    LineInfo** lines = malloc(sizeof(LineInfo*));

    // Fill the args structure with the input
    parse_args(argc, argv, args);      

    // Get the lines array
    num_of_results = control_get_lines(args, &lines);
    
    // Iterate over the lines and print the line number and bytes until the next line
    for (int i=0; i<num_of_results; i++) {
        current_line->line_ptr = lines[i]->line_ptr;
        current_line->line_num = lines[i]->line_num;
        current_line->bytes_until_line = lines[i]->bytes_until_line;

        print_command = ONLY_LINE; //TODO: Delete
        if ( args->n_flag ) {
            print_command = NUM_COLON_LINE; //TODO: Delete
        }

        if(args->a_flag) {
            print_num_lines(fp,current_line,args->a_num, print_command,args->b_flag);
        }
        else {
            print_line(current_line, print_command, args->b_flag);
        }
    }

    
    //free(lines);
    //TODO: print_control(lines, print_command, byte_command);
    //free(lines);

    //fclose(fp);
       
    return 0;
}



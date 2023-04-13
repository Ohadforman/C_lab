#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

int main(int argc, char* argv[]) {
    grep_args args;
    int ret = parse_args(argc, argv, &args);
    if (ret != 0) {
        exit(1);
    }
    if (args.e_flag==1)
        {args.pattern==remove_escape_char(args.pattern);}
    // Use args.pattern, args.file_name, and other arguments as needed 
    return 0;
}


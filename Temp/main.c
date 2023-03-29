#include <stdio.h>
#include "my_grep.h"

int main(int argc, char *argv[]) {
    /* Check if the correct number of arguments are provided */
    if (argc != 3) {
        printf("Usage: %s <file_name> <expression>\n", argv[0]);
        return -1;
    }
    char *filename = argv[1];
    char *expression = argv[2];
    my_grep(filename, expression);
}

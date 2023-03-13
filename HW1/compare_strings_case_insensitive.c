#include "compare_strings_case_insensitive.h"
#include <string.h>
#include <ctype.h>

int compare_strings_case_insensitive(const char* input, const char* target) {
    char input_lower[LENGTH_OF_INPUT];
    char target_lower[LENGTH_OF_INPUT];
    int i = 0;
    while (input[i]) {
        input_lower[i] = tolower(input[i]);
        i++;
    }
    input_lower[i] = '\0';
    i = 0;
    while (target[i]) {
        target_lower[i] = tolower(target[i]);
        i++;
    }
    target_lower[i] = '\0';
    return strcmp(input_lower, target_lower);
}

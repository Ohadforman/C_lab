#include <stdio.h>
#include <string.h>
#include <ctype.h>

int search_pattern(char* pattern, char* line, int case_sensitive, int is_regex) {
    int pattern_len = strlen(pattern);
    int line_len = strlen(line);
    int i, j;

    for (i = 0; i <= line_len - pattern_len; i++) {
        // Check if the substring starting at position i matches the pattern
        int match = 1;
        for (j = 0; j < pattern_len; j++) {
            char p = pattern[j];
            char l = line[i+j];
            if (!case_sensitive) {
                p = tolower(p);
                l = tolower(l);
            }
            if (is_regex && p == '\\' && j < pattern_len - 1 && pattern[j+1] == '.') {
                // Escape character followed by a dot, check for literal dot in line
                if (l != '.') {
                    match = 0;
                    break;
                }
                j++; // Skip the dot in the pattern
            } else if (p != l) {
                // Regular character, check for match
                match = 0;
                break;
            }        
        }
        if (match) {
            // Found a match
            return 1;
        }
    }

    // Pattern not found
    return 0;
}

int main() {
    char* pattern = "ohadpdf";
    char* line = "formanOhadpdf";
    int case_sensitive = 1;
    int is_regex = 0;

    if (search_pattern(pattern, line, case_sensitive, is_regex)) {
        printf("Pattern found in line.\n");
    } else {
        printf("Pattern not found in line.\n");
    }

    return 0;
}

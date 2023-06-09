#define _POSIX_C_SOURCE 200809L  // Needed for getline() without warnings
#define _XOPEN_SOURCE 700        // Needed for getline() without warnings
#include "search_logic.h"
#include "args_parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define LOW_RANGE_NEEDED_SHIFT 1
#define HIGH_RANGE_NEEDED_SHIFT 3
#define NUM_OF_CHARS_TO_SKIP_IF_RANGE 4
#define NUM_OF_CHARS_TO_IGNORE_IN_PATTERN 3

int* handle_regex(char* pattern, char* line, int j, int pattern_len, int case_sensitive, int is_regex, int is_exact);

/* Either read input file and return its fp or return stdin*/
FILE* get_input_file_pointer(grep_args* args)
{
  FILE* fp;
  if (args->file_name != NULL) {
    fp = fopen(args->file_name, "r");
    if (fp == NULL) {
      printf("Error opening file: %s\n", args->file_name);
      exit(1);
    }
    return fp;
  } else {
    return stdin;
  }
}

/* Creates a LineInfo object for the line read*/
LineInfo* create_lineinfo_object(char* line, int line_count, int total_read)
{
  LineInfo* info = malloc(sizeof(LineInfo));
  info->line_ptr = malloc(strlen(line) + 1);
  strcpy(info->line_ptr, line);
  info->line_num = line_count;
  info->bytes_until_line = total_read;

  return info;
}

/* Checks if we need to print the current row based on our search parameters*/
int is_row_relevant(const int* search_result, grep_args* args)
{
  int exact_match_search_found;
  int regular_match_search_found;
  int opposite_match_search_found;
  int found_match_in_line = search_result[0];
  int line_equal_pattern = search_result[1];
  int rows_not_contain_pattern = args->v_flag;
  int rows_only_contain_pattern = args->x_flag;

  // Check if we got relevant lines for our search type
  exact_match_search_found = (rows_only_contain_pattern) && (line_equal_pattern);
  regular_match_search_found = (!rows_not_contain_pattern) && (!rows_only_contain_pattern) && (found_match_in_line);
  opposite_match_search_found = (rows_not_contain_pattern) && (!found_match_in_line);

  return (exact_match_search_found || regular_match_search_found || opposite_match_search_found);
}

/* Check if current char fits the pattern*/
int is_mismatch(int is_range, char p, char l, char x, char y)
{
  if ((is_range == 1) && !((x <= l) && (l <= y))) {  // We got l not in range [x-y]
    return 1;
  }
  if ((is_range == 0) && (p != l)) {  // If it's not a range we just compare
    return 1;
  }
  return 0;
}

/**/
char* extract_substring_options(char** pattern, int index, char** str1, char** str2)
{
  char* open_parem = (*pattern) + index;
  char* close_paren = strchr(open_parem, ')');
  char* pipe = strchr(open_parem, '|');
  int str1_len = pipe - open_parem - 1;
  int str2_len = close_paren - pipe - 1;

  *str1 = (char*)realloc(*str1, str1_len + 1);
  if (*str1 == NULL) {
    printf("Failed to allocate memory to substring\n");
    exit(1);
  }
  strncpy(*str1, open_parem + 1, str1_len);
  (*str1)[str1_len] = '\0';

  *str2 = (char*)realloc(*str2, str2_len + 1);
  if (*str2 == NULL) {
    printf("Failed to allocate memory to substring\n");
    exit(1);
  }
  strncpy(*str2, pipe + 1, str2_len);
  (*str2)[str2_len] = '\0';

  return close_paren;
}

int* search_pattern(char* line, char* pattern, int case_sensitive, int is_regex, int is_exact)
{
  int* result = malloc(2 * sizeof(int));
  int pattern_len = strlen(pattern), line_len = strlen(line), match, skip, is_range, i, j;
  char p, l, low_range = 'x', high_range = 'y';
  result[0] = 0, result[1] = 1;
  for (i = 0; i < line_len; i++) {
    match = 1, skip = 0;
    for (j = 0; j < pattern_len; j++) {
      is_range = 0;
      p = pattern[j], l = line[i + j - skip];
      if ((is_regex == 1) && (p == '.')) {  // Regex .
        p = l;
      }
      if ((is_regex == 1) && (p == '[')) {  // Regex [x-y]
        low_range = pattern[j + LOW_RANGE_NEEDED_SHIFT];
        high_range = pattern[j + HIGH_RANGE_NEEDED_SHIFT];
        j += NUM_OF_CHARS_TO_SKIP_IF_RANGE;     // To skip x-y]
        skip += NUM_OF_CHARS_TO_SKIP_IF_RANGE;  // Fix the skip in the line
        is_range = 1;                           // Flag we nee a range check instead of regular compare
      }
      if ((is_regex == 1) && (p == '(')) {  // Regex (str1|str2)
        int* regex_result = handle_regex(pattern, line, j, pattern_len, case_sensitive, is_regex, is_exact);
        result[0] = regex_result[0], result[1] = regex_result[1];
        free(result);
        return regex_result;
      }
      if ((is_regex == 1) && (p == '\\')) {  // Escape character
        p = pattern[++j];                    // Skip escape char
        skip++;                              // Fix the skip in the line
      }
      if (!case_sensitive) {  // If not case sensitive change all to lower
        p = tolower(p);
        l = tolower(l);
        low_range = tolower(low_range), high_range = tolower(high_range);
      }
      if (is_mismatch(is_range, p, l, low_range, high_range)) {
        match = 0, result[1] = 0;  // If we got a mismatch it can't be an exact match
        break;
      }
    }
    if (match == 1) {  // Found a match
      result[0] = 1;
      break;
    }
  }
  if (i + j - skip != line_len - 1) {  // Checks we got to the last index in the line
    result[1] = 0;                     // If not it's not exact (we got extra chars at the end)
  }
  return result;
}

int* handle_regex(char* pattern, char* line, int j, int pattern_len, int case_sensitive, int is_regex, int is_exact)
{
  int* result = (int*)malloc(sizeof(int) * 2);
  char* str1 = NULL;
  char* str2 = NULL;
  char* close_parem = extract_substring_options(&pattern, j, &str1, &str2);
  int possible_pattern1_len = pattern_len - strlen(str2) - NUM_OF_CHARS_TO_IGNORE_IN_PATTERN;
  int possible_pattern2_len = pattern_len - strlen(str1) - NUM_OF_CHARS_TO_IGNORE_IN_PATTERN;
  char* possible_pattern1 = (char*)malloc(possible_pattern1_len + 1);
  char* possible_pattern2 = (char*)malloc(possible_pattern2_len + 1);
  strncpy(possible_pattern1, pattern, j + 1);
  strncpy(possible_pattern2, pattern, j + 1);
  possible_pattern1[j] = '\0';
  possible_pattern2[j] = '\0';
  possible_pattern1 = strcat(possible_pattern1, str1);
  possible_pattern2 = strcat(possible_pattern2, str2);
  free(str1);
  free(str2);
  possible_pattern1 = strcat(possible_pattern1, close_parem + 1);
  possible_pattern2 = strcat(possible_pattern2, close_parem + 1);
  int* result1 = search_pattern(line, possible_pattern1, case_sensitive, is_regex, is_exact);
  int* result2 = search_pattern(line, possible_pattern2, case_sensitive, is_regex, is_exact);
  free(possible_pattern1);
  free(possible_pattern2);
  result[0] = result1[0] || result2[0];
  result[1] = result1[1] || result2[1];
  free(result1);
  free(result2);
  return result;
}

/* Read the lines into an array */
int control_get_lines(grep_args* args, LineInfo*** results)
{
  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  ssize_t read, total_read = 0;
  int is_relevant, line_count = 1, result_count = 0, num_rows_after = 0;
  int* search_result;

  fp = get_input_file_pointer(args);

  // Search each line for the pattern
  while ((read = getline(&line, &len, fp)) != -1) {
    LineInfo* info = create_lineinfo_object(line, line_count, total_read);
    total_read += read;
    line_count++;

    search_result = search_pattern(line, args->pattern, !args->i_flag, args->e_flag, args->x_flag);
    is_relevant = is_row_relevant(search_result, args);

    if (is_relevant || (num_rows_after > 0)) {
      result_count++;

      if (is_relevant == 1) {
        info->seperator = ':';
        num_rows_after = args->a_num;
      } else {
        info->seperator = '-';
        num_rows_after--;
      }

      *results = realloc(*results, result_count * sizeof(LineInfo*));
      (*results)[result_count - 1] = info;
    } else {
      free(info->line_ptr);
      free(info);
    }
    free(search_result);
  }

  free(line);
  if (fp != stdin) {
    fclose(fp);
  }
  return result_count;
}

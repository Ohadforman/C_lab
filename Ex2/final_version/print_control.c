#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args_parser.h"
#include "print_control.h"
#include "search_logic.h"

void free_line(LineInfo* line)
{
  free(line->line_ptr);
  free(line);
}

void print_line(LineInfo* current_line, grep_args* args)
{
  char* line = current_line->line_ptr;
  int num_to_print = current_line->line_num;

  if (args->b_flag == 1) {
    num_to_print = current_line->bytes_until_line;
  }

  if (args->c_flag == 1) {  // Print: Num
    printf("%d", num_to_print);
  } else if ((args->n_flag == 1) || (args->b_flag == 1)) {  // Print: Num+Line
    printf("%d%c%s", num_to_print, current_line->seperator, line);
  } else {  // Print: Line
    printf("%s", line);
  }

  // Add a newline after each row
  if ((args->c_flag == 0) && (line[strlen(line) - 1] != '\n')) {
    printf("\n");
  }
}

void print_seperator_line(LineInfo** lines, int lines_num, int i)
{
  if (i == (lines_num - 1)) {  // If we printed the last line, no need for seperator
    return;
  }
  if (lines[i]->seperator != '-') {  // If the line we printed was a regular match
    return;
  }
  // If next line is regular match and its not the following line
  else if ((lines[i + 1]->seperator == ':') && (lines[i]->line_num + 1 != lines[i + 1]->line_num)) {
    printf("--\n");
  }
}

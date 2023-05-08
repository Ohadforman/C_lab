#ifndef SEARCH_LOGIC_H
#define SEARCH_LOGIC_H

#include "args_parser.h"

// Struct to hold line info
typedef struct LineInfo {
  char* line_ptr;
  int line_num;
  int bytes_until_line;
  char seperator;
} LineInfo;

int control_get_lines(grep_args* args, LineInfo*** results);

#endif /* SEARCH_LOGIC_H */

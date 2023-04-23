#ifndef PRINTCONTROL_H
#define PRINTCONTROL_H

#include "parse.h"

// Print a line from the file according to the specified print and line commands
void print_line(LineInfo* line, grep_args* args);

#endif /* PRINTCONTROL_H */

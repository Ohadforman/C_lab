# TODO 6/5

## logic tests 
**run18**
- [ ] 
`./my_grep -x -E '(ab|a).(d|cd)[x-z]' 
~nimrodav/grep_tests/in18 | diff ~nimrodav/grep_tests/out18 -
valgrind --quiet --leak-check=yes ./my_grep -x -E '(ab|a).(d|cd)[x-z]' ~nimrodav/grep_tests/in18 | diff ~nimrodav/grep_tests/out18 -`

**run20**
- [ ] 
`./my_grep -x -i -E '(Ab|a).(d|cD)[X-Z]' ~nimrodav/grep_tests/in20 | diff ~nimrodav/grep_tests/out20 -
valgrind --quiet --leak-check=yes ./my_grep -x -i -E '(Ab|a).(d|cD)[X-Z]' ~nimrodav/grep_tests/in20 | diff ~nimrodav/grep_tests/out20 -`

## Clang ~nimrodav/lint/lint.sh
- [ ] 
1) search_logic.c:44:26: warning: pointer parameter 'search_result' can be pointer to const [readability-non-const-parameter]
int is_row_relevant(int *search_result, grep_args *args)
---
- [ ] 
2)  search_logic.c:103:80: warning: pointer parameter 'line' can be pointer to const [readability-non-const-parameter]
int check_if_exact_match(int match_found, int line_len, int pattern_len, char *line)
---
- [ ] 
3) search_logic.c:114:6: warning: function 'search_pattern' exceeds recommended size/complexity thresholds [google-readability-function-size]
int *search_pattern(char *line, char *pattern, int case_sensitive, int is_regex)

- [ ] 
3.a **(Same as 3?)** search_logic.c:114:6: note: 100 lines including whitespace and comments (threshold 50) 

---
- [ ] 
4) search_logic.c:147:47: note: nesting level 5 starts here (threshold 4)
        if ((str1 == NULL) || (str2 == NULL)) { 

---
- [ ] 
5) search_logic.c:157:73: note: nesting level 5 starts here (threshold 4)
        if ((possible_pattern1 == NULL) || (possible_pattern2 == NULL)) {



## File missing

- [ ] 
- DESIGN File not found!

___


## To think about args.c/.h
clear my_grep, leave with only main. all the args functions 
**free_args**,
**initialize_grep_args**,
**parse_cmd_args**,
to args modul (args2command?).


also **struct grep_args** from search_logic 

*make it main, **my_grep**, use modul args to start, continue with search and finish with print.*

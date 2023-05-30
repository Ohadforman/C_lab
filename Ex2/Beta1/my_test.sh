#! /usr/bin/env tcsh

./my_grep abc1 Test_Input | diff Test_Output -

valgrind --quiet --leak-check=yes ./my_grep abc1 Test_Input | diff Test_Output -

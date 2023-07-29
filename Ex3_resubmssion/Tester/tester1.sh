#! /usr/bin/env bash

~nimrodav/socket_ex/verify_readme.sh

if [[ -f ./server_port ]]; then
	rm ./server_port
fi
if [[ -f ./http_port ]]; then
	rm ./http_port
fi

ls -la | grep ' \.$' | grep -v "drwxrwxrwx"
ls -la | grep -v '\.$' | grep -v "rwxr-xr-x" | grep -v "total "

~nimrodav/grep_tests/check_line_lengths.sh
python2.7 ~nimrodav/grep_tests/check_private_functions.py ./ex3_lb
~nimrodav/header_guards/check_header_guards.sh
~nimrodav/header_guards/check_unnecessary_includes.sh
~nimrodav/lint/lint.sh
~nimrodav/grep_tests/check_defines.py
~nimrodav/grep_tests/check_magic_constants.sh
~nimrodav/grep_tests/check_duplicated_constants.py

cat BIND_RECV | grep -v -i BIND_RECV

./ex3_lb &
disown

~nimrodav/socket_ex/wait_for_file_to_be_created_or_timeout.py server_port 3 || exit 1

./ex3_server `cat server_port` &
disown
./ex3_server `cat server_port` &
disown
./ex3_server `cat server_port` &
disown

~nimrodav/socket_ex/wait_for_file_to_be_created_or_timeout.py http_port 3 || exit 1

python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/1 -
python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/1 -
python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/1 -

python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/2 -
python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/2 -
python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/2 -

python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/3 -
python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/3 -
python2.7 ~nimrodav/socket_ex/run_test.py `cat http_port` | diff ~nimrodav/socket_ex/3 -

python2.7 ./run_test2.py `cat http_port`
python2.7 ./run_test2.py `cat http_port`
python2.7 ./run_test2.py `cat http_port`

python2.7 ./run_test3.py `cat http_port` | grep -w '4'
python2.7 ./run_test3.py `cat http_port` | grep -w '4'
python2.7 ./run_test3.py `cat http_port` | grep -w '4'

rm server_port
rm http_port

pkill -x -u `whoami` "ex3_lb"
pkill -x -u `whoami` "ex3_server"
pkill -f -u `whoami` "python2.7"
pkill -f -u `whoami` "python"

pkill -9 -x -u `whoami` "ex3_lb"
pkill -9 -x -u `whoami` "ex3_server"
pkill -9 -f -u `whoami` "python2.7"
pkill -9 -f -u `whoami` "python"

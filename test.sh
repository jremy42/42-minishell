#!/bin/bash

DIF=""
RET_MINI=""
RET_BASH=""

function test_str ()
{
	echo -n "Testing"  "'$1'"
	echo "$1" | ./minishell > output_file_minishell
	RET_MINI=$?
	echo "$1" | bash --posix > output_file_bash
	RET_BASH=$?
	DIF=$(diff output_file_minishell output_file_bash)
	test -z "$DIF" && echo -e "\x1b[1;32mOK output\x1b[0m" || echo -e "\x1b[1;31mKO output stdout\x1b[0m" "\n" "$DIF"
	test "$RET_MINI" -eq "$RET_BASH" && echo -e "\x1b[1;32mOK return value\x1b[0m" || echo -e "\x1b[1;31mKO return value\x1b[0m" "\n" "Minishell $RET_MINI | Bash $RET_BASH"
}

#### TEST echo ####

test_str 'echo toto'
test_str 'echo -n toto'
test_str 'echo -nnnnnn toto'
test_str 'echo -n -ntoto'
test_str 'echo -n -f toto'
test_str 'echo -n toto -n'
test_str 'echo toto -n'

rm -rf output_file_bash	output_file_minishell

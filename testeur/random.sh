#!/bin/bash

AVAILABLE_INPUTS=("&&" "||" "|" "&" "(" ")" "ls" "lso" "lso" "\"" "'" "\"" "'" ">" "<" ">>" "(" ")" "&|" "|&")
SIZE=${#AVAILABLE_INPUTS[*]}
TEST_RANGE=2000

function create_input ()
{
unset INPUTS
unset MAX
MAX=$(( 1 + $RANDOM % 20 ))
for i in $(seq 1 $MAX)
do
	INPUTS+=( ${AVAILABLE_INPUTS[$(( RANDOM % $SIZE ))]} )
done
echo ${INPUTS[*]}
}

while test "$TEST_RANGE" -ge 0
do
	create_input
	echo ${INPUTS[*]} | ./minishell 1> /dev/null 2>> .err_file 
	test $? -gt 128 && (echo -e "\x1b[31mKO\x1b[0m" "input : ${INPUTS[*]}")
	(( TEST_RANGE-- ))
	test $(( $TEST_RANGE % 1000 == 0 )) -eq 1 && echo "1000 tests done"
done

rm .err_file

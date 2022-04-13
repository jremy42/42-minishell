#!/bin/bash

AVAILABLE_INPUTS=("&&" "||" "|" "&" "(" ")" "ls" "lso")
SIZE=${#AVAILABLE_INPUTS[*]}
MAX=$(( $RANDOM % 15 ))
TEST_RANGE=50

function create_input ()
{
unset INPUTS
for i in $(seq 1 $MAX)
do
	INPUTS+=( ${AVAILABLE_INPUTS[$(( RANDOM % $SIZE ))]} )
done
##echo ${INPUTS[*]}
}

while test "$TEST_RANGE" -ge 0
do
	create_input
	echo ${INPUTS[*]} | ./minishell 1> /dev/null 2> /dev/null
	test $? -gt 2 && (echo -e "\x1b[31mKO\x1b[0m" "input : ${INPUTS[*]}")
	(( TEST_RANGE-- ))
done

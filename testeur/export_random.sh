#!/bin/bash

AVAILABLE_INPUTS=("+" "=" "\$a" "\$b" "a" "b")
SIZE=${#AVAILABLE_INPUTS[*]}
TEST_RANGE=100

function create_input ()
{
unset INPUTS
unset MAX
MAX=$(( $RANDOM % 20 ))
for i in $(seq 1 $MAX)
do
	INPUTS="${INPUTS}${AVAILABLE_INPUTS[$(( RANDOM % $SIZE ))]}"
done
#echo $INPUTS
}

while test "$TEST_RANGE" -ge 0
do
	create_input
	echo "export $INPUTS ; echo \$a ; echo \$b"
	(( TEST_RANGE-- ))
done

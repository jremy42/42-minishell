ret=0
while test ${ret} -lt 128    
do
cat /dev/urandom | head -1 | tee input_crash | ./minishell #2>> output_minishell_error #1>> output_minishell
ret=$(echo $?)
done

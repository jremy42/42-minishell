while test ${ret} -lt 128    
do
cat /dev/urandom | head -2 | tee input_crash | ./minishell
ret=$(echo $?)
done


#!/bin/bash

DIF=""
DIF_STDOUT=""
RET_MINI=""
RET_BASH=""

function test_str ()
{
	echo -e "Testing"  "'$1'"
	echo -e "$1" | $CMD ./minishell > output_file_minishell 2>output_file_minishell_error
	RET_MINI=$?
	echo -e "$1" | $CMD bash --posix > output_file_bash 2>output_file_bash_error
	RET_BASH=$?
	DIF=$(diff output_file_minishell output_file_bash)
	test -z "$DIF" && echo -e "\x1b[1;32mOK output\x1b[0m" || echo -e "\x1b[1;31mKO output stdout\x1b[0m" "\n" "$DIF"
	test "$RET_MINI" -eq "$RET_BASH" && echo -e "\x1b[1;32mOK return value\x1b[0m" || echo -e "\x1b[1;31mKO return value\x1b[0m" "\n" "Minishell $RET_MINI | Bash $RET_BASH"
	DIF_STDOUT=$(diff output_file_minishell_error output_file_bash_error)
	test -z "$DIF_STDOUT" && echo -e "\x1b[1;32mNO diff output error \x1b[0m"\
	|| (echo -e "\x1b[1;33moutput stdout Minishell : $(cat output_file_minishell_error)\x1b[0m" && echo -e "output stdout Bash : $(cat output_file_bash_error)")
	echo -en "\n"
}

##### TEST echo ####
#
#test_str 'echo toto'
#test_str 'echo -n toto'
#test_str 'echo -nnnnnn toto'
#test_str 'echo -n -ntoto'
#test_str 'echo -n -f toto'
#test_str 'echo -n toto -n'
#test_str 'echo toto -n'
#
##### TEST export ####
#
#test_str 'export a="1 2 3"; echo $a 4 5 6'
#test_str 'export 8="12"'
#test_str 'export fred=toto 8=12 jon=tutu'
#
#### TEST CD TOKI ####

#test_str "cd"
#test_str "cd a a"
#test_str "cd ../../../../../../../..\npwd"
#test_str "cd /mnt/nfs/homes/\$USER\npwd"
#test_str "cd \$HOME/Desktop"
#test_str "unset HOME\ncd"
#test_str "export HOME=\ncd"
#test_str "cd too many arguments"
#test_str "cd ./path_not_found"
#test_str "cd ..\ncd -\npwd"
#test_str "cd ..\nunset OLDPWD\ncd -"
#test_str "cd ..\nunset OLDPWD\ncd -\npwd"
#test_str "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd .."
#test_str "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd ..\npwd"
#test_str "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd ..\nunset PWD\npwd"
#test_str "export CDPATH=/mnt/nfs\ncd homes/..\npwd"
#test_str "export CDPATH=/mnt/nfs/\ncd homes/..\npwd"
#test_str "pwd\nexport CDPATH=\ncd .\npwd"
#test_str "pwd\nexport CDPATH=\ncd ..\npwd"
#test_str "unset PATH\ncd"
#
#CMD="env -i"
#test_str "cd"
#test_str "cd a a"
#test_str "cd ../../../../../../../..\npwd"
#test_str "cd /mnt/nfs/homes/\$USER\npwd"
#test_str "cd \$HOME/Desktop"
#test_str "unset HOME\ncd"
#test_str "export HOME=\ncd"
#test_str "cd too many arguments"
#test_str "cd ./path_not_found"
#test_str "cd ..\ncd -\npwd"
#test_str "cd ..\nunset OLDPWD\ncd -"
#test_str "cd ..\nunset OLDPWD\ncd -\npwd"
#test_str "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd .."
#test_str "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd ..\npwd"
#test_str "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd ..\nunset PWD\npwd"
#test_str "export CDPATH=/mnt/nfs\ncd homes/..\npwd"
#test_str "export CDPATH=/mnt/nfs/\ncd homes/..\npwd"
#test_str "pwd\nexport CDPATH=\ncd .\npwd"
#test_str "pwd\nexport CDPATH=\ncd ..\npwd"
#test_str "unset PATH\ncd"
#unset CMD

test_str "echo"
test_str "echo a b"
test_str "echo -n"
test_str "echo -n a"
test_str "echo a -n"
test_str "echo -nn a"
test_str "echo -nnnm"
test_str "echo -nnna"
test_str "echo -nnn -n -a"
test_str "echo -nn -a a -nn"
test_str "echo a\necho b"
test_str "echo -n a\necho b"
test_str "unset PATH\necho"
test_str "echo \$?"
test_str "echo \$HOME?"
test_str "echo \$?a"
test_str "echo \$??"
test_str "echo $ ?"
test_str "echo $^"
test_str "echo \"\'a b c\'\""
test_str "echo \'\"a b c\"\'"
test_str "echo \$\"\""
test_str "echo \$\'\'"
test_str "echo \"\"\$"
test_str "echo \'\'\$"
test_str "echo \'\$\'"
test_str "echo \"\$\""
test_str "echo \"  \$  \""
test_str "echo \'  \$  \'"
test_str "echo \$9"
test_str "echo \$ \$q"
test_str "echo \$b \$a \$s"
test_str "echo \"|\" ls"
test_str "echo \'|\' ls"
test_str "echo \" |\" ls"
test_str "echo \' |\' ls"
test_str "echo \"| \" ls"
test_str "echo \'| \' ls"
test_str "echo \"|\"ls"
test_str "echo \'|\'ls"
test_str "ec\"\"ho test"
test_str "ec\'\'ho test"
test_str "\"\"echo test"
test_str "\'\'echo test"
test_str "echo\"\" test"
test_str "echo\'\' test"
test_str "\"\" echo test"
test_str "\'\' echo test"
test_str "echo \"\" test"
test_str "echo \'\' test"
test_str "echo \"\""
test_str "echo \"\"\"\""
test_str "echo \"\" \"\""
test_str "echo \"\" \"\" \"\""
test_str "echo \'\'\'\'"
test_str "echo \'\' \'\'"
test_str "echo \'\' \'\' \'\'"
test_str "echo test\"\"test"
test_str "echo test\'\'test"
test_str "echo test\"\'\"test"
test_str "echo test\'\"\'test"
test_str "echo test$<test"
test_str "echo \"test$<test\""
test_str "echo \'test$<test\'"
test_str "echo te\"st$<test\""
test_str "echo te\'st$<test\'"

rm -rf output_file_bash	output_file_minishell output_file_minishell_error output_file_bash_error

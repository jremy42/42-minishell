#!/bin/bash

DIF=""
DIF_STDOUT=""
RET_MINI=""
RET_BASH=""

ALL_PARAM="$@"

function is_active ()
{
	for i in $ALL_PARAM 
	do
		if test "$i" = "$1" || test "$i" == "all"
		then
			return 0
		fi
	done
	return 1
}

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


#### TEST echo ####

if is_active "echo"
then

	test_str 'echo toto'
	test_str 'echo -n toto'
	test_str 'echo -nnnnnn toto'
	test_str 'echo -n -ntoto'
	test_str 'echo -n -f toto'
	test_str 'echo -n toto -n'
	test_str 'echo toto -n'
fi

#### TEST export ####

if is_active "export"
then

	test_str 'export a="1 2 3"; echo $a 4 5 6'
	test_str 'export 8="12"'
	test_str 'export fred=toto 8=12 jon=tutu'

fi

#### TEST quotes ####

if is_active "quote"
then

	test_str 'echo "$""$"'
	test_str 'echo $""'
	test_str 'echo "$"'
	test_str 'echo "\\\"'
fi

### TEST CD TOKI ####

if is_active "cd-toki"
then

test_str "cd"
test_str "cd a a"
test_str "cd ../../../../../../../..\npwd"
test_str "cd /mnt/nfs/homes/\$USER\npwd"
test_str "cd \$HOME/Desktop"
test_str "unset HOME\ncd"
test_str "export HOME=\ncd"
test_str "cd too many arguments"
test_str "cd ./path_not_found"
test_str "cd ..\ncd -\npwd"
test_str "cd ..\nunset OLDPWD\ncd -"
test_str "cd ..\nunset OLDPWD\ncd -\npwd"
test_str "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd .."
test_str "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd ..\npwd"
test_str "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd ..\nunset PWD\npwd"
test_str "export CDPATH=/mnt/nfs\ncd homes/..\npwd"
test_str "export CDPATH=/mnt/nfs/\ncd homes/..\npwd"
test_str "pwd\nexport CDPATH=\ncd .\npwd"
test_str "pwd\nexport CDPATH=\ncd ..\npwd"
test_str "unset PATH\ncd"

CMD="env -i"
test_str "cd"
test_str "cd a a"
test_str "cd ../../../../../../../..\npwd"
test_str "cd /mnt/nfs/homes/\$USER\npwd"
test_str "cd \$HOME/Desktop"
test_str "unset HOME\ncd"
test_str "export HOME=\ncd"
test_str "cd too many arguments"
test_str "cd ./path_not_found"
test_str "cd ..\ncd -\npwd"
test_str "cd ..\nunset OLDPWD\ncd -"
test_str "cd ..\nunset OLDPWD\ncd -\npwd"
test_str "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd .."
test_str "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd ..\npwd"
test_str "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd ..\nunset PWD\npwd"
test_str "export CDPATH=/mnt/nfs\ncd homes/..\npwd"
test_str "export CDPATH=/mnt/nfs/\ncd homes/..\npwd"
test_str "pwd\nexport CDPATH=\ncd .\npwd"
test_str "pwd\nexport CDPATH=\ncd ..\npwd"
test_str "unset PATH\ncd"
unset CMD

fi

if is_active "echo-toki"
then

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
fi

if is_active "export-toki"
then

test_str "env"
test_str "env\nexport ABC\nenv"
test_str "env\nexport ABC=\nenv"
test_str "env\nexport ABC=4\nenv"
test_str "env\nexport\nenv"
CMD="env -i"
test_str "env"
test_str "env\nexport ABC\nenv"
test_str "env\nexport ABC=\nenv"
test_str "env\nexport ABC=4\nenv"
test_str "unset PWD SHLVL _\nenv"
test_str "env\nexport\nenv"
unset CMD
fi

if is_active "exit-toki"
then

test_str "exit 1986416"
test_str "exit 2147483647"
test_str "exit 2147483648"
test_str "exit -2147483648"
test_str "exit -2147483649"
test_str "exit 9223372036854775807"
test_str "exit 9223372036854775808"
test_str "exit -9223372036854775808"
test_str "exit -9223372036854775809"
test_str "exit 9999999999999999999999999999"
test_str "unset PATH\nexit"
fi

if is_active "export-toki"
then

test_str "export var=a\nexport \$var=test\necho \$var \$a"
test_str "export \$var=test"
test_str "export ABC=4\nexport ABC\necho \$ABC"
test_str "export ABC\necho \$ABC"
test_str "export ABC\nexport ABC=DEF\necho \$ABC"
test_str "export A=\$HOME\n echo \$A"
test_str "export var\nexport var=q\nexport"
test_str "export \"'\" test=a"
test_str "export \"'\" test=a\nenv"
test_str "unset PATH\nexport"
test_str "export 0A 1A"
test_str "export 0B=4 1C=7"
test_str "export _ABC=17 _DEF=18"
test_str "export _A0=17 _B1=97"
test_str "export _0A _1B"
test_str "export 0A _1A\nexport"
test_str "export 0B=4 1C=7\nexport"
test_str "export _ABC=17 _DEF=18\nexport"
test_str "export _A0=17 _B1=97\nexport"
test_str "export _0A _1B\nexport"
test_str "export var=\"abc \"\nenv | grep var"
test_str "export var=\" abc\"\nenv | grep var"

CMD="env -i"
test_str "export"
test_str "export var=a\nexport \$var=test\necho \$var \$a"
test_str "export \$var=test"
test_str "export ABC=4\nexport ABC\necho \$ABC"
test_str "export ABC\necho \$ABC"
test_str "export ABC\nexport ABC=DEF\necho \$ABC"
test_str "export A=\$HOME\n echo \$A"
test_str "export var\nexport var=q\nexport"
test_str "export \"'\" test=a"
test_str "export \"'\" test=a\nenv"
test_str "unset PATH\nexport"
test_str "export 0A 1A"
test_str "export 0B=4 1C=7"
test_str "export _ABC=17 _DEF=18"
test_str "export _A0=17 _B1=97"
test_str "export _0A _1B"
unset CMD
fi

if is_active "unset-toki"
then
test_str "unset"
test_str "unset a b c d"
test_str "unset PATH\necho \$PATH"
test_str "unset PATH\nls"
test_str "unset \"'\" test"
test_str "unset ="
test_str "unset PWD\necho \$PWD"
test_str "export var=1\nexport var1=2\nunset var\nenv"
CMD="env -i"
test_str "unset"
test_str "unset a b c d"
test_str "unset PATH\necho \$PATH"
test_str "unset PATH\nls"
test_str "unset \"'\" test"
test_str "unset ="
test_str "unset PWD\necho \$PWD"
test_str "export var=1\nexport var1=2\nunset var\nenv"

unset CMD
fi

if is_active "quote-toki"
then
test_str "echo \$PATH"
test_str "echo \$PATH\$ q"
test_str "echo \$QBC"
test_str "echo \$?"
test_str "dzdq\necho \$?"
test_str "echo \$HOME\nunset HOME\necho \$HOME"
test_str "echo \$ \$q"
test_str "echo \$"
test_str "echo \$   \$a"
test_str "echo \$é"
test_str "echo \$é \$q"
test_str "echo \$abc f\$PWD"
test_str "export ABC=echo\n\$ABC a"
test_str "export ABC=cho\ne\$ABC b"
test_str "echo \$HOME?"
test_str "echo \$?a"
test_str "echo \$??"
test_str "echo \$PWD=22"
test_str "echo $:$= | cat -e"
test_str "echo \$USER\$var\$USER\$USER\$USERtest\$USER"
test_str "$"
test_str "echo \"   $   \" | cat -e"
test_str "\$LESS\$VAR"
test_str ""
test_str "\$bla"
test_str "echo \$var bonjour"
test_str "export test=\"    a    b   \"\necho ab\$test"
test_str "export test=\"    a    b   \"\necho \"ab\"\$test"
test_str "export test=\"    a    b   \"\necho ab\"\$test\""
test_str "export test=\"    a    b   \"\necho ab\"\"\$test\"\""
test_str "export test=\"    a    b   \"\necho ab\"\"\'\$test\"\"\'"
test_str "export var=at\nc\$var test_manuel"
test_str "export var=test\$var\necho \$var"
test_str "export var=\"a b\"\n> \$var"
test_str "export var=\"a b\"\n> \$\"var\""
test_str "export var=\"a b\"\n> \$var >hey\ncat hey\nrm hey"
test_str "export var=\"a b\"\n>hey > \$var\ncat hey\nrm hey"

test_str "export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8 && echo \"\$A\'\$B\"\'\$C\"\$D\'\$E\'\"\$F\"\'\"\'\$G\'\$H\""

test_str "export FOO=' \" ' && echo \$FOO"
test_str "export FOO=' \" ' && echo \"\$FOO\""
test_str "export FOO=' \" ' && echo \"\$FOO \""
test_str "export FOO=' \" ' && echo \" \$FOO\""
test_str "export FOO=' \" ' && echo \" \$FOO \""
test_str "export FOO=' \" ' && echo \'\$FOO\'"
test_str "export FOO=' \" ' && echo \'\$FOO \'"
test_str "export FOO=' \" ' && echo \' \$FOO\'"
test_str "export FOO=' \" ' && echo \' \$FOO \'"

test_str "export BAR=\" \' \" && echo \$BAR"
test_str "export BAR=\" \' \" && echo \"\$BAR\""
test_str "export BAR=\" \' \" && echo \"\$BAR \""
test_str "export BAR=\" \' \" && echo \" \$BAR\""
test_str "export BAR=\" \' \" && echo \" \$BAR \""
test_str "export BAR=\" \' \" && echo \'\$BAR\'"
test_str "export BAR=\" \' \" && echo \'\$BAR \'"
test_str "export BAR=\" \' \" && echo \' \$BAR\'"
test_str "export BAR=\" \' \" && echo \' \$BAR \'"


test_str "echo \$1"
test_str "echo test\$1"
test_str "echo \$does_not_exist"
test_str "echo test\$does_not_exist"
test_str "echo test\$does_not_existtest"
test_str "echo \$NULL"
test_str "echo \$USER"
test_str "echo \$USER_not_exist"

test_str "echo \'test\$-r\'"
test_str "echo \'test\$-?\'"
test_str "echo \$USER\$USER"
test_str "echo \$USER\"\"\$USER"
test_str "echo \$USER\" \"\$USER"
test_str "echo \$USER\"'\"\$USER"
test_str "echo \$USER\"''\"\$USER"
test_str "echo \$USER'\"'\$USER"
test_str "echo \$USER'\"\"'\$USER"
test_str "echo \$USE\"\"R"
test_str "echo \$USE\'\'R"
test_str "echo \$USER\"\""
test_str "echo \$USER\'\'"
test_str "echo \$USER\"\"TOTO"
test_str "echo \$USER\'\'TOTO"
test_str "echo \$USER \"\""
test_str "echo \$USER \'\'"
test_str "echo \"\"\$USE\"\"R"
test_str "echo \'\'\$USE\'\'R"
test_str "echo \"\"\$USER"
test_str "echo \'\'\$USER"
test_str "echo ''\$USER"
test_str "echo \$\"\"USER"
test_str "echo \$\'\'USER"
test_str "echo \$\"USER\""
test_str "echo \$\'USER\'"
test_str "echo \"\"'\$USER\"\"'"
test_str "echo \'\'\"\$USER\'\'\""

CMD="env -i"
test_str "echo \$QBC"
test_str "echo \$?"
test_str "dzdq\necho \$?"
test_str "echo \$HOME\nunset HOME\necho \$HOME"
test_str "echo \$ \$q"
test_str "echo \$"
test_str "echo \$   \$a"
test_str "echo \$é"
test_str "echo \$é \$q"
test_str "echo \$abc f\$PWD"
test_str "export ABC=echo\n\$ABC a"
test_str "export ABC=cho\ne\$ABC b"
test_str "echo \$HOME?"
test_str "echo \$?a"
test_str "echo \$??"
test_str "echo \$USER\$var\$USER\$USER\$USERtest\$USER"
test_str "$"
test_str "\$LESS\$VAR"
test_str ""
test_str "\$bla"
test_str "echo \$var bonjour"
test_str "export test=\"    a    b   \"\necho ab\$test"
test_str "export test=\"    a    b   \"\necho \"ab\"\$test"
test_str "export test=\"    a    b   \"\necho ab\"\$test\""
test_str "export test=\"    a    b   \"\necho ab\"\"\$test\"\""
test_str "export test=\"    a    b   \"\necho ab\"\"\'\$test\"\"\'"
test_str "export var=test\$var\necho \$var"
unset CMD
fi

rm -rf output_file_bash	output_file_minishell output_file_minishell_error output_file_bash_error

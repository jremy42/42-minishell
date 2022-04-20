#!/bin/bash

AVAILABLE_INPUTS=("+" "=" "\$a" "\$b" "a" "b")
SIZE=${#AVAILABLE_INPUTS[*]}
TEST_RANGE=30

function create_input ()
{
unset INPUTS
unset MAX
MAX=$(( 1 + $RANDOM % 20 ))
for i in $(seq 1 $MAX)
do
	INPUTS="${INPUTS}${AVAILABLE_INPUTS[$(( RANDOM % $SIZE ))]}"
done
}

DIF=""
DIF_STDOUT=""
RET_MINI=""
RET_BASH=""
TEST_NUMBER="0"
TEST_KO_OUTPUT="0"
TEST_KO_RET="0"
TEST_KO_LEAKS="0"
ALL_PARAM="$@"
TEST_DIR="test_dir_to_delete/"
SUB_DIR_TEST="subdir"
#VALGRIND="valgrind --log-file=.leak --leak-check=full --track-fds=yes --show-leak-kinds=all --undef-value-errors=no --error-exitcode=240 --errors-for-leak-kinds=all --suppressions=.ignore_readline" 

mkdir "$TEST_DIR"
mkdir "tmp"
mkdir "$SUB_DIR_TEST"
mkdir "${SUB_DIR_TEST}/${TEST_DIR}"

for i in abc  abcd  abcde  def  efg
do
	touch "${SUB_DIR_TEST}/$i"
done
cp ./minishell $SUB_DIR_TEST

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
	(( TEST_NUMBER++ ))
	echo -e "Testing"  "'$1'" "ref : $TEST_NUMBER"
	if test -z "$2"
	then
		echo -e "$1" | $CMD ./minishell > ${TEST_DIR}.output_file_minishell 2> ${TEST_DIR}.output_file_minishell_error
	else
		$CMD ./minishell -c "$1" > ${TEST_DIR}.output_file_minishell 2> ${TEST_DIR}.output_file_minishell_error
	fi
	RET_MINI=$?
	if test -z "$2"
	then
		echo -e "$1" | $CMD bash --posix > ${TEST_DIR}.output_file_bash 2> ${TEST_DIR}.output_file_bash_error
	else
		$CMD bash --posix -c "$1" > ${TEST_DIR}.output_file_bash 2> ${TEST_DIR}.output_file_bash_error
	fi
	RET_BASH=$?
	DIF=$(diff ${TEST_DIR}.output_file_minishell ${TEST_DIR}.output_file_bash)
	test -z "$DIF" && echo -e "\x1b[1;32mOK output\x1b[0m" || echo -e "\x1b[1;31mKO output stdout\x1b[0m" "\n" "$DIF"
	test -z "$DIF" || (( TEST_KO_OUTPUT++ ))
	test "$RET_MINI" -eq "$RET_BASH" && echo -e "\x1b[1;32mOK return value\x1b[0m" || echo -e "\x1b[1;31mKO return value\x1b[0m" "\n" "Minishell $RET_MINI | Bash $RET_BASH"
	test "$RET_MINI" -eq "$RET_BASH" || (( TEST_KO_RET++ ))
	DIF_STDOUT=$(diff ${TEST_DIR}.output_file_minishell_error ${TEST_DIR}.output_file_bash_error)
	test -z "$DIF_STDOUT" && echo -e "\x1b[1;32mNO diff output error \x1b[0m"\
	|| (echo -e "\x1b[1;33moutput stdout Minishell : $(cat ${TEST_DIR}.output_file_minishell_error)\x1b[0m" && echo -e "output stdout Bash : $(cat ${TEST_DIR}.output_file_bash_error)")
	echo -en "\n"
	$VALGRIND $CMD ./minishell < <(echo -e "$1") > /dev/null 2> /dev/null
	if [  "$?" -ne "240" ]
	then
		echo -e "valgrind     : \033[32m[OK]\033[0m"
	else
		(( TEST_KO_LEAKS++ ))
		echo -e "valgrind     : \033[31m[NOK]\033[0m"
	cat .leak
	fi
	rm -rf .leak
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
	test_str 'export a="\\"'
	test_str 'export a="\\\\"; echo "$a"'
	test_str 'export a="\\\\"; echo $a'
fi

#### TEST HD ####

if is_active "hd"
then

	test_str '<< toto\ntoto\n'
fi

### TEST EMPTY_VAR ####

if is_active "empty"
then

	test_str 'echo a > $a'
	test_str '$var'
	test_str 'echo toto | $var'
	test_str 'echo toto | $var | echo ici'
	test_str 'echo toto | $var | cat'
	test_str 'echo toto | $var $var | echo ici'
	test_str 'echo toto | "$var" | cat'
	test_str '$var $var'
	test_str '$var echo toto'
	test_str "'\$var'"
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
CMD="env -i"
test_str "unset"
test_str "unset a b c d"
test_str "unset PATH\necho \$PATH"
test_str "unset PATH\nls"
test_str "unset \"'\" test"
test_str "unset ="
test_str "unset PWD\necho \$PWD"

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

if is_active "delslash-toki"
then
test_str "echo \" a \"bc\" def ghij\"klmno\" \"pqrstuv\"wxyz0123 456789abc\"defghijklm\"nopqrstuvwxyz \""
fi

if is_active "parsing-toki"
then
	test_str "\"\""
	test_str ".."
	test_str "\"\"abc\"\""
	test_str "\"\'abc\'\""
	test_str "\'\"abc\"\'"
	test_str "\'  \"abc\"  \'"
	test_str "\"  \'abc\'  \""
	test_str "\'\"abc\"\'"
	test_str "ls  \"\""
	test_str "\"ls \""
	test_str "\"ls\""
	test_str "\" ls\""
	test_str "\" ls \""
	test_str "l\"s\""
	test_str "l\"s \""
	test_str "\"l\"s"
	test_str "\" l\"s"
	test_str
	test_str "ls -la"
	test_str "ls -l"
	test_str "ls \"\"-la"
	test_str "ls ''-la"
	test_str "ls \"\" -la"
	test_str "ls '' -la"
	test_str "ls\"\" -la"
	test_str "ls'' -la"
	test_str "ls \"\"\"\" -la"
	test_str "ls '''' -la"
	test_str "ls -\"la\""
	test_str "ls \"-la\""  
	test_str "ls \"-l\"a"  
	test_str "ls -'la'"    
	test_str "ls '-la'"    
	test_str "ls '-l'a"    
	test_str
	test_str "ls \"   '   \""
	test_str "ls \"   '\""
	test_str "ls \"'   \""
	test_str "ls \"'\""
	test_str "ls '\"'"
	test_str "ls '\"  '"
	test_str "ls '  \"'"
	test_str "ls '   \"  '"
	test_str "echo \"\" bonjour"
	test_str "   echo         bonjour    "
	test_str "echo test | cat"
	test_str "echo test ||| cat"
	test_str "export \"\""
	test_str "unset \"\""
	test_str "export \"test=ici\"=coucou\necho \$test"
	test_str "export var=\"cat Makefile | grep >\"\necho \$var"
	test_str "echo \"\$test\"\"Makefile\""
	test_str "echo \"\$test\"Makefile"
	test_str "export test=\"   a    b    \"\necho \$test"

fi

if is_active "pipe-toki"
then
	test_str "ls | sort"
	test_str "ls|ls |ls | ls| ls |                ls"
	test_str "ls | sort | grep i | wc -l"
	test_str "sleep 0.1 | ls"
	test_str "cat Makefile | grep pr | head -n 5 | cd file_not_exist"
	test_str "cat Makefile | grep pr | head -n 5 | hello"
	test_str "ls | exit"
	test_str "sleep 0.1 | exit"
	test_str "echo bip | bip\necho coyotte ><"
	test_str "ls | ls | ls | ls | ls /proc/self/fd"
	cd $SUB_DIR_TEST
	test_str "cat a | < abc cat | cat > c | cat\nrm c"
	cd ..
fi

if is_active "star-toki"
then
	cd $SUB_DIR_TEST
	test_str "echo *"
	test_str "echo .*"
	test_str "echo *a"
	test_str "echo a*"
	test_str "echo a*bc"
	test_str	"export TMP=*\nls | grep \$TMP"
	test_str	"export TMP=*\ncd ..\nls | grep \$TMP"
	test_str	"echo ****a*****b****c****d****"
	test_str "echo a"
	test_str "*"
	test_str "mkdir t1\ncd t1\nrm -rf ../t1\necho *"
	test_str "mkdir t1\ncd t1\nrm -rf ../t1\necho .*"
	test_str "echo \"ab\"*"
	test_str "echo \"a \"*"
	test_str "echo \'ab\'*"
	test_str "echo \'a \'*"
	test_str "echo a\"b\"*"
	test_str "echo a\" b\"*"
	test_str "echo *\"b\""
	test_str "echo *\"b\"*"
	cd ..
fi

if is_active "hd-toki"
then
	cd $SUB_DIR_TEST
	test_str "cat << end\nabc\nend"
	test_str "cat << end <<start\nabc\nstart\nend\nend\ndef\nstart"
	test_str "cat << end\nEnd\neNd\nenD\nENd\nEnD\neND\nEND\nend"
	test_str "<< end cat\nabc\nend"
	test_str "<< end <<start cat\nabc\nstart\nend\nend\ndef\nstart"
	test_str "<<end cat\nEnd\neNd\nenD\nENd\nEnD\neND\nEND\nend"
	test_str "<<end cat <<start abc\nStart\nstart\nend\ndef\n\nae\nend\nstart"
	test_str "<<end cat <<start abc def << x\na1\nend\na2\nstart\na3\nx"
	test_str "cat << eof\n\$USER\n\"\$USER\"\neof\n"
	test_str "cat << \"eof\"\n\$USER\n\"\$USER\"\neof\n"
	cd ..
fi

if is_active "redir-toki"
then
	cd $SUB_DIR_TEST

	test_str "<abc cat <abcd <def"
	test_str "<abc cat <doesntexist < dev"
	test_str "> test | echo blabla\nrm test"
	test_str ">a cat <b >>c\ncat a c\nrm a c"
	test_str ">a ls <b >>c >d\ncat a c d\nrm a c d"
	test_str ">a\ncat a\nrm a"
	test_str "cat -e > a < b\ncat a\nrm a"
	test_str "cat < a"
	test_str "echo 2 > a >> b\ncat a b\nrm a b"
	test_str "echo 2 >> a > b\ncat a b\nrm a b"
	test_str "echo a > b c\necho b\nrm a b c\n"
	test_str "not_cmd a > b\ncat b\nrm b"
	test_str "echo a > \$a"
	test_str "cat < abc < abcd < abcde > t1 > t2 << end >> t3 << t4\na1\nend\na2\nt4\ncat t1 t2 t3\n rm t1 t2 t3"

	cd ../
fi

if is_active "and-toki"
then
	test_str "ls && uname"
	test_str "ls && cat fesfjseoi && pwd"
	test_str "ls && ls -a && ls -l && ls -la && ls -fesfs && ls"
	test_str "ls | grep a && ls"
	test_str "ls | grep z && ls"
	test_str "ls && sleep 0.1 && pwd"
	test_str "sleep 0.1 && ls && sleep 0.1 && pwd"
	test_str "sleep -1 && ls"
	test_str "ls -fsenifs && ls -fseifkse && ls && ls"
fi

if is_active "or-toki"
then
	test_str "ls || uname"
	test_str "uname || cat doesnotexist"
	test_str "cat doesnotexist || uname"
	test_str "cat doesnotexist || cat doesnotexist || cat doesnotexist || uname"
	test_str "cat doesnotexist || uname || cat doesnotexist || uname"
	test_str "touch toto\n ls | grep toto || uname\n rm toto"
	test_str "ls | grep toto || uname"
	test_str "cat doesnotexist | ls || uname"
	test_str "cat doesnotexist | cat stillnotexist || uname"
	test_str "touch toto\n uname || ls | grep toto \nrm toto"
fi

if is_active "andor-toki"
then
	test_str "echo a && echo b || echo c"
	test_str "echo a || echo b && echo c"
	test_str "cat toto && echo b || echo c"
	test_str "cat toto || echo b && echo c"
	test_str "echo a && cat toto || echo c"
	test_str "echo a || cat toto && echo c"
	test_str "echo a && echo b || cat toto"
	test_str "echo a || echo b && cat toto"
	test_str "echo a || echo b || echo c && echo d"
	test_str "echo a || echo b && echo c || echo d"
	test_str "echo a && echo b || echo c || echo d"
	test_str "cat toto || echo b || echo c && echo d"
	test_str "cat toto || echo b && echo c || echo d"
	test_str "cat toto && echo b || echo c || echo d"
	test_str "echo a || cat toto || echo c && echo d"
	test_str "echo a || cat toto && echo c || echo d"
	test_str "echo a && cat toto || echo c || echo d"
	test_str "echo a || echo b || cat toto && echo d"
	test_str "echo a || echo b && cat toto || echo d"
	test_str "echo a && echo b || cat toto || echo d"
	test_str "echo a || echo b || echo c && cat toto"
	test_str "echo a || echo b && echo c || cat toto"
	test_str "echo a && echo b || echo c || cat toto"
	test_str "echo a || echo b && echo c && echo d"
	test_str "echo a && echo b || echo c && echo d"
	test_str "echo a && echo b && echo c || echo d"
	test_str "cat toto || echo b && echo c && echo d"
	test_str "cat toto && echo b || echo c && echo d"
	test_str "cat toto && echo b && echo c || echo d"
	test_str "echo a || cat toto && echo c && echo d"
	test_str "echo a && cat toto || echo c && echo d"
	test_str "echo a && cat toto && echo c || echo d"
	test_str "echo a || echo b && cat toto && echo d"
	test_str "echo a && echo b || cat toto && echo d"
	test_str "echo a && echo b && cat toto || echo d"
	test_str "echo a || echo b && echo c && cat toto"
	test_str "echo a && echo b || echo c && cat toto"
	test_str "echo a && echo b && echo c || cat toto"
fi

if is_active "sub-toki"
then
	test_str "(ls && pwd || uname)"
	test_str "(ls && (ls && (ls && (ls && (ls && ls)))))"
	test_str "ls -fesgse && (uname | cat) || pwd"
	test_str "ls && (uname | cat) || pwd"
	test_str "ls -fsfdfds || (uname | cat) && pwd"
	test_str "ls || (uname | cat) && pwd"
fi

if is_active "fix"
then
	test_str "export toto=12 ; export to=32 ; export | grep -v _="
	test_str "export toto; export toto=tata ; export | grep -v _="
	test_str "export he hehe ha haha ; export | grep -v _="
	test_str "exit yeye 456"
	test_str "unset ye="
	test_str "export CDPATH=/tmp ; cd . ; pwd"
	test_str "export CDPATH=/tmp ; cd .. ; pwd"
	test_str "export CDPATH=/tmp ; cd .. ; pwd"
	test_str "unset PATH ; ls"
	test_str "export CDPATHT=/ ; cd tmp ; pwd"
	test_str "export PATHT=\$PATH ; unset PATH ; ls"
	test_str "unset 1a"
	test_str "unset a+"
	test_str "unset a="
	test_str "unset _"
	test_str "unset a1a"
	test_str "not_command ; \$a"
	CMD="env -i"
	test_str "ls"
	unset CMD
	test_str "export a+= ; export | grep -v _="
	test_str "export a+=toto ; export | grep -v _="
	test_str "unset a ; export a+=toto ; export | grep -v _="
	test_str "unset a ; export a+= ;export a +=fred ; export | grep -v _="
	
	test_str "echo \$SHLVL"
	export SHLVL=999
	test_str "echo \$SHLVL"
	export SHLVL=1000
	test_str "echo \$SHLVL"
	unset SHLVL
	test_str "echo \$SHLVL"

	test_str "| a"
	test_str "&& a"
	test_str "|| a"
	test_str "|| &&"
	test_str "( )"
	test_str "(( )"
	test_str "()( ))"
	test_str "()( )()"
	test_str "()( &&  )()"
	test_str "()( ||  )()"
	test_str "(&&)"
	test_str "&&||"
	test_str "(&&)"
	test_str "||&&||"
	test_str "(|) && ls"
	test_str "| && ls"
	test_str "(|&&|) && ls"
	test_str "(|)(|)(|)"
	test_str "ls (|) ls"
	test_str "(ls|) && ls"
fi

if is_active "fix2"
then
	test_str "&& lso"
	test_str "(|ls) && ls"
	test_str "(cat <<)"
	test_str "echo toto && echo tata; echo titi ;"
	test_str "export a==a+= ; export b\$a ; echo \$b"

	CMD="env -i"
	test_str "export abba ; export abba+=\$abba ; export | grep -v _="
	test_str "unset a ; export a+==a+= ; echo \$a ; export b\$a ; echo \$b ; export | grep -v _="
	unset CMD
fi

if is_active "hc-export"
then

	while test "$TEST_RANGE" -ge 0
	do
		create_input
		test_str "export a=a; export b=b; export $INPUTS ; echo \$a ; echo \$b"
		test_str "export a; export b; export $INPUTS ; echo \$a ; echo \$b"
		test_str "export a; export b=b; export $INPUTS ; echo \$a ; echo \$b"
		test_str "export a=a; export b; export $INPUTS ; echo \$a ; echo \$b"
		(( TEST_RANGE-- ))
	done
fi

if is_active "ni"
then
	test_str "echo toto" "non-interactive"
	test_str "ls | lso" "non-interactive"
fi

if is_active "fix3"
then
	test_str "exit 123 123"
	test_str "exit 123 123" "non-interactive"
	test_str "bin/"
	test_str "/bin/"
	test_str "//bin//"
	CMD="env -i"
	test_str "exit 123 123"
	test_str "exit 123 123" "non-interactive"
	test_str "bin/"
	test_str "/bin/"
	test_str "//bin//"
	unset CMD
fi

if is_active "fix4"
then
	test_str "/bin/lss"
fi

echo "Test KO RET: ${TEST_KO_RET}/${TEST_NUMBER}"
echo "Test KO OUTPUT: ${TEST_KO_OUTPUT}/${TEST_NUMBER}"
echo "Test KO LEAKS: ${TEST_KO_LEAKS}/${TEST_NUMBER}"

rm -rf $TEST_DIR $SUB_DIR_TEST tmp 'a b' var

#!/bin/bash

#CMD=--trace-children=yes --suppressions=../ignoreliberror --leak-check=full --show-leak-kinds=all

COLOR_RED="\033[0;31m"
COLOR_GREEN="\033[0;32m"
COLOR_BLUE="\033[0;34m"
COLOR_WHITE="\033[0;38m"

TIMEOUT="timeout 2s"
TIMEOUT_ERR=124
ARG=$*
DIR_TEST=dir_test/
MINISHELL_DIR=$(pwd)
MINISHELL=$MINISHELL_DIR/../minishell
LOG_ERROR=$MINISHELL_DIR"/log_error"
INDEX=0

make -C ../ > /dev/null
clear

function in_arg()
{
    if [ "$ARG" == "" ]
    then
		return 1
    fi
	is_flag=1
    for i in $ARG
    do
		for j in $*
		do
			if [ "$j" == "-v" ] || [ "$j" == "--stop" ]
			then
				is_flag=0
			fi
		    if [ "$j" == "$i" ]
	    	then
				return 1
			else
				if [ "$i" != "-v" ] && [ "$i" != "--stop" ]
				then
					is_flag=0
				fi
			fi
		done
    done
    return $is_flag
}

function test_ret_stdout()
{
    TEST_MINISHELL=$(printf "$@" | $CMD $TIMEOUT $MINISHELL 2>$LOG_ERROR)
    RET_MINISHELL=$?
	LINE_ERROR_MINISHELL=$(cat $LOG_ERROR | wc -l)
	LOG_ERROR_MINISHELL=$(cat $LOG_ERROR)
    TEST_BASH=$(printf "$@" | $CMD bash 2>$LOG_ERROR)
    RET_BASH=$?
	LINE_ERROR_BASH=$(cat $LOG_ERROR | wc -l)
	LOG_ERROR_BASH=$(cat $LOG_ERROR)

    if [ "$TEST_MINISHELL" == "$TEST_BASH" ] && [ "$RET_MINISHELL" == "$RET_BASH" ] && [ "$LINE_ERROR_MINISHELL" == "$LINE_ERROR_BASH" ]
    then
		printf $COLOR_GREEN"$INDEX:OK "
		in_arg "-v"
		if [ $? == 1 ] && [ "$ARG" != "" ]
		then
			printf $COLOR_WHITE"CMD : \n$@\n"
			printf $COLOR_BLUE
			printf "\nBash      (ret value : $RET_BASH) : \n$TEST_BASH\n"
			printf "Error message :\n\"$LOG_ERROR_BASH\"\n"
			printf $COLOR_GREEN
			printf "\nMinishell (ret value : $RET_MINISHELL) : \n$TEST_MINISHELL\n"
			printf "Error message :\n\"$LOG_ERROR_MINISHELL\"\n\n"
		fi
    else
		printf $COLOR_RED"$INDEX:KO\n"
		printf $COLOR_WHITE"CMD : \n$@\n"
		if [ $RET_MINISHELL == $TIMEOUT_ERR ] && [ $RET_MINISHELL != $RET_BASH ]
		then
			printf "time out\n"
		else
			printf $COLOR_BLUE
			printf "\nBash      (ret value : $RET_BASH) :\n$TEST_BASH\n"
			printf "Error message :\n\"$LOG_ERROR_BASH\"\n"
			printf $COLOR_RED
			printf "\nMinishell (ret value : $RET_MINISHELL) :\n$TEST_MINISHELL\n"
			printf "Error message :\n\"$LOG_ERROR_MINISHELL\"\n\n"
		fi
		in_arg "--stop"
		if [ $? == 1 ] && [ "$ARG" != "" ]
		then
			exit
		fi
    fi
    printf $COLOR_WHITE
    INDEX=$((INDEX + 1))
}

function test_env()
{
    TEST_MINISHELL=$(printf "$@" | $CMD $TIMEOUT $MINISHELL 2>$LOG_ERROR)
    LINE_MINISHELL=$(echo "$TEST_MINISHELL" | wc -l)
    RET_MINISHELL=$?
	LINE_ERROR_MINISHELL=$(cat $LOG_ERROR | wc -l)
	LOG_ERROR_MINISHELL=$(cat $LOG_ERROR)
    TEST_BASH=$(printf "$@" | $CMD bash 2>$LOG_ERROR)
    LINE_BASH=$(echo "$TEST_BASH" | wc -l)
    RET_BASH=$?
	LINE_ERROR_BASH=$(cat $LOG_ERROR | wc -l)
	LOG_ERROR_BASH=$(cat $LOG_ERROR)

    if [ "$LINE_MINISHELL" == "$LINE_BASH" ] && [ "$RET_MINISHELL" == "$RET_BASH" ] && [ "$LINE_ERROR_MINISHELL" == "$LINE_ERROR_BASH" ]
    then
		printf $COLOR_GREEN"$INDEX:OK "
		in_arg "-v"
		if [ $? == 1 ] && [ "$ARG" != "" ]
		then
			printf $COLOR_WHITE"CMD : \n$@\n"
			printf $COLOR_BLUE
			printf "\nBash      (ret value : $RET_BASH) :\n$TEST_BASH\n"
			printf "Error message :\n$(cat $LOG_ERROR_BASH)\n"
			printf $COLOR_GREEN
			printf "\nMinishell (ret value : $RET_MINISHELL) :\n$TEST_MINISHELL\n"
			printf "Error message :\n\"$(cat $LOG_ERROR_MINISHELL)\"\n\n"
		fi
    else
		printf $COLOR_RED"$INDEX:KO\n"
		printf $COLOR_WHITE"CMD : \n$@\n"
		if [ $RET_MINISHELL == $TIMEOUT_ERR ] && [ $RET_MINISHELL != $RET_BASH ]
		then
			printf "timeout\n"
		else
			printf $COLOR_BLUE
			printf "\nBash      (ret value : $RET_BASH) :\nBash n line      : $LINE_BASH\nTest bash      :\n$TEST_BASH\n"
			printf "Error message :\n\"$(cat $LOG_ERROR_BASH)\"\n"
			printf $COLOR_RED
			printf "\nMinishell (ret value : $RET_MINISHELL) :\nMinishell n line : $LINE_MINISHELL\nTest minishell :\n$TEST_MINISHELL\n"
			printf "Error message :\n\"$(cat $LOG_ERROR_MINISHELL)\"\n\n"
		fi
		in_arg "--stop"
		if [ $? == 1 ] && [ "$ARG" != "" ]
		then
			exit
		fi
    fi
    printf $COLOR_WHITE
    INDEX=$((INDEX + 1))
}
###########################################################
######################## OPTIONNEL #########################
###########################################################
in_arg "optionnel"
if [ $? == 1 ]
then
#	INDEX=0
	

	echo
	unset CMD
fi
###########################################################
############################ CD ###########################
###########################################################
in_arg "cd"
if [ $? == 1 ]
then
#    INDEX=0
    printf "***** TEST CD *****\n"
    test_ret_stdout "cd"
    test_ret_stdout "cd a a"
    test_ret_stdout "cd ../../../../../../../..\npwd"
    test_ret_stdout "cd /mnt/nfs/homes/\$USER\npwd"
    test_ret_stdout "cd \$HOME/Desktop"
    test_ret_stdout "unset HOME\ncd"
    test_ret_stdout "export HOME=\ncd"
    test_ret_stdout "cd too many arguments"
    test_ret_stdout "cd ./path_not_found"
    test_ret_stdout "cd ..\ncd -\npwd"
    test_ret_stdout "cd ..\nunset OLDPWD\ncd -"
    test_ret_stdout "cd ..\nunset OLDPWD\ncd -\npwd"
    test_ret_stdout "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd .."
    test_ret_stdout "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd ..\npwd"
    test_ret_stdout "mkdir t1\nmkdir t1/t2\ncd t1/t2\nrm -rf ../../t1\ncd ..\nunset PWD\npwd"
    test_ret_stdout "export CDPATH=/mnt/nfs\ncd homes/..\npwd"
    test_ret_stdout "export CDPATH=/mnt/nfs/\ncd homes/..\npwd"
    test_ret_stdout "pwd\nexport CDPATH=\ncd .\npwd"
    test_ret_stdout "pwd\nexport CDPATH=\ncd ..\npwd"
    test_ret_stdout "unset PATH\ncd"
    CMD="env -i"
    test_ret_stdout "cd"
    test_ret_stdout "cd a a"
    test_ret_stdout "cd ../../../../../../../..\npwd"
    test_ret_stdout "cd /mnt/nfs/homes/\$USER\npwd"
    test_ret_stdout "cd \$HOME/Desktop"
    test_ret_stdout "unset HOME\ncd"
    test_ret_stdout "export HOME=\ncd"
    test_ret_stdout "cd too many arguments"
    test_ret_stdout "cd ./path_not_found"
    test_ret_stdout "cd ..\ncd -\npwd"
    test_ret_stdout "cd ..\nunset OLDPWD\ncd -"
    test_ret_stdout "cd ..\nunset OLDPWD\ncd -\npwd"
    test_ret_stdout "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd .."
    test_ret_stdout "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd ..\npwd"
    test_ret_stdout "/bin/mkdir t1\n/bin/mkdir t1/t2\ncd t1/t2\n/bin/rm -rf ../../t1\ncd ..\nunset PWD\npwd"
    test_ret_stdout "export CDPATH=/mnt/nfs\ncd homes/..\npwd"
    test_ret_stdout "export CDPATH=/mnt/nfs/\ncd homes/..\npwd"
    test_ret_stdout "pwd\nexport CDPATH=\ncd .\npwd"
    test_ret_stdout "pwd\nexport CDPATH=\ncd ..\npwd"
    test_ret_stdout "unset PATH\ncd"
    echo
    unset CMD
fi

###########################################################
########################## ECHO ###########################
###########################################################
in_arg "echo"
if [ $? == 1 ]
then
#    INDEX=0
    printf "***** TEST ECHO *****\n"
    test_ret_stdout "echo"
    test_ret_stdout "echo a b"
    test_ret_stdout "echo -n"
    test_ret_stdout "echo -n a"
    test_ret_stdout "echo a -n"
    test_ret_stdout "echo -nn a"
    test_ret_stdout "echo -nnnm"
    test_ret_stdout "echo -nnna"
    test_ret_stdout "echo -nnn -n -a"
    test_ret_stdout "echo -nn -a a -nn"
    test_ret_stdout "echo a\necho b"
    test_ret_stdout "echo -n a\necho b"
    test_ret_stdout "unset PATH\necho"
    test_ret_stdout "echo \$?"
    test_ret_stdout "echo \$HOME?"
    test_ret_stdout "echo \$?a"
    test_ret_stdout "echo \$??"
    test_ret_stdout "echo $ ?"
    test_ret_stdout "echo $^"
    test_ret_stdout "echo \"\'a b c\'\""
    test_ret_stdout "echo \'\"a b c\"\'"
    test_ret_stdout "echo \$\"\""
    test_ret_stdout "echo \$\'\'"
    test_ret_stdout "echo \"\"\$"
    test_ret_stdout "echo \'\'\$"
    test_ret_stdout "echo \'\$\'"
    test_ret_stdout "echo \"\$\""
    test_ret_stdout "echo \"  \$  \""
    test_ret_stdout "echo \'  \$  \'"
    test_ret_stdout "echo \$9"
    test_ret_stdout "echo \$ \$q"
    test_ret_stdout "echo \$b \$a \$s"
	test_ret_stdout "echo \"|\" ls"
	test_ret_stdout "echo \'|\' ls"
	test_ret_stdout "echo \" |\" ls"
	test_ret_stdout "echo \' |\' ls"
	test_ret_stdout "echo \"| \" ls"
	test_ret_stdout "echo \'| \' ls"
	test_ret_stdout "echo \"|\"ls"
	test_ret_stdout "echo \'|\'ls"
	test_ret_stdout "ec\"\"ho test"
    test_ret_stdout "ec\'\'ho test"
    test_ret_stdout "\"\"echo test"
    test_ret_stdout "\'\'echo test"
    test_ret_stdout "echo\"\" test"
    test_ret_stdout "echo\'\' test"
    test_ret_stdout "\"\" echo test"
    test_ret_stdout "\'\' echo test"
    test_ret_stdout "echo \"\" test"
    test_ret_stdout "echo \'\' test"
   	test_ret_stdout "echo \"\""
	test_ret_stdout "echo \"\"\"\""
	test_ret_stdout "echo \"\" \"\""
	test_ret_stdout "echo \"\" \"\" \"\""
	test_ret_stdout "echo \'\'\'\'"
	test_ret_stdout "echo \'\' \'\'"
	test_ret_stdout "echo \'\' \'\' \'\'"
	test_ret_stdout "echo test\"\"test"
	test_ret_stdout "echo test\'\'test"
	test_ret_stdout "echo test\"\'\"test"
	test_ret_stdout "echo test\'\"\'test"
	test_ret_stdout "echo test$<test"
	test_ret_stdout "echo \"test$<test\""
	test_ret_stdout "echo \'test$<test\'"
	test_ret_stdout "echo te\"st$<test\""
	test_ret_stdout "echo te\'st$<test\'"
    echo
fi

###########################################################
########################## ENV ############################
###########################################################
in_arg "env"
if [ $? == 1 ]
then
#    INDEX=0
    printf "***** TEST ENV *****\n"
    test_env "env"
    test_env "env\nexport ABC\nenv"
    test_env "env\nexport ABC=\nenv"
    test_env "env\nexport ABC=4\nenv"
	test_env "env\nexport\nenv"
    CMD="env -i"
    test_env "env"
    test_env "env\nexport ABC\nenv"
    test_env "env\nexport ABC=\nenv"
    test_env "env\nexport ABC=4\nenv"
    test_env "unset PWD SHLVL _\nenv"
	test_env "env\nexport\nenv"
    echo
    unset CMD
fi

###########################################################
######################### EXIT ############################
###########################################################
in_arg "exit"
if [ $? == 1 ]
then
#    INDEX=0
    printf "***** TEST EXIT *****\n"
    for i in {-2..256}
    do 
	    test_ret_stdout "exit $i"
    done
    test_ret_stdout "exit 1986416"
    test_ret_stdout "exit 2147483647"
    test_ret_stdout "exit 2147483648"
    test_ret_stdout "exit -2147483648"
    test_ret_stdout "exit -2147483649"
    test_ret_stdout "exit 9223372036854775807"
    test_ret_stdout "exit 9223372036854775808"
    test_ret_stdout "exit -9223372036854775808"
    test_ret_stdout "exit -9223372036854775809"
    test_ret_stdout "exit 9999999999999999999999999999"
    test_ret_stdout "unset PATH\nexit"
    echo
fi

###########################################################
######################### EXPORT ##########################
###########################################################
in_arg "export"
if [ $? == 1 ]
then
#    INDEX=0
    printf "***** TEST EXPORT *****\n"
    test_ret_stdout "export var=a\nexport \$var=test\necho \$var \$a"
    test_ret_stdout "export \$var=test"
    test_ret_stdout "export ABC=4\nexport ABC\necho \$ABC"
    test_ret_stdout "export ABC\necho \$ABC"
    test_ret_stdout "export ABC\nexport ABC=DEF\necho \$ABC"
    test_ret_stdout "export A=\$HOME\n echo \$A"
    test_env "export var\nexport var=q\nexport"
    test_ret_stdout "export \"'\" test=a"
    test_env "export \"'\" test=a\nenv"
    test_env "unset PATH\nexport"
    test_ret_stdout "export 0A 1A"
    test_ret_stdout "export 0B=4 1C=7"
    test_ret_stdout "export _ABC=17 _DEF=18"
    test_ret_stdout "export _A0=17 _B1=97"
    test_ret_stdout "export _0A _1B"
    test_env "export 0A _1A\nexport"
    test_env "export 0B=4 1C=7\nexport"
    test_env "export _ABC=17 _DEF=18\nexport"
    test_env "export _A0=17 _B1=97\nexport"
    test_env "export _0A _1B\nexport"
	test_env "export var=\"abc \"\nenv | grep var"
	test_env "export var=\" abc\"\nenv | grep var"

    CMD="env -i"
    test_env "export"
    test_ret_stdout "export var=a\nexport \$var=test\necho \$var \$a"
    test_ret_stdout "export \$var=test"
    test_ret_stdout "export ABC=4\nexport ABC\necho \$ABC"
    test_ret_stdout "export ABC\necho \$ABC"
    test_ret_stdout "export ABC\nexport ABC=DEF\necho \$ABC"
    test_ret_stdout "export A=\$HOME\n echo \$A"
    test_env "export var\nexport var=q\nexport"
    test_ret_stdout "export \"'\" test=a"
    test_env "export \"'\" test=a\nenv"
    test_env "unset PATH\nexport"
	test_ret_stdout "export 0A 1A"
    test_ret_stdout "export 0B=4 1C=7"
    test_ret_stdout "export _ABC=17 _DEF=18"
    test_ret_stdout "export _A0=17 _B1=97"
    test_ret_stdout "export _0A _1B"
    echo
    unset CMD
fi

###########################################################
########################## PWD ############################
###########################################################
in_arg "pwd"
if [ $? == 1 ]
then
#    INDEX=0
    printf "***** TEST PWD *****\n"
    test_ret_stdout "pwd"
    test_ret_stdout "pwd a"
    test_ret_stdout "pwd pwd"
    test_ret_stdout "pwd a b c d e f"
    test_ret_stdout "pwd ls"
    test_ret_stdout "unset PWD\npwd"
    test_ret_stdout "unset PWD\ncd ..\npwd"
    test_ret_stdout "pwd\ncd ..\npwd"
    test_ret_stdout "pwd -"
    test_ret_stdout "pwd -p"
    test_ret_stdout "pwd --"
    test_ret_stdout "pwd --p"
    CMD="env -i"
    test_ret_stdout "pwd"
    test_ret_stdout "pwd a"
    test_ret_stdout "pwd pwd"
    test_ret_stdout "pwd a b c d e f"
    test_ret_stdout "pwd ls"
    test_ret_stdout "unset PWD\npwd"
    test_ret_stdout "unset PWD\ncd ..\npwd"
    test_ret_stdout "pwd\ncd ..\npwd"
    test_ret_stdout "pwd -"
    test_ret_stdout "pwd -p"
    test_ret_stdout "pwd --"
    test_ret_stdout "pwd --p"
    echo
fi

###########################################################
######################### UNSET ###########################
###########################################################
in_arg "unset"
if [ $? == 1 ]
then
#    INDEX=0
    printf "***** TEST UNSET *****\n"
    test_ret_stdout "unset"
    test_ret_stdout "unset a b c d"
    test_ret_stdout "unset PATH\necho \$PATH"
    test_ret_stdout "unset PATH\nls"
    test_ret_stdout "unset \"'\" test"
    test_ret_stdout "unset ="
    test_ret_stdout "unset PWD\necho \$PWD"
    test_env "export var=1\nexport var1=2\nunset var\nenv"
	CMD="env -i"
	test_ret_stdout "unset"
    test_ret_stdout "unset a b c d"
    test_ret_stdout "unset PATH\necho \$PATH"
    test_ret_stdout "unset PATH\nls"
    test_ret_stdout "unset \"'\" test"
    test_ret_stdout "unset ="
    test_ret_stdout "unset PWD\necho \$PWD"
    test_env "export var=1\nexport var1=2\nunset var\nenv"

    echo
    unset CMD
fi

###########################################################
###################### EXPAND VAR #########################
###########################################################
in_arg "expand_var"
if [ $? == 1 ]
then
 #   INDEX=0
    printf "***** TEST EXPAND VAR *****\n"
    test_ret_stdout "echo \$PATH"
    test_ret_stdout "echo \$PATH\$ q"
    test_ret_stdout "echo \$QBC"
    test_ret_stdout "echo \$?"
    test_ret_stdout "dzdq\necho \$?"
    test_ret_stdout "echo \$HOME\nunset HOME\necho \$HOME"
    test_ret_stdout "echo \$ \$q"
    test_ret_stdout "echo \$"
    test_ret_stdout "echo \$   \$a"
    test_ret_stdout "echo \$é"
    test_ret_stdout "echo \$é \$q"
    test_ret_stdout "echo \$abc f\$PWD"
    test_ret_stdout "export ABC=echo\n\$ABC a"
    test_ret_stdout "export ABC=cho\ne\$ABC b"
    test_ret_stdout "echo \$HOME?"
    test_ret_stdout "echo \$?a"
    test_ret_stdout "echo \$??"
	test_ret_stdout "echo \$PWD=22"
	test_ret_stdout "echo $:$= | cat -e"
	test_ret_stdout "echo \$USER\$var\$USER\$USER\$USERtest\$USER"
	test_ret_stdout "$"
	test_ret_stdout "echo \"   $   \" | cat -e"
	test_ret_stdout "\$LESS\$VAR"
	test_ret_stdout ""
	test_ret_stdout "\$bla"
	test_ret_stdout "echo \$var bonjour"
	test_ret_stdout "export test=\"    a    b   \"\necho ab\$test"
	test_ret_stdout "export test=\"    a    b   \"\necho \"ab\"\$test"
	test_ret_stdout "export test=\"    a    b   \"\necho ab\"\$test\""
	test_ret_stdout "export test=\"    a    b   \"\necho ab\"\"\$test\"\""
	test_ret_stdout "export test=\"    a    b   \"\necho ab\"\"\'\$test\"\"\'"
	test_ret_stdout "export var=at\nc\$var test_manuel"
	test_ret_stdout "export var=test\$var\necho \$var"
	test_ret_stdout "export var=\"a b\"\n> \$var"
	test_ret_stdout "export var=\"a b\"\n> \$\"var\""
	test_ret_stdout "export var=\"a b\"\n> \$var >hey\ncat hey\nrm hey"
	test_ret_stdout "export var=\"a b\"\n>hey > \$var\ncat hey\nrm hey"

	test_ret_stdout "export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8 && echo \"\$A\'\$B\"\'\$C\"\$D\'\$E\'\"\$F\"\'\"\'\$G\'\$H\""

	test_ret_stdout "export FOO=' \" ' && echo \$FOO"
	test_ret_stdout "export FOO=' \" ' && echo \"\$FOO\""
	test_ret_stdout "export FOO=' \" ' && echo \"\$FOO \""
	test_ret_stdout "export FOO=' \" ' && echo \" \$FOO\""
	test_ret_stdout "export FOO=' \" ' && echo \" \$FOO \""
	test_ret_stdout "export FOO=' \" ' && echo \'\$FOO\'"
	test_ret_stdout "export FOO=' \" ' && echo \'\$FOO \'"
	test_ret_stdout "export FOO=' \" ' && echo \' \$FOO\'"
	test_ret_stdout "export FOO=' \" ' && echo \' \$FOO \'"

	test_ret_stdout "export BAR=\" \' \" && echo \$BAR"
	test_ret_stdout "export BAR=\" \' \" && echo \"\$BAR\""
	test_ret_stdout "export BAR=\" \' \" && echo \"\$BAR \""
	test_ret_stdout "export BAR=\" \' \" && echo \" \$BAR\""
	test_ret_stdout "export BAR=\" \' \" && echo \" \$BAR \""
	test_ret_stdout "export BAR=\" \' \" && echo \'\$BAR\'"
	test_ret_stdout "export BAR=\" \' \" && echo \'\$BAR \'"
	test_ret_stdout "export BAR=\" \' \" && echo \' \$BAR\'"
	test_ret_stdout "export BAR=\" \' \" && echo \' \$BAR \'"


	test_ret_stdout "echo \$1"
	test_ret_stdout "echo test\$1"
	#test_ret_stdout "echo test\$1test"         #PAS A GERER
	test_ret_stdout "echo \$does_not_exist"
	test_ret_stdout "echo test\$does_not_exist"
	test_ret_stdout "echo test\$does_not_existtest"
	test_ret_stdout "echo \$NULL"
    test_ret_stdout "echo \$USER"
    test_ret_stdout "echo \$USER_not_exist"

	#test_ret_stdout "echo \$\$USER"            # PAS A GERER MAIS ...
    #test_ret_stdout "echo \$\$\$USER"          # PAS A GERER MAIS ...
	#test_ret_stdout "echo \$-"					# PAS A GERER MAIS ...
	#test_ret_stdout "echo \$-1"				# PAS A GERER MAIS ...
	#test_ret_stdout "echo test\$-r"			# PAS A GERER MAIS ...
	#test_ret_stdout "echo test\$-?"			# PAS A GERER MAIS ...
	#test_ret_stdout "echo \"test\$-r\""		# PAS A GERER MAIS ...
	#test_ret_stdout "echo \"test\$-?\""		# PAS A GERER MAIS ...
	test_ret_stdout "echo \'test\$-r\'"
	test_ret_stdout "echo \'test\$-?\'"
	#test_ret_stdout "echo \$-1\$USER"			# PAS A GERER MAIS ...
    test_ret_stdout "echo \$USER\$USER"
    test_ret_stdout "echo \$USER\"\"\$USER"
    test_ret_stdout "echo \$USER\" \"\$USER"
    test_ret_stdout "echo \$USER\"'\"\$USER"
    test_ret_stdout "echo \$USER\"''\"\$USER"
    test_ret_stdout "echo \$USER'\"'\$USER"
    test_ret_stdout "echo \$USER'\"\"'\$USER"
    test_ret_stdout "echo \$USE\"\"R"
    test_ret_stdout "echo \$USE\'\'R"
    test_ret_stdout "echo \$USER\"\""
    test_ret_stdout "echo \$USER\'\'"
    test_ret_stdout "echo \$USER\"\"TOTO"
    test_ret_stdout "echo \$USER\'\'TOTO"
    test_ret_stdout "echo \$USER \"\""
    test_ret_stdout "echo \$USER \'\'"
    test_ret_stdout "echo \"\"\$USE\"\"R"
    test_ret_stdout "echo \'\'\$USE\'\'R"
    test_ret_stdout "echo \"\"\$USER"
    test_ret_stdout "echo \'\'\$USER"
    test_ret_stdout "echo ''\$USER"
    test_ret_stdout "echo \$\"\"USER"
    test_ret_stdout "echo \$\'\'USER"
    test_ret_stdout "echo \$\"USER\""
    test_ret_stdout "echo \$\'USER\'"
	test_ret_stdout "echo \"\"'\$USER\"\"'"
	test_ret_stdout "echo \'\'\"\$USER\'\'\""

	CMD="env -i"
    test_ret_stdout "echo \$QBC"
    test_ret_stdout "echo \$?"
    test_ret_stdout "dzdq\necho \$?"
    test_ret_stdout "echo \$HOME\nunset HOME\necho \$HOME"
    test_ret_stdout "echo \$ \$q"
    test_ret_stdout "echo \$"
    test_ret_stdout "echo \$   \$a"
    test_ret_stdout "echo \$é"
    test_ret_stdout "echo \$é \$q"
    test_ret_stdout "echo \$abc f\$PWD"
    test_ret_stdout "export ABC=echo\n\$ABC a"
    test_ret_stdout "export ABC=cho\ne\$ABC b"
    test_ret_stdout "echo \$HOME?"
    test_ret_stdout "echo \$?a"
    test_ret_stdout "echo \$??"
	test_ret_stdout "echo \$USER\$var\$USER\$USER\$USERtest\$USER"
	test_ret_stdout "$"
	test_ret_stdout "\$LESS\$VAR"
	test_ret_stdout ""
	test_ret_stdout "\$bla"
	test_ret_stdout "echo \$var bonjour"
	test_ret_stdout "export test=\"    a    b   \"\necho ab\$test"
	test_ret_stdout "export test=\"    a    b   \"\necho \"ab\"\$test"
	test_ret_stdout "export test=\"    a    b   \"\necho ab\"\$test\""
	test_ret_stdout "export test=\"    a    b   \"\necho ab\"\"\$test\"\""
	test_ret_stdout "export test=\"    a    b   \"\necho ab\"\"\'\$test\"\"\'"
	test_ret_stdout "export var=test\$var\necho \$var"

	rm *var*
    echo
	unset CMD
fi

###########################################################
##################### DELETE QUOTE ########################
###########################################################
in_arg "delete_quote"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST DELETE QUOTE *****\n"
	test_ret_stdout "echo \" a \"bc\" def ghij\"klmno\" \"pqrstuv\"wxyz0123 456789abc\"defghijklm\"nopqrstuvwxyz \""

	echo
	unset CMD
fi


###########################################################
######################### PARSING #########################
###########################################################
in_arg "parsing"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST PARSING *****\n"
	test_ret_stdout "\"\""
	test_ret_stdout ".."
	test_ret_stdout "\"\"abc\"\""
	test_ret_stdout "\"\'abc\'\""
	test_ret_stdout "\'\"abc\"\'"
	test_ret_stdout "\'  \"abc\"  \'"
	test_ret_stdout "\"  \'abc\'  \""
	test_ret_stdout "\'\"abc\"\'"
	test_ret_stdout "ls  \"\""
	test_ret_stdout "\"ls \""
	test_ret_stdout "\"ls\""
	test_ret_stdout "\" ls\""
	test_ret_stdout "\" ls \""
	test_ret_stdout "l\"s\""
	test_ret_stdout "l\"s \""
	test_ret_stdout "\"l\"s"
	test_ret_stdout "\" l\"s"
	
	test_ret_stdout "ls -la"
	test_ret_stdout "ls -l"
	test_ret_stdout "ls \"\"-la"
	test_ret_stdout "ls ''-la"
	test_ret_stdout "ls \"\" -la"
	test_ret_stdout "ls '' -la"
	test_ret_stdout "ls\"\" -la"
	test_ret_stdout "ls'' -la"
	test_ret_stdout "ls \"\"\"\" -la"
	test_ret_stdout "ls '''' -la"
	test_ret_stdout "ls -\"la\""
	test_ret_stdout "ls \"-la\""  
	test_ret_stdout "ls \"-l\"a"  
	test_ret_stdout "ls -'la'"    
	test_ret_stdout "ls '-la'"    
	test_ret_stdout "ls '-l'a"    
	
	test_ret_stdout "ls \"   '   \""
	test_ret_stdout "ls \"   '\""
	test_ret_stdout "ls \"'   \""
	test_ret_stdout "ls \"'\""
	test_ret_stdout "ls '\"'"
	test_ret_stdout "ls '\"  '"
	test_ret_stdout "ls '  \"'"
	test_ret_stdout "ls '   \"  '"
	test_ret_stdout "echo \"\" bonjour"
	test_ret_stdout "   echo         bonjour    "
	test_ret_stdout "echo test | cat"
	test_ret_stdout "echo test ||| cat"
	test_ret_stdout "export \"\""
	test_ret_stdout "unset \"\""
	test_ret_stdout "export \"test=ici\"=coucou\necho \$test"
	test_ret_stdout "export var=\"cat Makefile | grep >\"\necho \$var"
	test_ret_stdout "echo \"\$test\"\"Makefile\""
	test_ret_stdout "echo \"\$test\"Makefile"
	test_ret_stdout "export test=\"   a    b    \"\necho \$test"

	echo
	unset CMD
fi

###########################################################
########################### PIPE ##########################
###########################################################
in_arg "pipe"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST PIPE *****\n"
	
	test_ret_stdout "ls | sort"
	test_ret_stdout "ls|ls |ls | ls| ls |                ls"
	test_ret_stdout "ls | sort | grep i | wc -l"
	test_ret_stdout "sleep 0.1 | ls"
	test_ret_stdout "cat Makefile | grep pr | head -n 5 | cd file_not_exist"
	test_ret_stdout "cat Makefile | grep pr | head -n 5 | hello"
	test_ret_stdout "ls | exit"
	test_ret_stdout "sleep 0.1 | exit"
	test_ret_stdout "echo bip | bip\necho coyotte ><"
	test_ret_stdout "ls | ls | ls | ls | ls /proc/self/fd"
	cd $DIR_TEST
	test_ret_stdout "cat a | < abc cat | cat > c | cat\nrm c"
	cd ..
	

	echo
	unset CMD
fi

###########################################################
################### EXPAND ASTERISK #######################
###########################################################
in_arg "expand_asterisk"
if [ $? == 1 ]
then
#    INDEX=0
    printf "***** TEST EXPAND ASTERISK *****\n"
	cd $DIR_TEST
	
	test_ret_stdout "echo *"
	test_ret_stdout "echo .*"
	test_ret_stdout "echo *a"
	test_ret_stdout "echo a*"
	test_ret_stdout "echo a*bc"
	test_ret_stdout	"export TMP=*\nls | grep $TMP"
	test_ret_stdout	"export TMP=*\ncd ..\nls | grep $TMP"
	test_ret_stdout	"echo ****a*****b****c****d****"
	test_ret_stdout "echo a"
	test_ret_stdout "*"
	test_ret_stdout "mkdir t1\ncd t1\nrm -rf ../t1\necho *"
	test_ret_stdout "mkdir t1\ncd t1\nrm -rf ../t1\necho .*"
	test_ret_stdout "echo \"ab\"*"
	test_ret_stdout "echo \"a \"*"
	test_ret_stdout "echo \'ab\'*"
	test_ret_stdout "echo \'a \'*"
	test_ret_stdout "echo a\"b\"*"
	test_ret_stdout "echo a\" b\"*"
	test_ret_stdout "echo *\"b\""
	test_ret_stdout "echo *\"b\"*"

	cd ../
    echo
	unset CMD
fi

###########################################################
######################### HEREDOC #########################
###########################################################
in_arg "heredoc"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST HEREDOC *****\n"
	cd $DIR_TEST
	test_ret_stdout "cat << end\nabc\nend"
	test_ret_stdout "cat << end <<start\nabc\nstart\nend\nend\ndef\nstart"
	test_ret_stdout "cat << end\nEnd\neNd\nenD\nENd\nEnD\neND\nEND\nend"
	test_ret_stdout "<< end cat\nabc\nend"
	test_ret_stdout "<< end <<start cat\nabc\nstart\nend\nend\ndef\nstart"
	test_ret_stdout "<<end cat\nEnd\neNd\nenD\nENd\nEnD\neND\nEND\nend"
	test_ret_stdout "<<end cat <<start abc\nStart\nstart\nend\ndef\n\nae\nend\nstart"
	test_ret_stdout "<<end cat <<start abc def << x\na1\nend\na2\nstart\na3\nx"

	cd ..
	echo
	unset CMD
fi
###########################################################
##################### REDIRECTIONS ########################
###########################################################
in_arg "redir"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST REDIRECTIONS *****\n"
	cd $DIR_TEST

	test_ret_stdout "<abc cat <abcd <def"
	test_ret_stdout "<abc cat <doesntexist < dev"
	test_ret_stdout "> test | echo blabla\nrm test"
	test_ret_stdout ">a cat <b >>c\ncat a c\nrm a c"
	test_ret_stdout ">a ls <b >>c >d\ncat a c d\nrm a c d"
	test_ret_stdout ">a\ncat a\nrm a"
	test_ret_stdout "cat -e > a < b\ncat a\nrm a"
	test_ret_stdout "cat < a"
	test_ret_stdout "echo 2 > a >> b\ncat a b\nrm a b"
	test_ret_stdout "echo 2 >> a > b\ncat a b\nrm a b"
	test_ret_stdout "echo a > b c\necho b\nrm a b c\n"
	test_ret_stdout "not_cmd a > b\ncat b\nrm b"
	test_ret_stdout "echo a > \$a"
	test_ret_stdout "cat < abc < abcd < abcde > t1 > t2 << end >> t3 << t4\na1\nend\na2\nt4\ncat t1 t2 t3\n rm t1 t2 t3"

	cd ../
	echo
	unset CMD
fi

###########################################################
########################### AND ###########################
###########################################################
in_arg "and"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST AND *****\n"
	test_ret_stdout "ls && uname"
	test_ret_stdout "ls && cat fesfjseoi && pwd"
	test_ret_stdout "ls && ls -a && ls -l && ls -la && ls -fesfs && ls"
	test_ret_stdout "ls | grep a && ls"
	test_ret_stdout "ls | grep z && ls"
	test_ret_stdout "ls && sleep 0.1 && pwd"
	test_ret_stdout "sleep 0.1 && ls && sleep 0.1 && pwd"
	test_ret_stdout "sleep -1 && ls"
	test_ret_stdout "ls -fsenifs && ls -fseifkse && ls && ls"

	echo
	unset CMD
fi

###########################################################
########################### OR ############################
###########################################################
in_arg "or"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST OR *****\n"
	test_ret_stdout "ls || uname"
	test_ret_stdout "uname || cat doesnotexist"
	test_ret_stdout "cat doesnotexist || uname"
	test_ret_stdout "cat doesnotexist || cat doesnotexist || cat doesnotexist || uname"
	test_ret_stdout "cat doesnotexist || uname || cat doesnotexist || uname"
	test_ret_stdout "touch toto\n ls | grep toto || uname\n rm toto"
	test_ret_stdout "ls | grep toto || uname"
	test_ret_stdout "cat doesnotexist | ls || uname"
	test_ret_stdout "cat doesnotexist | cat stillnotexist || uname"
	test_ret_stdout "touch toto\n uname || ls | grep toto \nrm toto"
	echo
	unset CMD
fi


###########################################################
########################### ANDOR #########################
###########################################################
in_arg "andor"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST ANDOR *****\n"
	test_ret_stdout "echo a && echo b || echo c"
	test_ret_stdout "echo a || echo b && echo c"
	test_ret_stdout "cat toto && echo b || echo c"
	test_ret_stdout "cat toto || echo b && echo c"
	test_ret_stdout "echo a && cat toto || echo c"
	test_ret_stdout "echo a || cat toto && echo c"
	test_ret_stdout "echo a && echo b || cat toto"
	test_ret_stdout "echo a || echo b && cat toto"
	test_ret_stdout "echo a || echo b || echo c && echo d"
	test_ret_stdout "echo a || echo b && echo c || echo d"
	test_ret_stdout "echo a && echo b || echo c || echo d"
	test_ret_stdout "cat toto || echo b || echo c && echo d"
	test_ret_stdout "cat toto || echo b && echo c || echo d"
	test_ret_stdout "cat toto && echo b || echo c || echo d"
	test_ret_stdout "echo a || cat toto || echo c && echo d"
	test_ret_stdout "echo a || cat toto && echo c || echo d"
	test_ret_stdout "echo a && cat toto || echo c || echo d"
	test_ret_stdout "echo a || echo b || cat toto && echo d"
	test_ret_stdout "echo a || echo b && cat toto || echo d"
	test_ret_stdout "echo a && echo b || cat toto || echo d"
	test_ret_stdout "echo a || echo b || echo c && cat toto"
	test_ret_stdout "echo a || echo b && echo c || cat toto"
	test_ret_stdout "echo a && echo b || echo c || cat toto"
	test_ret_stdout "echo a || echo b && echo c && echo d"
	test_ret_stdout "echo a && echo b || echo c && echo d"
	test_ret_stdout "echo a && echo b && echo c || echo d"
	test_ret_stdout "cat toto || echo b && echo c && echo d"
	test_ret_stdout "cat toto && echo b || echo c && echo d"
	test_ret_stdout "cat toto && echo b && echo c || echo d"
	test_ret_stdout "echo a || cat toto && echo c && echo d"
	test_ret_stdout "echo a && cat toto || echo c && echo d"
	test_ret_stdout "echo a && cat toto && echo c || echo d"
	test_ret_stdout "echo a || echo b && cat toto && echo d"
	test_ret_stdout "echo a && echo b || cat toto && echo d"
	test_ret_stdout "echo a && echo b && cat toto || echo d"
	test_ret_stdout "echo a || echo b && echo c && cat toto"
	test_ret_stdout "echo a && echo b || echo c && cat toto"
	test_ret_stdout "echo a && echo b && echo c || cat toto"

	echo
	unset CMD
fi

###########################################################
######################## SUBSHELL #########################
###########################################################
in_arg "subshell"
if [ $? == 1 ]
then
#	INDEX=0
	printf "***** TEST SUBSHELL *****\n"
	test_ret_stdout "(ls && pwd || uname)"
	test_ret_stdout "(ls && (ls && (ls && (ls && (ls && ls)))))"
	test_ret_stdout "ls -fesgse && (uname | cat) || pwd"
	test_ret_stdout "ls && (uname | cat) || pwd"
	test_ret_stdout "ls -fsfdfds || (uname | cat) && pwd"
	test_ret_stdout "ls || (uname | cat) && pwd"

	echo
	unset CMD
fi

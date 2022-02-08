/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 10:04:17 by ngiroux           #+#    #+#             */
/*   Updated: 2022/01/09 17:45:52 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

static int	__test(int (*f)(void), t_utest *utest, char *fun, char *name)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == 0)
	{
		__log_name(fun, name);
		ret = f();
		__lstclear(utest);
		exit(ret);
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status) > 0)
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status) > 0)
			return (WTERMSIG(status));
		else
			return (-1);
	}
}

static char	*__error_message(int ret_error)
{
	if (ret_error == _SUCCESS)
		return (_MSG_OK);
	if (ret_error == _KO)
		return (_MSG_KO);
	if (ret_error == _SEGFAULT)
		return (_MSG_SEG);
	if (ret_error == _SEGBUS)
		return (_MSG_BUS);
	if (ret_error == _LEAK)
		return (_MSG_LEAK);
	return (_MSG_UKN);
}

int	__print_score(int test_cnt, int cnt)
{
	__putnbr((test_cnt - cnt));
	__putstr("/");
	__putnbr(test_cnt);
	__putstr("\n\n");
	if (cnt == 0)
		return (0);
	return (-1);
}

/* static void	__print_name(char *fun, char *name)
{
	__putstr("[");
	__putstr(fun);
	__putstr("] : ");
	__putstr("[");
	__putstr(name);
	__putstr("]");
} */

int	__launcher(t_utest *utest, t_score *score)
{
	t_utest	*tmp;
	int		ret_error;
	int		cnt;
	int		test_cnt;

	if (!utest)
		return (-1);
	tmp = utest;
	ret_error = 0;
	cnt = 0;
	test_cnt = 0;

	printf(BOLDWHITE" %s >\n"RESET,utest->fun);
	while (utest != NULL)
	{
		ret_error = __test(utest->f, tmp, utest->fun, utest->name);
		if (ret_error != 0)
			cnt++;
		test_cnt++;
	 printf("%7c %-8s %-2c %3s\n",' ', utest->name,':', __error_message(ret_error));
		utest = utest->next;
	}
	__lstclear(tmp);
	score->ok += test_cnt - cnt;
	score->total += test_cnt;
	return (__print_score(test_cnt, cnt));
}

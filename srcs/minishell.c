/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/09 13:06:35 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

char	*__get_prompt(t_msh *msh)
{
	char	path[PATH_MAX];
	int		i;

	i = 0;
	__bzero(path, PATH_MAX);
	if (msh->prompt)
		free(msh->prompt);
	if (msh->rv == 0)
		msh->prompt = __strdup(BOLDGREEN"➜  "RESET BOLDCYAN);
	else
		msh->prompt = __strdup(BOLDRED"➜  "RESET BOLDCYAN);
	if (getcwd(path, PATH_MAX))
	{
		while (path[i])
			i++;
		while (path[i] != '/' && i >= 0)
			i--;
		i++;
		msh->prompt = __strjoin(msh->prompt, path + i);
		msh->prompt = __strjoin(msh->prompt, RESET BOLDYELLOW "  ~  "RESET);
	}
	return (msh->prompt);
}

static void	get_env(t_msh *msh, char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	msh->envp = (char ***)malloc((i + 1) * sizeof(char **));
	msh->envp[i] = NULL;
	while (i--)
	{
		msh->envp[i] = (char **)malloc((3) * sizeof(char *));
		msh->envp[i][0] = __strdup(envp[i]);
		msh->envp[i][1] = __strdup("1");
		msh->envp[i][2] = NULL;
	}
}
/*

// destroy_env(t_msh *msh)
//TODO

static void	print_env(t_msh *msh)
{
	int	i;
	int	j;

	i = 0;
	while (msh->envp[i])
	{
		j = 0;
		while (msh->envp[i][j])
		{
			printf("msh->envp[%d][%d] : %s", i, j, msh->envp[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}
*/

void __mini_parsing(char *arg, t_msh *msh)
{
	char	**argv;
	int		i;

	(void)msh;
	argv = __split(arg, ' ');
	if (__strncmp(argv[0], "echo", 4) == 0)
		__echo(argv, 1);
	if (__strncmp(argv[0], "cd", 2) == 0)
		__cd(argv[1]);
	if (__strncmp(argv[0], "pwd", 3) == 0)
		__pwd(1);
	if (__strncmp(argv[0], "env", 3) == 0)
		__env(msh);
	if (__strncmp(argv[0], "export", 6) == 0)
		__export(msh, argv[1]);
	if (__strncmp(argv[0], "unset", 5) == 0)
		__unset(argv + 1, msh);
	i = -1;
	while (argv[++i])
		free(argv[i]);
	free(argv[i]);
	free(argv);
}

int	main (int ac, char *av[], char *envp[])
{
	char	*arg;
	t_msh	msh;
	(void) ac;
	(void) av;

	msh = (t_msh){.rv = 0};
	get_env(&msh, envp);
	//print_env(&msh);
	while (42)
	{
		arg = readline(__get_prompt(&msh));
		add_history(arg);
		signal(SIGINT, history);
		printf("%s\n", arg);
		if (__strncmp(arg, "exit", __strlen("exit")) == 0 || arg == NULL)
		{
			if (arg)
				free(arg);
			arg = NULL;
			break ;
		}
		__mini_parsing(arg, &msh);

	}
	free(msh.prompt);
}

/*
	__pwd(1);
	__echo(arg, 1);
	__cd("srcs");
	__pwd(1);
	__cd("/dev");
	__pwd(1);
	__env(NULL);
	*/

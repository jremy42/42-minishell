/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/11 15:16:10 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	g_rv;

char	*__get_prompt(t_msh *msh)
{
	char	path[PATH_MAX];
	int		i;

	i = 0;
	__bzero(path, PATH_MAX);
	if (msh->prompt)
		free(msh->prompt);
	if (g_rv == 0)
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
	if (!envp)
		return ;
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

void	__mini_parsing(char *arg, t_msh *msh)
{
	char	**argv;
	int		i;
	t_list	*start;
	t_list	*index;
	char *to_tokenize;
	
	start = NULL;
	to_tokenize = __strtrim(arg, " \f\t\r\v");
	__tokenize(to_tokenize, &start);
	free(to_tokenize);
	index = start;
	while (index)
	{
		printf("Le token vaut : >%s<\n", (char *)index->content); 
		index = index->next;
	}
	argv = __split(arg, ' ');
	if (__strncmp(argv[0], "echo", 4) == 0)
		__echo(argv, 1);
	if (__strncmp(argv[0], "cd", 2) == 0)
		__cd(argv[1], msh);
	if (__strncmp(argv[0], "pwd", 3) == 0)
		__pwd(1);
	if (__strncmp(argv[0], "env", 3) == 0)
		__env(msh);
	if (__strncmp(argv[0], "export", 6) == 0)
		__export(argv + 1, msh);
	if (__strncmp(argv[0], "unset", 5) == 0)
		__unset(argv + 1, msh);
	i = -1;
	while (argv[++i])
		free(argv[i]);
	free(argv);
}

int	main (int ac, char *av[], char *envp[])
{
	char	*arg;
	t_msh	msh;
	(void)	ac;
	(void)	av;
	(void)	envp;

	msh = (t_msh){.rv = 0};
	get_env(&msh, envp);
	while (42)
	{
		signal(SIGINT, __signal);
		signal(SIGQUIT, __signal);
		arg = readline(__get_prompt(&msh));
		add_history(arg);
		if (__strncmp(arg, "exit", __strlen("exit")) == 0 || arg == NULL)
		{
			if (arg)
				free(arg);
			arg = NULL;
			break ;
		}
		__mini_parsing(arg, &msh);
	}
	__exit(&msh);
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

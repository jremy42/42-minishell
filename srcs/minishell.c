/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 18:59:11 by fle-blay         ###   ########.fr       */
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

int	main (int ac, char *av[], char *envp[])
{
	(void) ac;
	(void) av;

	char	*arg[10];
	int		i;
	t_msh	msh;

	(void)ac;
	(void)av;
	msh = (t_msh){.rv = 1};
	i = 0;
	get_env(&msh, envp);
	print_env(&msh);
	while (42)
	{
		arg[i] = readline(__get_prompt(&msh));
		if (arg[i][0] == '*')
		{
			free(arg[i]);
			arg[i] = NULL;
			break ;
		}
		i++;
	}
	__pwd(1);
	__echo(arg, 1);
	__cd("srcs");
	__pwd(1);
	__cd("/dev");
	__pwd(1);
	__env(NULL);
	free(msh.prompt);
}

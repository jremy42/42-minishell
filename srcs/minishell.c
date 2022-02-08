/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 19:06:04 by jremy            ###   ########.fr       */
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

void __mini_parsing(char *arg, t_msh *msh)
{
	char **argv;

	(void)msh;
	argv = __split(arg, ' ');
	if (__strncmp(argv[0], "echo", 4) == 0)
		__echo(argv, 1);
}

int	main (int ac, char *av[], char *envp[])
{
	(void) ac;
	(void) av;
	(void)envp;
	char	*arg;
	t_msh	msh;
	
	msh = (t_msh){.rv = 1};
	while (42)
	{
		arg = readline(__get_prompt(&msh));
		__mini_parsing(arg, &msh);
		if (__strncmp(arg, "exit", __strlen("exit")) == 0 || arg == NULL)
		{
			if (arg)
				free(arg);
			arg = NULL;
			break ;
		}
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

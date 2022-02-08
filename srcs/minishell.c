/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 15:24:57 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	main (int ac, char **av, char **envp)
{
	char	*arg[10];
	int		i;
	t_msh	msh;

	(void)ac;
	(void)av;
	(void)envp;
	msh = (t_msh){.rv = 1};
	i = 0;
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
	__env(NULL);
	free(msh.prompt);
}

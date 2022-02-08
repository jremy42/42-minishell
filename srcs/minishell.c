/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 10:22:30 by jremy            ###   ########.fr       */
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

int	main ()
{
	char	*arg[10];
	int		i;
	char	*prompt;
	t_msh	msh;

	msh.rv = 1;
	msh.prompt = NULL;
	i = 0;
	prompt = NULL;
	while (42)
	{
		prompt = readline(__get_prompt(&msh));
		arg[i] = prompt;
		if (prompt[i] == '*')
			break ;
		i++;
	}
	arg[i++] = NULL;
	__pwd(1);
	__echo(arg, 1);
}

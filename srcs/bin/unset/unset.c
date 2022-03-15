/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:29:51 by jremy             #+#    #+#             */
/*   Updated: 2022/03/15 09:16:00 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int __is_arg(char **arg, char* env)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (__strncmp(arg[i], env, __strlen(arg[i])) == 0 
				&& (env[__strlen(arg[i])] == '='))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	__unset(char **arg, t_msh *msh)
{
	int	i;

	i = 0;
	if (!arg[0])
		return (__SUCCESS);
	while (msh->envp[i])
	{
		if (__is_arg(arg, msh->envp[i][0]) == 1)
		{
			msh->envp[i][0][0] = '\0';
			msh->envp[i][1][0] = '0';
		}
		i++;
	}
	return (__SUCCESS);
}
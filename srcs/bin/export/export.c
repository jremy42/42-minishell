/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 10:21:11 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/09 10:36:16 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	__export(t_msh *msh, char *key_val)
{
	char	***new_env;
	int		i;

	i = 0;
	while (msh->envp[i])
		i++;	
	new_env = (char ***)malloc((i + 2) * sizeof(char **));
	if (!new_env)
		return (__FAIL);
	new_env[i + 1] = NULL;
	new_env[i] = (char **)malloc((3) * sizeof(char *));
	if (!new_env[i])
	{
		free(new_env);
		return (__FAIL);
	}
	new_env[i][0] = __strdup(key_val);
	new_env[i][1] = __strdup("1");
	new_env[i][2] = NULL;
	while (--i)
		new_env[i] = msh->envp[i];
	msh->envp = new_env;
	return (__SUCCESS);
}

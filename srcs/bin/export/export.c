/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 10:21:11 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/09 11:53:51 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static int	add_key_val(t_msh *msh, char *key_val, int i)
{
	char	***new_env;

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
	while (--i > -1)
		new_env[i] = msh->envp[i];
	free(msh->envp);
	msh->envp = new_env;
	return (__SUCCESS);
}

static int	modify_status_key_val(t_msh *msh, char *key)
{
	int		i;

	i = 0;
	while (msh->envp[i])
	{
		if (!__strncmp(msh->envp[i][0], key, __strlen(key)))
			break ;
		i++;
	}
	if (!msh->envp[i])
		return (__FAIL);
	free(msh->envp[i][1]);
	msh->envp[i][1] = __strdup("1");
	if (!msh->envp[i][1])
		return (__FAIL);
	return (__SUCCESS);
}

int	__export(t_msh *msh, char *key_val)
{
	int		i;

	i = 0;
	while (msh->envp[i])
		i++;	
	printf("i : >%d<\n", i);
	if (__strchr(key_val, '='))
	{
		if (add_key_val(msh, key_val, i) == __SUCCESS)
			return (__SUCCESS);
		else
			return (__FAIL);
	}
	else
	{
		if (modify_status_key_val(msh, key_val) == __SUCCESS)
			return (__SUCCESS);
		else
			return (__FAIL);
	}
}

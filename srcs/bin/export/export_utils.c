/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:13:52 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/28 18:31:49 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_envp_size(t_msh *msh)
{
	int		i;

	i = 0;
	while (msh->envp[i])
		i++;
	return (i);
}

int	key_exist(t_msh *msh, char *key)
{
	int		i;
	char	*plus;
	char	*equal;
	char	to_find;

	plus = __strchr(key, '+');
	equal = __strchr(key, '=');
	if (!equal && !plus)
		return (0);
	i = 0;
	if (!plus || plus > equal)
		to_find = '=';
	else
		to_find = '+';
	while (msh->envp[i])
	{
		if (!__strncmp(msh->envp[i][0], key, __strchr(key, to_find) - key))
			return (i);
		i++;
	}
	return (-1);
}

int	update_key_val(t_msh *msh, char *key_val)
{
	int	pos;

	pos = key_exist(msh, key_val);
	if (pos < 0)
		return (__FAIL);
	free(msh->envp[pos][0]);
	free(msh->envp[pos][1]);
	msh->envp[pos][0] = NULL;
	msh->envp[pos][1] = NULL;
	msh->envp[pos][0] = __strdup(key_val);
	if (!msh->envp[pos][0])
		return (__MALLOC);
	msh->envp[pos][1] = __strdup("1");
	if (!msh->envp[pos][1])
	{
		free(msh->envp[pos][0]);
		msh->envp[pos][0] = NULL;
		return (__MALLOC);
	}
	return (__SUCCESS);
}

int	join_key_val(t_msh *msh, char *key_val)
{
	int	pos;

	pos = key_exist(msh, key_val);
	free(msh->envp[pos][1]);
	msh->envp[pos][0] = __strjoin(msh->envp[pos][0],
			__strstr(key_val, "+=") + 2);
	if (!msh->envp[pos][0])
		return (__MALLOC);
	msh->envp[pos][1] = __strdup("1");
	if (!msh->envp[pos][1])
		return (free(msh->envp[pos][0]),__MALLOC);
	return (__SUCCESS);
}

int	add_key_val(t_msh *msh, char *key_val, int i)
{
	char	***new_env;

	new_env = (char ***)malloc((i + 2) * sizeof(char **));
	if (!new_env)
		return (__MALLOC);
	new_env[i + 1] = NULL;
	new_env[i] = (char **)malloc((3) * sizeof(char *));
	if (!new_env[i])
	{
		free(new_env);
		return (__MALLOC);
	}
	new_env[i][0] = __strdup(key_val);
	if (!new_env[i][0])
		return (free(new_env[i]), free(new_env),__MALLOC);
	new_env[i][1] = __strdup("1");
	if (!new_env[i][1])
		return (free(new_env[i][0]), free(new_env[i]), free(new_env),__MALLOC);
	new_env[i][2] = NULL;
	while (--i >= 0)
		new_env[i] = msh->envp[i];
	free(msh->envp);
	msh->envp = new_env;
	return (__SUCCESS);
}

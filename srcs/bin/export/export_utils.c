/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:13:52 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/11 10:15:06 by fle-blay         ###   ########.fr       */
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

	i = 0;
	while (msh->envp[i])
	{
		if (!__strncmp(msh->envp[i][0], key, __strchr(key, '=') - key))
			return (i); 
		i++;	
	}
	return (0);
}

int	update_key_val(t_msh *msh, char *key_val)
{
	int	pos;

	pos = key_exist(msh, key_val);
	free(msh->envp[pos][0]);
	free(msh->envp[pos][1]);
	msh->envp[pos][0] = __strdup(key_val);
	msh->envp[pos][1] = __strdup("1");
	if (!msh->envp[pos][0] || !msh->envp[pos][1])
		return (__FAIL);
	return (__SUCCESS);
}

int	add_key_val(t_msh *msh, char *key_val, int i)
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
	if (!new_env[i][0] || !new_env[i][1])
		return (__FAIL);
	new_env[i][2] = NULL;
	while (--i >= 0)
		new_env[i] = msh->envp[i];
	free(msh->envp);
	msh->envp = new_env;
	return (__SUCCESS);
}

int	modify_status_key_val(t_msh *msh, char *key_val)
{
	int		i;

	i = 0;
	while (msh->envp[i])
	{
		if (!__strncmp(msh->envp[i][0], key_val, __strlen(key_val)))
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


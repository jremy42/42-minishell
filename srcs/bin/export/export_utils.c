/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:13:52 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/12 18:56:45 by jremy            ###   ########.fr       */
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
		if (msh->envp[i][1][0] != '0' && __is_same_key(msh->envp[i][0], key))
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
	int		pos;

	pos = key_exist(msh, key_val);
	if (pos == -1)
		return (add_key_val_skip_plus(msh, key_val, get_envp_size(msh), "1"));
	if (__strchr(msh->envp[pos][0], '='))
	{
		free(msh->envp[pos][1]);
		msh->envp[pos][0] = __strjoin(msh->envp[pos][0],
				__strstr(key_val, "+=") + 2);
		if (!msh->envp[pos][0])
			return (__MALLOC);
		msh->envp[pos][1] = __strdup("1");
		if (!msh->envp[pos][1])
			return (free(msh->envp[pos][0]), __MALLOC);
		return (__SUCCESS);
	}
	else
		return (update_key_val(msh, key_val));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 10:04:49 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/28 12:39:15 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (add_key_val(msh, key_val, get_envp_size(msh)));
	free(msh->envp[i][1]);
	msh->envp[i][1] = __strdup("1");
	if (!msh->envp[i][1])
		return (__MALLOC);
	return (__SUCCESS);
}

char	*get_key(t_msh *msh, char *key)
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
	{
		__putstr_fd(key, 2);
		__putstr_fd(" not set\n", 2);
		return (NULL);
	}
	return (__strchr(msh->envp[i][0], '=') + 1);
}

int	get_number_exported_values(char ***envp)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (envp[i])
	{
		if (envp[i][1][0] != '0')
			count++;
		i++;
	}
	return (count);
}

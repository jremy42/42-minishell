/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 10:04:49 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/11 17:51:01y jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	modify_status_key_val(t_msh *msh, char *key_val)
{
	int		i;

	i = 0;
	while (msh->envp[i])
	{
		if (msh->envp[i][1][0] != '0'
		&& !__strncmp(msh->envp[i][0], key_val,
			strchr(msh->envp[i][0], '=') - msh->envp[i][0]))
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
		return (NULL);
	if (__strchr(msh->envp[i][0], '='))
		return (__strchr(msh->envp[i][0], '=') + 1);
	return (NULL);
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

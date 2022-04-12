/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 18:53:11 by jremy             #+#    #+#             */
/*   Updated: 2022/04/12 18:56:15 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*__str_whithout_plus(char *key_val)
{
	char	*tmp;

	tmp = __substr(key_val, 0, __strchr(key_val, '+') - key_val);
	if (!tmp)
		return (NULL);
	tmp = __strjoin(tmp, __strchr(key_val, '='));
	return (tmp);
}

int	add_key_val_skip_plus(t_msh *msh, char *key_val, int i, char *export_status)
{
	char	***new_env;

	new_env = (char ***)malloc((i + 2) * sizeof(char **));
	if (!new_env)
		return (__MALLOC);
	new_env[i + 1] = NULL;
	new_env[i] = (char **)malloc((3) * sizeof(char *));
	if (!new_env[i])
		return (free(new_env), __MALLOC);
	new_env[i][0] = __str_whithout_plus(key_val);
	if (!new_env[i][0])
		return (free(new_env[i]), free(new_env), __MALLOC);
	new_env[i][1] = __strdup(export_status);
	if (!new_env[i][1])
		return (free(new_env[i][0]), free(new_env[i]), free(new_env), __MALLOC);
	new_env[i][2] = NULL;
	while (--i >= 0)
		new_env[i] = msh->envp[i];
	free(msh->envp);
	msh->envp = new_env;
	return (__SUCCESS);
}

int	add_key_val(t_msh *msh, char *key_val, int i, char *export_status)
{
	char	***new_env;

	new_env = (char ***)malloc((i + 2) * sizeof(char **));
	if (!new_env)
		return (__MALLOC);
	new_env[i + 1] = NULL;
	new_env[i] = (char **)malloc((3) * sizeof(char *));
	if (!new_env[i])
		return (free(new_env), __MALLOC);
	new_env[i][0] = __strdup(key_val);
	if (!new_env[i][0])
		return (free(new_env[i]), free(new_env), __MALLOC);
	new_env[i][1] = __strdup(export_status);
	if (!new_env[i][1])
		return (free(new_env[i][0]), free(new_env[i]), free(new_env), __MALLOC);
	new_env[i][2] = NULL;
	while (--i >= 0)
		new_env[i] = msh->envp[i];
	free(msh->envp);
	msh->envp = new_env;
	return (__SUCCESS);
}

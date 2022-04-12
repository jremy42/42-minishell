/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:23:48 by jremy             #+#    #+#             */
/*   Updated: 2022/04/12 15:35:09 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size_env(char *envp[])
{
	int	size;

	size = 0;
	if (!envp)
		return (0);
	while (envp[size])
		size++;
	return (size);
}

void	partial_destroy_env(t_msh *msh, int limit)
{
	int	i;

	i = 0;
	if (!msh->envp)
		return ;
	while (i < limit)
	{
		free(msh->envp[i][0]);
		free(msh->envp[i][1]);
		free(msh->envp[i]);
		i++;
	}
	free(msh->envp);
}

int	__set_oldpwd(t_msh *msh)
{
	char	*cmd[2];

	cmd[0] = "OLDPWD";
	cmd[1] = NULL;
	return (__export(cmd, msh));
}

int	__set_mini_path(t_msh *msh)
{
	int	ret;
	int	size;

	ret = 1;
	size = get_envp_size(msh);
	if (key_exist(msh, "PATH") < 0)
		ret = add_key_val(msh, MINIPATH, size, "0");
	return (ret);
}

int	get_env(t_msh *msh, char *envp[], int size)
{
	int	i;

	i = -1;
	msh->envp = (char ***)malloc((size + 1) * sizeof(char **));
	if (!msh->envp)
		return (0);
	msh->envp[size] = NULL;
	while (++i < size)
	{
		msh->envp[i] = (char **)malloc((3) * sizeof(char *));
		if (!msh->envp[i])
			return (partial_destroy_env(msh, i), 0);
		msh->envp[i][0] = __strdup(envp[i]);
		if (!msh->envp[i][0])
			return (free(msh->envp[i]), partial_destroy_env(msh, i), 0);
		msh->envp[i][1] = __strdup("1");
		if (!msh->envp[i][1])
			return (free(msh->envp[i][0]), free(msh->envp[i]),
				partial_destroy_env(msh, i), 0);
		msh->envp[i][2] = NULL;
	}
	if (update_pwd(msh) == __MALLOC || update_shlvl(msh) == __MALLOC
		|| __set_oldpwd(msh) == __MALLOC || __set_mini_path(msh) == __MALLOC)
		__exit_error(msh, 240, "Malloc Error : setting minimal env");
	return (1);
}

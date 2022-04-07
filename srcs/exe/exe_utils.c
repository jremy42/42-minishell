/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:19:33 by jremy             #+#    #+#             */
/*   Updated: 2022/04/07 14:18:13 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

int	__is_builtin(char **arg)
{
	if (!arg || !arg[0])
		return (0);
	if (__strcmp(arg[0], "echo") == 0)
		return (1);
	if (__strcmp(arg[0], "cd") == 0)
		return (1);
	if (__strcmp(arg[0], "pwd") == 0)
		return (1);
	if (__strcmp(arg[0], "env") == 0)
		return (1);
	if (__strcmp(arg[0], "export") == 0)
		return (1);
	if (__strcmp(arg[0], "unset") == 0)
		return (1);
	if (__strcmp(arg[0], "exit") == 0)
		return (1);
	return (0);
}

int	__exec_builtin(char **arg, t_msh *msh, t_cmd *cmd, int fork)
{
	if (__strcmp(arg[0], "echo") == 0)
		msh->rv = __echo(arg, 1);
	if (__strcmp(arg[0], "cd") == 0)
		msh->rv = __cd(arg + 1, msh);
	if (__strcmp(arg[0], "pwd") == 0)
		msh->rv = __pwd(1);
	if (__strcmp(arg[0], "env") == 0)
		msh->rv = __env(msh);
	if (__strcmp(arg[0], "export") == 0)
		msh->rv = __export(arg + 1, msh);
	if (__strcmp(arg[0], "unset") == 0)
		msh->rv = __unset(arg + 1, msh);
	if (__strcmp(arg[0], "exit") == 0)
		msh->rv = __bin_exit(arg, msh, cmd, fork);
	return (msh->rv);
}

int	__save_fd(int *std)
{
	std[out] = dup(STDOUT_FILENO);
	std[in] = dup(STDIN_FILENO);
	if (std[out] < 0 || std[in] < 0)
		return (0);
	return (1);
}

int	__restore_fd(int *std)
{
	if (dup2(std[out], STDOUT_FILENO) < 0)
		return (0);
	if (close(std[out]) < 0)
		return (0);
	if (dup2(std[in], STDIN_FILENO) < 0)
		return (0);
	if (close(std[in]) < 0)
		return (0);
	return (1);
}

int	__update_underscore(char **arg, t_msh *msh)
{
	int		i;
	char	*to_export;
	char	*tab[2];

	i = 0;
	if (!arg)
		return (1);
	while (arg[i])
		i++;
	if (i > 0)
		i--;
	if (!arg[i])
		return (1);
	to_export = __old_strjoin("_=", arg[i]);
	if (!to_export)
		return (0);
	tab[0] = to_export;
	tab[1] = NULL;
	msh->rv = __export(tab, msh);
	if (msh->rv == 240)
		return (free(to_export), 0);
	return (free(to_export), 1);
}

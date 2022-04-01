/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:19:33 by jremy             #+#    #+#             */
/*   Updated: 2022/04/01 10:55:46 by jremy            ###   ########.fr       */
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

int	__exec_builtin(char **arg, t_msh *msh, t_cmd *cmd)
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
		msh->rv = __bin_exit(arg, msh, cmd);
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
	DEBUG && fprintf(stderr, "restore fd\n");
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

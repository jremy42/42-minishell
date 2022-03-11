/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:20:32 by jremy             #+#    #+#             */
/*   Updated: 2022/03/11 12:23:04 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

void	__cmd_list_clear(t_cmd *start)
{
	t_cmd	*next_to_free;

	while (start)
	{
		next_to_free = start->next;
		__redirect_list_clear(start->redirect);
		free_split(start->arg);
		free(start);
		start = next_to_free;
	}
}

int __is_builtin(char **arg)
{
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
    return (0);
}

void	__exec_builtin(char **arg, t_msh *msh)
{

	if (__strcmp(arg[0], "echo") == 0)
        msh->rv =__echo(arg, 1);
	if (__strcmp(arg[0], "cd") == 0)
        msh->rv = __cd(arg[1], msh);
	if (__strcmp(arg[0], "pwd") == 0)
        msh->rv = __pwd(1);
	if (__strcmp(arg[0], "env") == 0)
        msh->rv = __env(msh);
	if (__strcmp(arg[0], "export") == 0)
        msh->rv = __export(arg + 1, msh);
	if (__strcmp(arg[0], "unset") == 0)
        msh->rv = __unset(arg + 1, msh);
}

int	__save_fd(int *std)
{
	std[out] = dup(STDOUT_FILENO);
	std[in] = dup(STDIN_FILENO);
	if (std[out] < 0|| std[in] < 0)
		return (0);
	return (1);
}

int __restore_fd(int *std)
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

int execute_seq(t_cmd *cmd, t_msh *msh)
{
    t_sequ seq;
	int std[2];

    if (__find_max_cmd(cmd) == 1 && __is_builtin(cmd->arg))
	{
		fprintf(stderr, " I m a builtin\n");
		if(cmd->redirect)
		{
			if (!__save_fd(std))
				return (0);
			__handle_redirect_builtin(cmd);
		}
		__exec_builtin(cmd->arg, msh);
		if (!__restore_fd(std))
			return (0);
		__cmd_list_clear(cmd);
        return (0);
	}
    if (!__init_seq(&seq, msh->envp, cmd))
        return (__putstr_fd("Malloc error\n", 2), 0);
    msh->rv = __launcher_fork(&seq, cmd);
	free_split(seq.path);
	free(seq.envp);
	__cmd_list_clear(cmd);
    return (0);
}

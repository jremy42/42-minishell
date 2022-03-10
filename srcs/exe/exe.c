/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:20:32 by jremy             #+#    #+#             */
/*   Updated: 2022/03/10 16:43:07 by jremy            ###   ########.fr       */
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
		free_split(start->arg);
		free(start);
		start = next_to_free;
	}
}

int __is_builtin(char **arg)
{
	if (__strncmp(arg[0], "echo", 4) == 0)
        return (1);
	if (__strncmp(arg[0], "cd", 2) == 0)
         return (1);
	if (__strncmp(arg[0], "pwd", 3) == 0)
         return (1);
	if (__strncmp(arg[0], "env", 3) == 0)
        return (1);
	if (__strncmp(arg[0], "export", 6) == 0)
         return (1);
	if (__strncmp(arg[0], "unset", 5) == 0)
        return (1);
    return (0);
}

void	__exec_builtin(char **arg, t_msh *msh)
{

	if (__strncmp(arg[0], "echo", 4) == 0)
        msh->rv =__echo(arg, 1);
	if (__strncmp(arg[0], "cd", 2) == 0)
        msh->rv = __cd(arg[1], msh);
	if (__strncmp(arg[0], "pwd", 3) == 0)
        msh->rv = __pwd(1);
	if (__strncmp(arg[0], "env", 3) == 0)
        msh->rv = __env(msh);
	if (__strncmp(arg[0], "export", 6) == 0)
        msh->rv = __export(arg + 1, msh);
	if (__strncmp(arg[0], "unset", 5) == 0)
        msh->rv = __unset(arg + 1, msh);
}

int execute_seq(t_cmd *cmd, t_msh *msh)
{
    t_sequ seq;

    if (!__init_seq(&seq, msh->envp, cmd))
        return (__putstr_fd("Malloc error\n", 2), 0);
    if (seq.max_cmd == 1 && __is_builtin(cmd->arg))
	{
		__exec_builtin(cmd->arg, msh);
		free_split(seq.path);
		free(seq.envp);
		__cmd_list_clear(cmd);
        return (0);
	}
    msh->rv = __launcher_fork(&seq, cmd);
	free_split(seq.path);
	free(seq.envp);
	__cmd_list_clear(cmd);
    return (0);
}

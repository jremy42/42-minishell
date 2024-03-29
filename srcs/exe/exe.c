/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:18:33 by jremy             #+#    #+#             */
/*   Updated: 2022/04/14 15:59:35 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_node_clear(t_cmd *start)
{
	t_cmd	*next_to_free;

	while (start)
	{
		next_to_free = start->next;
		__redirect_list_clear(start->redirect);
		free(start->arg);
		free(start);
		start = next_to_free;
	}
}

void	__cmd_full_list_clear(t_cmd *start)
{
	t_cmd	*next_to_free;

	while (start)
	{
		next_to_free = start->next;
		__redirect_list_clear(start->redirect);
		free_split(start->arg);
		free(start->arg);
		free(start);
		start = next_to_free;
	}
}

char	*__get_name(int index)
{
	char	*tmp;
	char	*cmd_nbr;

	cmd_nbr = __itoa(index);
	if (!cmd_nbr)
		return (NULL);
	tmp = __strjoin(__strdup(".hd.tmp"), cmd_nbr);
	if (!tmp)
		return (free(cmd_nbr), NULL);
	return (free(cmd_nbr), tmp);
}

int	del_hd(t_cmd *cmd)
{
	t_redirect	*save;
	char		*tmp;

	while (cmd)
	{
		save = cmd->redirect;
		while (cmd->redirect)
		{
			if (cmd->redirect->type == H_D)
			{
				tmp = __get_name(cmd->index);
				if (!tmp)
					return (0);
				unlink(tmp);
				free(tmp);
				break ;
			}
			cmd->redirect = cmd->redirect->next;
		}
		cmd->redirect = save;
		cmd = cmd->next;
	}
	return (1);
}

int	execute_seq(t_cmd *cmd, t_msh *msh)
{
	t_sequ		seq;
	int			std[2];

	if (__find_max_cmd(cmd) == 1 && !__update_underscore(cmd->arg, msh))
		return (cmd_node_clear(cmd), msh->rv);
	if (__find_max_cmd(cmd) == 1 && __is_builtin(cmd->arg))
	{
		if (cmd->redirect)
		{
			if (!__save_fd(std))
				return (cmd_node_clear(cmd), 0);
			if (!__handle_redirect(cmd))
				return (del_hd(cmd), __restore_fd(std), cmd_node_clear(cmd), 0);
		}
		__exec_builtin(cmd->arg, msh, cmd, 0);
		if (cmd->redirect && !__restore_fd(std))
			__putendl_fd(strerror(errno), 2);
		return (del_hd(cmd), cmd_node_clear(cmd), msh->rv);
	}
	if (!__init_seq(&seq, msh->envp, cmd))
		return (cmd_node_clear(cmd), __putstr_fd("Malloc err\n", 2), 0);
	msh->rv = __launcher_fork(&seq, cmd, cmd);
	del_hd(cmd);
	return (free_split(seq.path), free(seq.envp),
		cmd_node_clear(cmd), 1);
}

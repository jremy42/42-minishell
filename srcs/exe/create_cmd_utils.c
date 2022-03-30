/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:13:16 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 18:27:10 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

void	__cmd_add_back(t_cmd **alst, t_cmd *new)
{
	t_cmd	*nextlst;

	if (!(*alst))
		*alst = new;
	else
	{
		nextlst = (*alst);
		while (nextlst->next)
			nextlst = nextlst->next;
		nextlst->next = new;
	}
}

int	print_cmd_lst(t_cmd *cmd)
{
	while (cmd)
	{
		print_cmd(cmd);
		cmd = cmd->next;
	}
	return (1);
}

int	print_cmd(t_cmd *cmd)
{
	int			i;
	t_redirect	*tmp;

	i = 0;
	tmp = cmd->redirect;
	while (tmp)
	{
		DEBUG && printf("type = %d et file = %s\n", tmp->type, tmp->file_name);
		tmp = tmp->next;
	}
	while (cmd->arg[i])
	{
		DEBUG && printf("arg %d >%s<\n", i, cmd->arg[i]);
		i++;
	}
	return (1);
}

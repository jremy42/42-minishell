/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:19:12 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 11:39:26 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static void	__cmd_add_back(t_cmd **alst, t_cmd *new)
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

static int	__get_nb_param_cmd(t_lexing *start)
{
	int	size;

	size = 0;
	while (start && start->type != PIPE)
	{
		if (start->type == REDIRECTION || start->type == HERE_DOC)
		{
			start = start->next->next;
			continue ;
		}
		if (start->type == P_LEFT || start->type == P_RIGHT)
		{
			start = start->next;
			continue ;
		}
		size++;
		start = start->next;
	}
	return (size);
}

static t_cmd	*create_new_cmd(int nb_param, int index, t_msh *msh)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return(NULL);
	new->next = NULL;
	new->redirection[0] = -1;
	new->redirection[1] = -1;
	new->redirect = NULL;
	new->index = index;
	new->msh = msh;
	new->arg = malloc(sizeof(char *) * (nb_param + 1));
	if (!new->arg)
		return (free(new), NULL);
	new->arg[nb_param] = NULL;
	return (new);
}

int	create_cmd_arg_and_redirection(t_lexing **lexing, t_cmd *new)
{
	int			i;

	i = 0;
	while (*lexing && (*lexing)->type != PIPE)
	{
		if ((*lexing)->type == P_LEFT || (*lexing)->type == P_RIGHT)
		{
			*lexing = (*lexing)->next;
			continue ;
		}
		if ((*lexing)->type == REDIRECTION || (*lexing)->type == HERE_DOC)
		{
			if (!__add_redirect(new, *lexing))
				return (__redirect_list_clear(new->redirect), (free(new->arg), free(new), 0));
			*lexing = (*lexing)->next->next;
			continue ;
		}
		new->arg[i] = (*lexing)->token;
		*lexing = (*lexing)->next;
		i++;
	}
	return (1);
}

int	add_next_cmd(t_cmd **start, t_lexing **lexing, t_msh *msh, int index)
{
	t_cmd		*new;

	new = create_new_cmd(__get_nb_param_cmd(*lexing), index, msh);
	if (!new)
		return (0);
	if(!create_cmd_arg_and_redirection(lexing, new))
		return (0);
	if (*lexing)
		*lexing = (*lexing)->next;
	__cmd_add_back(start, new);
	return (1);
}

t_cmd	*create_cmd_list(t_lexing *lexing, t_msh *msh)
{
	t_cmd		*start;
	t_lexing	*save;
	int			i;

	i = 0;
	save = lexing;
	start = NULL;
	while (lexing)
	{
		if (!add_next_cmd(&start, &lexing, msh, i))
			return (__cmd_node_list_clear(start), NULL);
		i++;
	}
	return (start);
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
	DEBUG && printf("stdin = %d stdout = %d, index = %d\n", cmd->redirection[0], cmd->redirection[1], cmd->index);
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

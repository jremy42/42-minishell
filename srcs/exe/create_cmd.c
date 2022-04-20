/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:19:12 by jremy             #+#    #+#             */
/*   Updated: 2022/04/08 15:29:01 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (start->empty == 0)
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
		return (NULL);
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
				return (__redirect_list_clear(new->redirect),
					(free(new->arg), free(new), 0));
			*lexing = (*lexing)->next->next;
			continue ;
		}
		if ((*lexing)->empty == 0)
			new->arg[i++] = (*lexing)->token;
		*lexing = (*lexing)->next;
	}
	return (1);
}

int	add_next_cmd(t_cmd **start, t_lexing **lexing, t_msh *msh, int index)
{
	t_cmd		*new;

	new = create_new_cmd(__get_nb_param_cmd(*lexing), index, msh);
	if (!new)
		return (0);
	if (!create_cmd_arg_and_redirection(lexing, new))
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
			return (cmd_node_clear(start), NULL);
		i++;
	}
	return (start);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 17:54:50 by jremy             #+#    #+#             */
/*   Updated: 2022/04/01 16:02:18 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

void	__redirect_list_clear(t_redirect *start)
{
	t_redirect	*next_to_free;

	while (start)
	{
		next_to_free = start->next;
		free(start);
		start = next_to_free;
	}
}

void	__redirect_add_back(t_redirect **alst, t_redirect *new)
{
	t_redirect	*nextlst;

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

t_redirect	*__create_new_redirect(t_lexing *lexing)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->file_name = lexing->next->token;
	if (__strcmp(lexing->token, "<") == 0)
		new->type = LESS;
	if (__strcmp(lexing->token, ">") == 0)
		new->type = GREAT;
	if (__strcmp(lexing->token, ">>") == 0)
		new->type = DGREAT;
	if (__strcmp(lexing->token, "<<") == 0)
		new->type = H_D;
	return (new);
}

int	__add_redirect(t_cmd *cmd, t_lexing *lexing)
{
	t_redirect	*new;

	new = __create_new_redirect(lexing);
	if (!new)
		return (0);
	__redirect_add_back(&(cmd->redirect), new);
	return (1);
}

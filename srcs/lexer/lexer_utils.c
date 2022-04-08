/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 11:50:42 by jremy             #+#    #+#             */
/*   Updated: 2022/04/08 14:41:45 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexing	*__lexnew_force_word(char *content)
{
	t_lexing	*newlst;

	newlst = malloc(sizeof(t_lexing));
	if (!newlst)
		return (NULL);
	newlst->token = __strdup(content);
	if (!newlst->token)
		return (free(newlst), NULL);
	newlst->type = WORD;
	newlst->next = NULL;
	newlst->hd_type = 0;
	newlst->empty = 0;
	return (newlst);
}

int	__count_node(t_lexing *lexing)
{
	int	i;

	i = 0;
	while (lexing)
	{
		if (lexing->type == OPERATOR)
			i++;
		lexing = lexing->next;
	}
	return (2 * i + 1);
}

void	__lex_fclear(t_lexing **start)
{
	t_lexing	*next_to_free;

	if (!start)
		return ;
	while (*start && (*start)->token)
	{
		next_to_free = (*start)->next;
		free((*start)->token);
		(*start)->token = NULL;
		free(*start);
		*start = NULL;
		*start = next_to_free;
	}
}

void	__lexing_node_list_clear(t_lexing *start)
{
	t_lexing	*next_to_free;

	while (start)
	{
		next_to_free = start->next;
		free(start);
		start = next_to_free;
	}
}

int	__is_operator(char *content)
{
	int	size;

	size = __strlen(content);
	if (size == 1 && __is_operator_char(content[0]) && content[0] != '&')
		return (1);
	if (size == 2)
	{
		if (content[0] == content[1])
			return (1);
	}
	return (0);
}

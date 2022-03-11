/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/11 12:58:47 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__lexing_full_list_clear(t_lexing *start)
{
	t_lexing	*next_to_free;

	while (start)
	{
		next_to_free = start->next;
		free(start->token);
		free(start);
		start = next_to_free;
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
	if (size == 1 && __is_operator_char(content[0]) && content[0]!= '&')
		return (1);
	if (size == 2)
	{
		if (content[0] == content[1])
			return (1);
	}
	return (0);
}

static int __is_redirection(char *content)
{
	if (!__strcmp(content, "<"))
		return (1);
	if (!__strcmp(content, ">"))
		return (1);
	if (!__strcmp(content, "<<"))
		return (2);
	if (!__strcmp(content, ">>"))
		return (1);
	return (0);
}

t_token_type	__get_type(char *content)
{
	int	i;

	i = 0;
	if (content[0] == '(')
		return (P_LEFT);
	if (content[0] == ')')
		return (P_RIGHT);
	if (!__strcmp("<newline>", content))
		return (NEW_LINE);
	if (!__strcmp(content, "|"))
		return (PIPE);
	if (__is_redirection(content) == 1)
		return (REDIRECTION);
	if (__is_redirection(content) == 2)
		return (HERE_DOC);
	if (!__is_operator_char(content[i]))
		return (WORD);
	if (__is_operator(content) == 1)
		return (OPERATOR);
	else
		return (INVALID);
}

t_lexing	*__lexnew(char *content)
{
	t_lexing	*newlst;

	newlst = malloc(sizeof(t_lexing));
	if (!newlst)
		return (NULL);
	newlst->token = __strdup(content);
	if (!newlst->token)
		return (NULL);
	newlst->type = __get_type(content);
	newlst->next = NULL;
	return (newlst);
}

void	__lexadd_back(t_lexing **alst, t_lexing *new)
{
	t_lexing	*nextlst;

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

int	__lexing(t_list *token, t_lexing **lexing)
{
	t_list		*tmp;
	t_lexing	*new;

	tmp = token;
	while (tmp)
	{
		new = __lexnew((char *)tmp->content);
		if (!new)
			return (-1);
		__lexadd_back(lexing, new);
		tmp = tmp->next;
	}
	__lstclear(&token, &free);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/11 15:20:43 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lex	*__lstnew(void *content)
{
	t_lex	*newlst;

	newlst = malloc(sizeof(t_lex));
	if (!newlst)
		return (NULL);
	newlst->token = content;
	newlst->next = NULL;
	return (newlst);
}

int	__lexing(t_list *token, t_lex *lexing)
{
	t_list *tmp;

	tmp = token
	while (tmp)
	{
	
		tmp = tmp->next;
	}
	__lstclear(token, &free);
}
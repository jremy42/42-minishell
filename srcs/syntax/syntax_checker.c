/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 17:58:11 by jremy             #+#    #+#             */
/*   Updated: 2022/04/13 12:24:45 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	__check_lexing(t_lexing *lexing, t_msh *msh)
{
	if (lexing->type == OPERATOR || lexing->type == PIPE
		|| lexing->type == REDIRECTION || lexing->type == HERE_DOC)
	{
		if (!lexing->next || lexing->next->type == OPERATOR
			|| lexing->next->type == PIPE
			|| lexing->next->type == REDIRECTION
			|| lexing->next->type == NEW_LINE
			|| lexing->next->type == P_RIGHT)
		{
			if (lexing->type == PIPE && lexing->next
				&& (lexing->next->type == REDIRECTION
					|| lexing->next->type == HERE_DOC))
				return (1);
			msh->rv = 2;
			msh->syntax_error = 2;
			return (0);
		}
	}
	return (1);
}

static t_lexing	*__check_parenthesis(t_msh *msh, t_lexing *lexing, int p)
{
	if (p != 0)
	{
		msh->rv = 2;
		msh->syntax_error = 2;
		return (__putendl_fd("end. Parenthesis no closed", 2), lexing);
	}
	return (NULL);
}

t_lexing	*__check_lexing_if_no_word(t_lexing *lexing, t_msh *msh)
{
	int	word;

	word = 0;
	if (lexing && (lexing->type == OPERATOR || lexing->type == PIPE))
	{
		msh->rv = 2;
		msh->syntax_error = 2;
		return (lexing);
	}
	while (lexing)
	{
		if (lexing->type == WORD)
			return (NULL);
		lexing = lexing->next;
	}
	msh->rv = 2;
	msh->syntax_error = 2;
	return (lexing);
}

t_lexing	*__check_lexing_if_func(t_lexing *lexing, t_msh *msh)
{
	int	word;

	word = 0;
	while (lexing)
	{
		if (lexing->type == P_LEFT && lexing->next
			&& (lexing->next->type == P_RIGHT
				|| lexing->next->type == OPERATOR
				|| lexing->next->type == PIPE))
		{
			msh->rv = 2;
			msh->syntax_error = 2;
			return (lexing);
		}
		lexing = lexing->next;
	}
	return (NULL);
}

t_lexing	*__synthax_checker(t_lexing *lexing, t_msh *msh)
{
	int	parenthesis;

	parenthesis = 0;
	if (__check_lexing_if_no_word(lexing, msh))
		return (__synthax_error(__check_lexing_if_no_word(lexing, msh), msh));
	if (__check_lexing_if_func(lexing, msh))
		return (__synthax_error(__check_lexing_if_func(lexing, msh), msh));
	while (lexing)
	{
		if (lexing->type == P_LEFT)
			parenthesis++;
		if (lexing->type == P_RIGHT)
			parenthesis--;
		if (lexing->type == INVALID)
			return (__invalid_error(lexing->token, msh), lexing);
		if (!__check_lexing(lexing, msh))
			return (__synthax_error(lexing, msh));
		if (parenthesis < 0)
			return (__synthax_error(lexing, msh));
		lexing = lexing->next;
	}
	return (__check_parenthesis(msh, lexing, parenthesis));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 17:58:11 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 11:55:10 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	__check_lexing(t_lexing *lexing, t_msh *msh)
{
	if (lexing->type == OPERATOR || lexing->type == PIPE
		|| lexing->type == REDIRECTION)
	{
		if (!lexing->next || lexing->next->type == OPERATOR
			|| lexing->next->type == PIPE
			|| lexing->next->type == REDIRECTION
			|| lexing->next->type == NEW_LINE)
		{
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
		return (__synthax_error("end. Parenthesis no closed"), lexing);
	}
	return (NULL);
}

t_lexing	*__synthax_checker(t_lexing *lexing, t_msh *msh)
{
	int	parenthesis;

	parenthesis = 0;
	while (lexing)
	{
		if (lexing->type == P_LEFT)
			parenthesis++;
		if (lexing->type == P_RIGHT)
			parenthesis--;
		if (lexing->type == INVALID)
			return (__invalid_error(lexing->token), lexing);
		if (!__check_lexing(lexing, msh))
			return (__synthax_error(lexing->token), lexing);
		if (parenthesis < 0)
		{
			msh->rv = 2;
			msh->syntax_error = 2;
			return (__synthax_error(lexing->token), lexing);
		}
		lexing = lexing->next;
	}
	return (__check_parenthesis(msh, lexing, parenthesis));
}

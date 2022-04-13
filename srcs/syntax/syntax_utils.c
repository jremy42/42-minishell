/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 11:53:34 by jremy             #+#    #+#             */
/*   Updated: 2022/04/13 12:17:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__is_valide_operator(char *token)
{
	if (!__strncmp(token, "&&", 2))
		return (1);
	if (!__strncmp(token, "||", 2))
		return (1);
	if (!__strncmp(token, "|", 1))
		return (1);
	return (0);
}

t_lexing	*__synthax_error(t_lexing *lexing, t_msh *msh)
{
	write(2, "minishell: syntax error near :", 30);
	if (lexing && lexing->token)
		write(2, lexing->token, __strlen(lexing->token));
	write(2, "\n", 1);
	msh->rv = 2;
	msh->syntax_error = 2;
	return (lexing);
}

void	__invalid_error(char *str, t_msh *msh)
{
	write(2, "minishell: Valid token but not handled in minishell:", 52);
	write(2, str, __strlen(str));
	write(2, "\n", 1);
	msh->rv = 2;
	msh->syntax_error = 2;
}

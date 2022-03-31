/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 11:53:34 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 11:53:56 by jremy            ###   ########.fr       */
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

void	__synthax_error(char *str)
{
	write(2, "minishell: syntax error near :", 19);
	write(2, str, __strlen(str));
	write(2, "\n", 1);
}

void	__invalid_error(char *str)
{
	write(2, "minishell: Valid token but not handled in minishell:", 42);
	write(2, str, __strlen(str));
	write(2, "\n", 1);
}

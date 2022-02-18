/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/18 17:09:32 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__skip_spaces(char *str, int *i)
{
	while (str[*i] == ' ')
		(*i)++;
	return (*i);
}

int	__init_token_if_none(char **str, int *token_status)
{
	if (!*token_status)
	{
		*str = __strdup("");
		if (!*str)
			return (0);
		*token_status = 1;
		return (1);
	}
	return (1);
}

int	__treat_newline(t_list **start, int *i)
{
	char *new_line;

	new_line = __strdup("<newline>");
	if (!new_line)
	{
		*i = -1;
		return (-1);
	}
	if (__add_token(new_line, start) < 0)
	{
		free(new_line);
		*i = -1;
		return (-1);
	}
	return (1);
}

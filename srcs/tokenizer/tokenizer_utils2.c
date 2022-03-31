/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 11:55:42 by jremy            ###   ########.fr       */
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

int	__treat_newline(t_list **start, t_msh *msh)
{
	char	*new_line;

	new_line = __strdup("\n");
	if (!new_line)
	{
		msh->rv = 2;
		__putendl_fd("Malloc Error", 2);
		return (__lstclear(start, free), __exit(msh));
	}
	if (__add_token(new_line, start) < 0)
	{
		free(new_line);
		msh->rv = 2;
		__putendl_fd("Malloc Error", 2);
		return (__lstclear(start, free), __exit(msh));
	}
	return (1);
}

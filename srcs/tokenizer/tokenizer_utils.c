/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/10 18:03:00 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__is_operator_char(char c)
{
	if (__strchr("&|<>", c))
		return (1);
	return (0);
}

int	__need_to_escape(int i, t_state state, char *str)
{
	if (state == S_QUOTE)
		return (0);
	if (state == D_QUOTE && __strchr("$`\\\"", str[i + 1]))
		return (1);
	if (state == UNQUOTE)
		return (1);
	return (1);
}

char	*__add_char_nt(char *tmp, char c, char **new_token)
{
	tmp = __strdup(" ");
	tmp[0] = c;
	return (__strjoin(*new_token, tmp));
}

int	__return_state(char c, int state, int slash_state)
{
	if (c == 34 && state == D_QUOTE && slash_state)
		state = UNQUOTE;
	else if (c == 39 && state == S_QUOTE && slash_state)
		state = UNQUOTE;
	else if (state == UNQUOTE)
		state = (c == 39) * S_QUOTE + (c == 34) * D_QUOTE;
	return (state);
}
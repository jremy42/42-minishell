/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/04/05 18:36:14 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__is_operator_char(char c)
{
	if (__strchr("&|<>()", c))
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
	return (0);
}

int	__add_char_to_token(char c, char **token)
{
	char	*appended_token;
	int		i;
	int		token_len;

	i = 0;
	token_len = __strlen(*token);
	appended_token = (char *)malloc((token_len + 2) * sizeof(char));
	if (!appended_token)
	{
		free (*token);
		*token = NULL;
		return (0);
	}
	while (i < token_len)
	{
		appended_token[i] = (*token)[i];
		i++;
	}
	appended_token[token_len] = c;
	appended_token[token_len + 1] = '\0';
	free(*token);
	*token = appended_token;
	return (1);
}

t_state	__return_state(char c, int state, int slash_state)
{
	if (c == 34 && state == D_QUOTE && slash_state == 0)
		return (UNQUOTE);
	else if (c == 39 && state == S_QUOTE && slash_state == 0)
		return (UNQUOTE);
	else if (state == UNQUOTE && slash_state == 0)
		state = (c == 39) * S_QUOTE + (c == 34) * D_QUOTE;
	return (state);
}

int	__add_token(char *str, t_list **start)
{
	t_list	*t_token;

	if (!str)
		return (-1);
	t_token = __lstnew(str);
	if (!t_token)
		return (-1);
	__lstadd_back(start, t_token);
	return (0);
}

/*
34 = double quote
39 = simgple quote
*/

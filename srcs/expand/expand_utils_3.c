/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:25:17 by jremy             #+#    #+#             */
/*   Updated: 2022/04/07 14:26:27 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__is_an_exception(char *tmp, int i)
{
	if (tmp[i + 1] && __strchr("?'", tmp[i + 1]))
		return (1);
	if (tmp[i + 1] && tmp[i + 1] == '"')
	{
		if (__get_char_quote_status(&tmp[i], tmp) && tmp[i + 1] == '"')
			return (0);
		else
			return (1);
	}
	return (0);
}

void	__update_token_word(char **token_word, char **expd_tken_wd)
{
	free(*token_word);
	*token_word = *expd_tken_wd;
}

int	__get_dollar_status(char *str, char *to_find)
{
	t_state	slash_status;
	t_state	quote_status;
	int		i;

	quote_status = UNQUOTE;
	slash_status = 0;
	i = 0;
	while (str[i] && &str[i] != to_find)
	{
		if (str[i] == '\\'
			&& str[i + 1] && __need_to_escape(i, quote_status, str)
			&& !slash_status)
		{
			slash_status = BACKSLASH;
			i++;
			continue ;
		}
		quote_status = __return_state(str[i], quote_status, slash_status);
		i++;
		slash_status = 0;
	}
	if (!slash_status && (quote_status == UNQUOTE || quote_status == D_QUOTE))
		return (0);
	return (1);
}

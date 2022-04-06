/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:10:26 by jremy             #+#    #+#             */
/*   Updated: 2022/04/06 09:47:43 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__is_inside_quotes(char *str, char *to_find)
{
	t_state	s_status;
	t_state	q_status;
	int		i;

	q_status = UNQUOTE;
	s_status = 0;
	i = 0;
	while (str[i] && &str[i] != to_find)
	{
		if (str[i] == '\\' && str[i + 1] && __need_to_escape(i, q_status, str)
			&& !s_status)
		{
			s_status = BACKSLASH;
			i++;
			continue ;
		}
		q_status = __return_state(str[i], q_status, s_status);
		i++;
		s_status = 0;
	}
	if (__strchr("\\'\"", str[i]) && q_status != UNQUOTE)
		q_status = __return_state(str[i], q_status, s_status);
	return (q_status);
}

int	__is_a_real_unquoted_backslash(int i, char *tmp)
{
	int	status;

	status = __is_inside_quotes(tmp, &tmp[i]);
	if (tmp[i] == '\\' && __need_to_escape(i, status, tmp)
		&& !__get_char_quote_status(tmp, &tmp[i]))
		return (1);
	return (0);
}

int	__quote_remove(char **token_word)
{
	char	*tmp;
	int		i;
	char	*quote_removed_token_word;

	quote_removed_token_word = __strdup("");
	if (!quote_removed_token_word)
		return (0);
	tmp = *token_word;
	i = 0;
	while (tmp[i])
	{
		if (__is_a_real_unquoted_backslash(i, tmp) || ((tmp[i] == '\'' || tmp
					[i] == '"' ) && !__get_char_quote_status(tmp, &tmp[i])))
		{
			i++;
			continue ;
		}
		else
			if (!__add_char_to_token(tmp[i], &quote_removed_token_word))
				return (free(quote_removed_token_word), 0);
		i++;
	}
	free(*token_word);
	*token_word = quote_removed_token_word;
	return (1);
}

int	__quote_removal_token(t_lexing *lexing)
{
	while (lexing)
	{
		if (lexing->type == WORD && lexing->hd_type == 0)
		{
			if (!__quote_remove(&lexing->token))
				return (__putendl_fd("Malloc error", 2), 0);
		}
		lexing = lexing->next;
	}
	return (1);
}

int	__quote_removal_glob(t_glob *glob)
{
	while (glob)
	{
		if (glob->to_find)
		{
			if (!__quote_remove(&glob->to_find))
				return (__putendl_fd("Malloc error", 2), 0);
		}
		glob = glob->next;
	}
	return (1);
}

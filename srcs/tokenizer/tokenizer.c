/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/04/05 10:52:05 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__adjust_i(char *str, int i, int state, t_msh *msh)
{
	if (state != UNQUOTE)
	{
		msh->rv = 2;
		__putendl_fd("minishell: Error : Quote not closed", 2);
		return (__SYNTAX_ERROR);
	}
	if (__is_operator_char(str[i]))
		i--;
	if (!str[i])
		i--;
	return (i);
}

int	__get_word(char **new_token, char *str, int i, t_msh *msh)
{
	t_state	slash_status;
	t_state	quote_status;

	quote_status = UNQUOTE;
	slash_status = 0;
	while (str[++i])
	{
		if (str[i] == '\\' && str[i + 1]
			&& __need_to_escape(i, quote_status, str)
			&& !slash_status)
		{
			slash_status = BACKSLASH;
			if (!__add_char_to_token(str[i], new_token))
				return (__MALLOC_TOKEN);
			continue ;
		}
		quote_status = __return_state(str[i], quote_status, slash_status);
		if (quote_status == UNQUOTE && slash_status == 0)
			if (__is_operator_char(str[i]) || __strchr("\f\t\n\r\v() ", str[i]))
				break ;
		if (!__add_char_to_token(str[i], new_token))
			return (__MALLOC_TOKEN);
		slash_status = 0;
	}
	return (__adjust_i(str, i, quote_status, msh));
}

int	__get_operator(char **new_token, char *str, int i)
{
	while (str[i] && str[i] != ' ' && __is_operator_char(str[i]))
	{
		if (*new_token && __strlen(*new_token) == 1
			&& ((str[i] == ')' || str[i] == '(') || (str[i] != **new_token)))
			return (i - 1);
		if (!__add_char_to_token(str[i], new_token))
			return (-1);
		if (str[i] == '(' || str[i] == ')')
			break ;
		if (__strlen(*new_token) == 2)
			break ;
		i++;
	}
	if (!__is_operator_char(str[i]))
		--i;
	if (!str[i])
		i--;
	return (i);
}

int	__tokenize(char *str, t_list **start, t_msh *msh)
{
	char	*new_token;
	int		status;
	int		i;

	status = 0;
	i = -1;
	while (str[++i])
	{
		__skip_spaces(str, &i);
		if (str[i] == '\n' && __treat_newline(start, msh))
			continue ;
		if (i == -1 || !__init_token_if_none(&new_token, &status))
			return (__lstclear(start, free), __MALLOC_TOKEN);
		if (__is_operator_char(str[i]))
			i = __get_operator(&new_token, str, i);
		else
			i = __get_word(&new_token, str, i - 1, msh);
		if (i < 0)
			return (free(new_token), __lstclear(start, free), i);
		if (__add_token(new_token, start) < 0)
			return (free(new_token), __lstclear(start, free), __MALLOC_TOKEN);
		status = 0;
	}
	return (1);
}
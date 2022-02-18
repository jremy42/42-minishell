/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/18 12:48:03 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__adjust_i(char *str, int i, int state)
{
	if (state != UNQUOTE)
		return (24 - write(2, "Error : Quote not closed\n", 25));
	if (__is_operator_char(str[i]))
		i--;
	if (!str[i])
		i--;
	return (i);
}

int	__get_word(char **new_token, char *str, int i)
{
	t_state sl_st;
	t_state st;

	st = UNQUOTE;
	sl_st = 0;
	while (str[++i])
	{
		if (str[i] == '\\' && str[i + 1] && __need_to_escape(i, st, str))
		{
			sl_st = BACKSLASH;
			continue ;
		}
		st = __return_state(str[i], st, sl_st);
		if (st == UNQUOTE && sl_st == 0)
			if (__is_operator_char(str[i]) || __strchr("\f\t\n\r\v() ", str[i]))
				break ;
		if (!__add_char_to_token(str[i], new_token))
			return (-1);
		sl_st = 0;
	}
	return (__adjust_i(str, i, st));
}

int	__get_operator(char **new_token, char *str, int i)
{
	while (str[i] && str[i] != ' ' && __is_operator_char(str[i]))
	{
		if (*new_token && __strlen(*new_token) == 1
			&& (str[i] == ')' || str[i] == '('))
			break ;
		__add_char_to_token(str[i], new_token);
		if (str[i] == '(' || str[i] == ')')
			break ;
		if (__strlen(*new_token) == 2)
			break ;
		i++;
	}
	if (!__is_operator_char(str[i]))
		i--;
	if (!str[i])
		i--;
	return (i);
}

int	__tokenize2(char *str, char *new_token, int i, t_list **start)
{
	if (__is_operator_char(str[i]))
		i = __get_operator(&new_token, str, i);
	else
		i = __get_word(&new_token, str, i - 1);
	if (i < 0)
		return (free(new_token), -1);
	if (__add_token(new_token, start) < 0)
		return (-1);
	return (i);
}

int	__tokenize(char *str, t_list **start)
{
	t_token	token;
	int	i;

	i = -1;
	token = (t_token){.new_token = NULL};
	while (str && str[++i])
	{
		i = __skip_spaces(str, i);
		if (str[i] == '\n')
		{
			if (__add_token(__strdup("<newline>"), start) < 0)
				return (-1);
			continue ;
		}
		if (!token.status)
			token.status = __create_new_token(&token.new_token);
		i = __tokenize2(str, token.new_token, i, start);
		if (i < 0)
			return (-1);
		token.status = 0;
	}
	return (0);
}

/*

int main(int ac, char *av[])
{
	(void)ac;
	t_list	*start;
	t_list	*index;

	start = NULL;
	if (ac != 2)
		return 42;
	tokenize(av[1], &start);

	index = start;
	while (index)
	{
		printf("Le token vaut : >%s<\n", (char *)index->content); 
		index = index->next;
	}
	__lstclear(&start, &free);
	return (0);
}
*/

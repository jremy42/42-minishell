/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/11 14:07:00 by jremy            ###   ########.fr       */
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
	t_gw	gw;

	gw = (t_gw){.st = UNQUOTE, .sl_st = 0, .tmp = NULL};
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] && __need_to_escape(i, gw.st, str))
		{
			gw.sl_st = BACKSLASH;
			i++;
			continue ;
		}
		gw.st = __return_state(str[i], gw.st, gw.sl_st);
		if (gw.st == UNQUOTE && gw.sl_st == 0)
			if (__is_operator_char(str[i]) || __strchr("\f\t\n\r\v ", str[i]))
				break ;
		*new_token = __add_char_nt(gw.tmp, str[i], new_token);
		if (!new_token)
			return (-1);
		free(gw.tmp);
		i++;
		gw.sl_st = 0;
	}
	return (__adjust_i(str, i, gw.st));
}

int	__get_operator(char **new_token, char *str, int i)
{
	char	*tmp;

	while (str[i] && str[i] != ' ' && __is_operator_char(str[i]))
	{
		tmp = __strdup(" ");
		tmp[0] = str[i];
		*new_token = __strjoin(*new_token, tmp);
		free(tmp);
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
		i = __get_word(&new_token, str, i);
	if (i < 0)
		return (-1);
	if (__add_token(new_token, start) < 0)
		return (-1);
	return (i);
}

int	__tokenize(char *str, t_list **start)
{
	t_token	token;

	token = (t_token){.new_token = NULL};
	while (str && str[token.i])
	{
		if (str[token.i] == '\n')
		{
			if (__add_token(__strdup("<newline>"), start) < 0)
				return (-1);
			token.i++;
			continue ;
		}
		if (!token.status)
			token.status = __create_new_token(&token.new_token);
		token.i = __tokenize2(str, token.new_token, token.i, start);
		if (token.i < 0)
			return (-1);
		token.status = 0;
		token.i = __adjust_space(str, token.i);
	}
	//if (!__strlen(token.new_token))
	//	return (__add_token(__strdup("EOI"), start));
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

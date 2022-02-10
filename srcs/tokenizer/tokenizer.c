/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/10 18:14:12 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__adjust_i(char *str, int i, int state)
{
	(void)state;
	/*
	if (state != UNQUOTE)
		return (24 - write(2, "Error : Quote not closed\n", 25));
		*/
	if (__is_operator_char(str[i]))
		i--;
	if (!str[i])
		i--;
	return (i);
}

int		__get_word(char **new_token, char *str, int i)
{
	t_state	slash_state;
	t_state	state;
	char	*tmp;

	state = UNQUOTE;
	slash_state = 0;
	tmp = NULL;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] && __need_to_escape(i, state, str))
		{
			slash_state = BACKSLASH;
			i++;
			continue ;
		}
		state = __return_state(str[i], state, slash_state);
		printf(">state =%d slash_state = %d\n", state, slash_state);
		if (state == UNQUOTE && slash_state == 0)
			if (__is_operator_char(str[i]) || __strchr("\f\t\n\r\v", str[i]))
				break ;
		*new_token = __add_char_nt(tmp, str[i], new_token);
		free(tmp);
		i++;
		slash_state = 0;
	}
	return (__adjust_i(str, i, state));
}

int		__get_operator(char **new_token, char *str, int i)
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

int	__tokenize(char *s, t_list **start)
{
	int		status_token;
	char	*new_token;
	int		i;
	t_list	*t_token;
	char	*str;

	status_token = 0;
	i = 0;
	new_token = NULL;
	str = __strtrim(s, " \f\t\r\v");
	while (str && str[i])
	{
		if (str[i] == '\n')
		{
			t_token = __lstnew(__strdup("<newline>"));
			if (!t_token)
				return (0);
			__lstadd_back(start, t_token);
			i++;
			continue ;
		}
		if (!status_token)
		{
			new_token = __strdup("");
			status_token = 1;
		}
		if (__is_operator_char(str[i]))
			i = __get_operator(&new_token, str, i);
		else
			i = __get_word(&new_token, str, i);
		if (i < 0)
			return (-1);
		t_token = __lstnew(new_token);
		if (!t_token)
			return (-2);
		__lstadd_back(start, t_token);
		status_token = 0;
		i++;
		while (str[i] == ' ')
			i++;
	}
	if (!__strlen(new_token))
	{
			new_token = __strdup("EOI");
		t_token = __lstnew(new_token);
		if (!t_token)
			return (-2);
		__lstadd_back(start, t_token);
	}
	free(str);
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

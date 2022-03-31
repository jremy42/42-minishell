/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globe_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 09:52:39 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 12:23:35 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <string.h>

t_glob	*__glob_new(char *content, t_globe_type type, int size)
{
	t_glob	*newlst;

	newlst = malloc(sizeof(t_glob));
	if (!newlst)
		return (NULL);
	newlst->to_find = __strndup(content, size);
	if (!newlst->to_find && size)
		return (free(newlst), NULL);
	newlst->type = type;
	newlst->next = NULL;
	return (newlst);
}

int	__move_to_next_unquoted_charset(char *str, char *charset)
{
	t_state	s_status;
	t_state	q_status;
	int		i;

	q_status = UNQUOTE;
	s_status = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] && __need_to_escape(i, q_status, str)
			&& !s_status)
		{
			s_status = BACKSLASH;
			i++;
			continue ;
		}
		q_status = __return_state(str[i], q_status, s_status);
		if (!s_status && q_status == UNQUOTE
			&& __is_in_charset(str[i], charset))
			return (i);
		else
			i++;
		s_status = 0;
	}
	return (-1);
}

int	__move_to_next_unquoted_char(char *str, char c)
{
	t_state	s_status;
	t_state	q_status;
	int		i;

	q_status = UNQUOTE;
	s_status = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] && __need_to_escape(i, q_status, str)
			&& !s_status)
		{
			s_status = BACKSLASH;
			i++;
			continue ;
		}
		q_status = __return_state(str[i], q_status, s_status);
		if (!s_status && str[i] == c && q_status == UNQUOTE)
			return (i);
		else
			i++;
		s_status = 0;
	}
	return (-1);
}

void	__globadd_back(t_glob **alst, t_glob *new)
{
	t_glob	*nextlst;

	if (!(*alst))
		*alst = new;
	else
	{
		nextlst = (*alst);
		while (nextlst->next)
			nextlst = nextlst->next;
		nextlst->next = new;
	}
}

void	__glob_list_clear(t_glob *start)
{
	t_glob	*next_to_free;

	while (start)
	{
		next_to_free = start->next;
		free(start->to_find);
		free(start);
		start = next_to_free;
	}
}

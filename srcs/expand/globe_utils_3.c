/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globe_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 09:52:39 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 12:26:31 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <string.h>

void	__get_glob_size(int glob_len, int *i, int *j, char **tge)
{
	while (*i < glob_len && (*tge)[*i] == '*')
		(*i)++;
	while (*i < glob_len)
	{
		if ((*tge)[*i] == '*' && !__get_char_quote_status(*tge, &(*tge)[*i]))
			break ;
		(*i)++;
		(*j)++;
	}
}

int	__add_remaining_globs(char **tge, t_glob **glob, t_globe_type state)
{
	int	i;
	int	j;
	int	glob_len;

	i = 0;
	j = 0;
	glob_len = __strlen(*tge);
	while (i < glob_len)
	{
		if ((*tge)[i] == '*' && !__get_char_quote_status(*tge, &(*tge)[i]))
		{
			__get_glob_size(glob_len, &i, &j, tge);
			if (!__add_new_glob((*tge) + i, &state, glob, j))
				return (0);
			j = 0;
		}
		else
			i++;
	}
	return (1);
}

t_glob	*__create_glob_lst(char **to_glob_expand)
{
	t_glob			*glob;
	t_globe_type	state;
	int				i;

	glob = NULL;
	state = FIRST;
	i = 0;
	if ((*to_glob_expand)[0] == '*')
	{
		if (!__add_new_glob(*to_glob_expand, &state, &glob, 0))
			return (NULL);
	}
	else
	{
		i = __move_to_next_unquoted_char(*to_glob_expand, '*');
		if (!__add_new_glob((*to_glob_expand) + i, &state, &glob, i))
			return (NULL);
	}
	if (!__add_remaining_globs(to_glob_expand, &glob, state))
		return (NULL);
	return (glob);
}

int	__find_end(char *file_name, t_glob *glob_lst)
{
	char	*tmp;

	tmp = __strrstr(file_name, glob_lst->to_find);
	if (tmp && tmp[__strlen(glob_lst->to_find)] == 0)
		return (1);
	return (0);
}

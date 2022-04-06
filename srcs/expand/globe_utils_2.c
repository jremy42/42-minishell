/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globe_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 09:52:39 by jremy             #+#    #+#             */
/*   Updated: 2022/04/05 17:21:40by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <string.h>

int	__copy_dir_entries(t_list **dir_content, struct dirent *curr_dir, DIR *dp)
{
	char	*tmp;
	t_list	*new_entry;

	while (curr_dir)
	{
		tmp = __strdup(curr_dir->d_name);
		if (!tmp)
			return (__lstclear(dir_content, free), closedir(dp), 0);
		new_entry = __lstnew(tmp);
		if (!new_entry)
			return (free(tmp), __lstclear(dir_content, free),
				closedir(dp), 0);
		__lstadd_back(dir_content, new_entry);
		curr_dir = readdir(dp);
	}
	return (1);
}

int	__init_dir_content(t_list **dir_content)
{
	char			path[PATH_MAX];
	struct dirent	*curr_dir;
	DIR				*dp;

	if (!getcwd(path, PATH_MAX))
		return (1);
	dp = opendir(path);
	if (!dp)
		return (1);
	curr_dir = readdir(dp);
	if (!__copy_dir_entries(dir_content, curr_dir, dp))
		return (0);
	if (closedir(dp) < 0)
		return (__lstclear(dir_content, free), 0);
	return (1);
}

// "dsdsd"dsdd
int	__get_char_quote_status(char *s, char *to_find)
{
	t_state	s_stat;
	t_state	q_stat;
	int		i;

	q_stat = UNQUOTE;
	s_stat = 0;
	i = 0;
	while (s[i] && &s[i] != to_find)
	{
		if (s[i] == '\\' && s[i + 1] && __need_to_escape(i, q_stat, s)
			&& !s_stat)
		{
			s_stat = BACKSLASH;
			i++;
			continue ;
		}
		q_stat = __return_state(s[i], q_stat, s_stat);
		i++;
		s_stat = 0;
	}
	if (s[i] == '\\' && s[i + 1] && __need_to_escape(i, q_stat, s) && !s_stat)
		return (0);
	if (__strchr("\\'\"", s[i]) && q_stat != UNQUOTE)
		q_stat = __return_state(s[i], q_stat, s_stat);
	return ((s_stat || q_stat != UNQUOTE) * 1);
}

t_globe_type	__get_next_state(t_globe_type current_state, char *token)
{
	int	i;

	i = 0;
	while (token[i] && token[i] == '*'
		&& !__get_char_quote_status(token, &token[i]))
		i++;
	if (__move_to_next_unquoted_char(token + i, '*') < 0)
		return (LAST);
	if (current_state == FIRST)
		return (MIDDLE);
	return (MIDDLE);
}

int	__add_new_glob(char *to_glob_expand,
t_globe_type *state, t_glob **glob, int j)
{
	t_glob	*new;

	if (j == 0)
		new = __glob_new(NULL, *state, 0);
	else
		new = __glob_new(to_glob_expand - j, *state, j);
	if (!new)
		return (__glob_list_clear(*glob), 0);
	__globadd_back(glob, new);
	*state = __get_next_state(*state, to_glob_expand);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 09:52:39 by jremy             #+#    #+#             */
/*   Updated: 2022/04/01 15:32:05 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <string.h>

int	__file_find(char *f_name, t_glob *g_lst)
{
	char	*tmp;

	if (!g_lst)
		return (1);
	if (!g_lst->to_find || !g_lst->to_find[0])
		return (__file_find(f_name, g_lst->next));
	if (g_lst->type == 0)
	{
		if (!__strncmp(f_name, g_lst->to_find, __strlen(g_lst->to_find)))
			return (__file_find(f_name + __strlen(g_lst->to_find),
					g_lst->next));
		return (0);
	}
	if (g_lst->type == 2)
		return (__find_end(f_name, g_lst));
	else
	{
		tmp = __strstr(f_name, g_lst->to_find);
		if (!tmp)
			return (0);
		tmp += __strlen(g_lst->to_find);
		return (__file_find(tmp, g_lst->next));
	}
	return (0);
}

int	__insert_first_token(t_lexing *lexing, char *new_glob_match)
{
	char	*tmp;

	tmp = __strdup(new_glob_match);
	if (!tmp)
		return (0);
	free(lexing->token);
	lexing->token = tmp;
	return (1);
}

int	__insert_token(t_lexing *lexing,
	char *new_glob_match, int reset, t_lexing *true_end)
{
	static int	first = 1;
	t_lexing	*new_token;

	if (reset)
	{
		first = 1;
		return (1);
	}
	if (first)
	{
		first = 0;
		return (__insert_first_token(lexing, new_glob_match));
	}
	new_token = __lexnew(new_glob_match);
	if (!new_token)
		return (0);
	while (lexing->next != true_end)
		lexing = lexing->next;
	new_token->next = lexing->next;
	lexing->next = new_token;
	return (1);
}

int	__glob_expand_token(t_lexing *lexing,
	t_list *d_content, t_lexing *s_nxt, int take_hidden)
{
	t_glob		*glob_lst;
	char		*to_test;

	glob_lst = __create_glob_lst(&lexing->token);
	if (!glob_lst)
		return (__lstclear(&d_content, free), 0);
	if (!__quote_removal_glob(glob_lst))
		return (__glob_list_clear(glob_lst), __lstclear(&d_content, free), 0);
	while (d_content)
	{
		to_test = (char *)d_content->content;
		if ((__file_find(to_test, glob_lst)
				&& (take_hidden || to_test[0] != '.'))
			&& !__insert_token(lexing, (char *)d_content->content, 0, s_nxt))
			return (__glob_list_clear(glob_lst), 0);
		d_content = d_content->next;
	}
	__insert_token(NULL, NULL, 1, NULL);
	__glob_list_clear(glob_lst);
	return (1);
}

int	__handle_wildcards(t_lexing *lexing)
{
	t_list		*dir_content;
	t_list		*save;
	t_lexing	*save_next;
	int			take_hide;

	dir_content = NULL;
	if (!__init_dir_content(&dir_content))
		return (0);
	save = dir_content;
	while (lexing)
	{
		save_next = lexing->next;
		take_hide = (lexing->token[0] == '.');
		if (__move_to_next_unquoted_char(lexing->token, '*') >= 0
			&& !__glob_expand_token(lexing, dir_content, save_next, take_hide))
			return (__lstclear(&save, free), 0);
		dir_content = save;
		lexing = save_next;
	}
	__lstclear(&dir_content, free);
	return (__insert_token(NULL, NULL, 1, NULL));
}

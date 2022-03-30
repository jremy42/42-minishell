/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_globe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 09:52:39 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 14:19:05 by fle-blay         ###   ########.fr       */
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
	t_state s_status;
	t_state q_status;
	int i;

	q_status = UNQUOTE;
	s_status = 0;
	i = 0;
	while(str[i])
	{
		if (str[i] == '\\' && str[i + 1] && __need_to_escape(i, q_status, str)
			&& !s_status)
		{
			s_status = BACKSLASH;
			i++;
			continue ;
		}
		q_status = __return_state(str[i], q_status, s_status);
		if(!s_status && q_status == UNQUOTE && __is_in_charset(str[i], charset))
			return(i);
		else
			i++;
		s_status = 0;
	}
	return (-1);
}

int	__move_to_next_unquoted_char(char *str, char c)
{
	t_state s_status;
	t_state q_status;
	int i;

	q_status = UNQUOTE;
	s_status = 0;
	i = 0;
	while(str[i])
	{
		if (str[i] == '\\' && str[i + 1] && __need_to_escape(i, q_status, str)
			&& !s_status)
		{
			s_status = BACKSLASH;
			i++;
			continue ;
		}
		q_status = __return_state(str[i], q_status, s_status);
		if(!s_status && str[i] == c && q_status == UNQUOTE )
			return(i);
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

int	__copy_dir_entries(t_list **dir_content, struct dirent *curr_dir, DIR *dp)
{
    char    *tmp;
    t_list  *new_entry;

    while (curr_dir)
    {
        if(curr_dir->d_name[0] != '.')
        {
            tmp = __strdup(curr_dir->d_name);
            if (!tmp)
                return (__lstclear(dir_content, free), closedir(dp), 0);
            new_entry = __lstnew(tmp);
            if (!new_entry)
                return (free(tmp), __lstclear(dir_content, free),
					closedir(dp), 0);
            __lstadd_back(dir_content, new_entry);
        }
        curr_dir = readdir(dp);
    }
	return (1);
}

int   __init_dir_content(t_list **dir_content)
{
    char    path[PATH_MAX];
    struct  dirent *curr_dir;
    DIR     *dp;

    if (!getcwd(path, PATH_MAX))
        return (__putendl_fd("Minishell : getcwd: cannot access directories:",2)
			, 1);
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

void    __print_dir(t_list *dir_content)
{
    while (dir_content)
    {
        dir_content = dir_content->next;
    }
}



int	__get_char_quote_status(char *str, char *to_find)
{
	t_state s_status;
	t_state q_status;
	int i;

	q_status = UNQUOTE;
	s_status = 0;
	i = 0;
	while(str[i] && &str[i] != to_find)
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
	if(!s_status && q_status == UNQUOTE )
			return(0);
	return (1);
}


t_globe_type __get_next_state(t_globe_type current_state, char *token)
{
	int i;

	i = 0;
	
	while(token[i] && token[i] == '*'
		&& !__get_char_quote_status(token, &token[i]))
		i++;
	if (__move_to_next_unquoted_char(token + i, '*') < 0)
		return (LAST);
	if (current_state == FIRST)
		return (MIDDLE);	
	return (MIDDLE);
}

int __add_new_glob(char *to_glob_expand,
t_globe_type *state, t_glob **glob, int j)
{
	t_glob	*new;
	
	if (j == 0)
		new = __glob_new(NULL, *state, 0);
	else
		new = __glob_new(to_glob_expand - j, *state, j);
	if (!new)
		return (__glob_list_clear(*glob),0);
	__globadd_back(glob, new);
	*state = __get_next_state(*state, to_glob_expand);
	return (1);
}

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


int __add_remaining_globs(char **tge, t_glob **glob, t_globe_type state)
{
	int i;
	int j;
	int glob_len;
	
	i = 0;
	j = 0;
	glob_len = __strlen(*tge);
	while (i < glob_len)
    {
		if ((*tge)[i] == '*' && !__get_char_quote_status(*tge, &(*tge)[i]))
		{
			__get_glob_size(glob_len, &i, &j, tge);
			if(!__add_new_glob((*tge) + i, &state, glob, j))
				return (0);
			j = 0;
		}
		else
			i++;
    }
	return (1);
}

t_glob *__create_glob_lst(char **to_glob_expand)
{
    t_glob  *glob;
	t_globe_type	state;
	int				i;
 
    glob = NULL;
	state = FIRST;
	i = 0;
	if ((*to_glob_expand)[0] == '*')
	{
		if(!__add_new_glob(*to_glob_expand, &state, &glob, 0))
			return (NULL);
	}
	else
	{
		i = __move_to_next_unquoted_char(*to_glob_expand, '*');
		if(!__add_new_glob((*to_glob_expand) + i, &state, &glob, i))
			return (NULL);
	}
	if (!__add_remaining_globs(to_glob_expand, &glob, state))
		return (NULL);
	return (glob);
}

int	__find_end(char *file_name, t_glob *glob_lst)
{
	char *tmp;

	tmp = __strrstr(file_name, glob_lst->to_find);
	if (tmp && tmp[__strlen(glob_lst->to_find)] == 0)
		return(1);	
	return (0);
}

int __file_find(char *f_name, t_glob *g_lst)
{
	char *tmp;

	if (!g_lst)
		return (1);
	if(!g_lst->to_find || !g_lst->to_find[0])
		return(__file_find(f_name,g_lst->next));
	if (g_lst->type == 0)
	{
		if(!__strncmp(f_name, g_lst->to_find, __strlen(g_lst->to_find)))
			return(__file_find(f_name + __strlen(g_lst->to_find), g_lst->next));
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
	char *tmp;

	tmp = __strdup(new_glob_match);
	if (!tmp)
		return (0);
	free(lexing->token);
	lexing->token = tmp;
	return (1);
}

int __insert_token(t_lexing *lexing,
	char *new_glob_match, int reset, t_lexing *true_end)
{
	static int first = 1;
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

int	__glob_expand_token(t_msh *msh, t_lexing *lexing,
	t_list *d_content, t_lexing *s_nxt)
{
	t_glob		*glob_lst;

	glob_lst = __create_glob_lst(&lexing->token);
	if (!glob_lst)
		return (__lstclear(&d_content, free), 0);
	if(!__quote_removal_glob(glob_lst, msh))
		return ( __glob_list_clear(glob_lst),__lstclear(&d_content, free), 0);
	while (d_content)
	{
		if (__file_find((char *)d_content->content, glob_lst)
			&& !__insert_token(lexing, (char *)d_content->content, 0, s_nxt))
				return ( __glob_list_clear(glob_lst), 0);
		d_content = d_content->next;
	}
	__insert_token(NULL, NULL, 1, NULL);
	__glob_list_clear(glob_lst);
	return (1);
}

int    __handle_wildcards(t_msh *msh, t_lexing *lexing)
{
	t_list		*dir_content;
	t_list		*save;
	t_lexing	*save_next;

	dir_content = NULL;
    if (!__init_dir_content(&dir_content))
		return(0);
	save = dir_content;
	while(lexing)
	{
		save_next = lexing->next;
		if (__move_to_next_unquoted_char(lexing->token, '*') >= 0
			&& !__glob_expand_token(msh, lexing, dir_content, save_next))
			return (__lstclear(&save, free), 0);
		dir_content = save;
		lexing = save_next;
	}
	__lstclear(&dir_content, free);
	return (__insert_token(NULL, NULL, 1, NULL));
}

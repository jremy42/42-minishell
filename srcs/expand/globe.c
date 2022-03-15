/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 09:52:39 by jremy             #+#    #+#             */
/*   Updated: 2022/03/15 13:02:34 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

t_glob	*__glob_new(char *content, t_globe_type type, int size)
{
	t_glob	*newlst;

	newlst = malloc(sizeof(t_glob));
	if (!newlst)
		return (NULL);
	newlst->to_find = __strndup(content, size);
	fprintf(stderr, " add glob >%s< type >%d<\n", newlst->to_find, type);
	if (!newlst->to_find)
		return (NULL);
	newlst->type = type;
	newlst->next = NULL;
	return (newlst);
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

t_list    *__init_dir_content(void)
{
    char    path[PATH_MAX];
    struct  dirent *curr_dir;
    DIR     *dp;
    t_list  *dir_content;
    t_list  *new_entry;
    char    *tmp;

    dir_content = NULL;
    if (!getcwd(path, PATH_MAX))
        return (NULL);
    dp = opendir(path);
    if (!dp)
        return (NULL);
    curr_dir = readdir(dp);
    while (curr_dir)
    {
        if(curr_dir->d_name[0] != '.')
        {
            tmp = __strdup(curr_dir->d_name);
            if (!tmp)
                return (__lstclear(&dir_content, NULL), NULL);
            new_entry = __lstnew(tmp);
            if (!new_entry)
                return (__lstclear(&dir_content, NULL), NULL);
            __lstadd_back(&dir_content, new_entry);
        }
        curr_dir = readdir(dp);
    }
    if (closedir(dp) < 0)
        return (__lstclear(&dir_content, NULL), NULL);
    return (dir_content);
}

void    __print_dir(t_list *dir_content)
{
    while (dir_content)
    {
        fprintf(stderr, "next_entry from list: [%s]\n", (char *)dir_content->content);
        dir_content = dir_content->next;
    }
}

int    __load_dir(t_msh *msh)
{
    t_list    *dir_content;

    (void)msh;
    dir_content = __init_dir_content();
    if (!dir_content)
        return(0);
    __print_dir(dir_content);
    return (1);

}
/*
ake*i*le

token = ake
token->next = i;
token->next->next = le;
dir_content = Makefile
*/

t_globe_type __get_next_state(t_globe_type current_state, char *token)
{
	if (current_state == FIRST)
		return (MIDDLE);
	if (!strchr(token, '*'))
		return (MIDDLE);
	return (LAST);
}

t_glob *__create_glob_lst(char **to_glob_expand)
{
    t_glob  *glob;
	t_glob	*new;
    char    *save;
    int     i;
    int     j;
	t_globe_type	state;
    
    glob = NULL;
    save = *to_glob_expand;
	state = FIRST;
    i = 0;
	j = 0;
	fprintf(stderr,"coucou\n");
	if ((*to_glob_expand)[0] == '*')
	{
		new = __glob_new(NULL, state, 0);
		state = __get_next_state(state, (*to_glob_expand));
		if (!new)
			return (NULL);
		__globadd_back(&glob, new);
		while((*to_glob_expand)[i] && (*to_glob_expand)[i] == '*')
			i++;
	}
    while ((*to_glob_expand)[i])
    {
		if((*to_glob_expand)[i] == '*')
		{
			new = __glob_new((*to_glob_expand) + i - j, state, j);
			state = __get_next_state(state, (*to_glob_expand) + i);
			if (!new)
				return (__glob_list_clear(glob), NULL);
			__globadd_back(&glob, new);
			j = 0;
			while((*to_glob_expand)[i] && (*to_glob_expand)[i] == '*')
			{
				i++;
				break;
			}
		}
		j++;
		i++;
    }
	if (!(*to_glob_expand)[i])
	{
		new = __glob_new((*to_glob_expand) + i - j, state, j);
		state = __get_next_state(state, (*to_glob_expand) + i);
		if (!new)
			return (__glob_list_clear(glob), NULL);
		__globadd_back(&glob, new);
	}
	return (glob);
}

/*
int __file_find(char *dir_content, t_token *token)
{
    char *tmp;
    
    token = "ake";
    tmp = strchr(dir_content, token);
    if (!tmp)
        return (0);
    tmp + strlen(token);
    tmp = file;
    __file_find(tmp, token->next, state);
}
*/

void    __open_dir(t_msh *msh, t_lexing *lexing)
{
    __load_dir(msh);
	__create_glob_lst(&lexing->token);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 09:52:39 by jremy             #+#    #+#             */
/*   Updated: 2022/03/15 16:59 by jremy            ###   ########.fr       */
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
	DEBUG && fprintf(stderr, " add glob >%s< type >%d<\n", newlst->to_find, type);
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

t_globe_type __get_next_state(t_globe_type current_state, char *token)
{
	int i;

	i = 0;
	
	while(token[i] && token[i] == '*')
		i++;
	if (!strchr(token + i, '*'))
		return (LAST);
	if (current_state == FIRST)
		return (MIDDLE);	
	return (MIDDLE);
}

int __add_new_glob(char *to_glob_expand, t_globe_type *state, t_glob **glob, int j)
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

int __add_remaining_globs(char **to_glob_expand, t_glob **glob, t_globe_type state)
{
	int i;
	int j;
	int glob_len;
	
	i = 0;
	j = 0;
	glob_len = __strlen(*to_glob_expand);
	while (i < glob_len)
    {
		if((*to_glob_expand)[i] == '*')
		{
			while(i < glob_len && (*to_glob_expand)[i] == '*')
				i++;
			while(i < glob_len && (*to_glob_expand)[i] != '*' && i++)
				j++;
			if(!__add_new_glob((*to_glob_expand) + i, &state, glob, j))
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
		if(!__add_new_glob((*to_glob_expand), &state, &glob, 0))
			return (NULL);
	}
	else
	{
		while((*to_glob_expand)[i] && (*to_glob_expand)[i] != '*')
				i++;
		if(!__add_new_glob((*to_glob_expand) + i, &state, &glob, i))
			return (NULL);
	}
	if (!__add_remaining_globs(to_glob_expand, &glob, state))
		return (NULL);
	return (glob);
}


int __file_find(char *file_name, t_glob *glob_lst)
{
	char *tmp;

	if (!glob_lst)
		return (1);
	if(!glob_lst->to_find[0])
		return(__file_find(file_name,glob_lst->next));
	if (glob_lst->type == 0)
	{
		if(!__strncmp(file_name, glob_lst->to_find, __strlen(glob_lst->to_find)))
			return(__file_find(file_name + __strlen(glob_lst->to_find), glob_lst->next));
		return (0);
	}
	if(glob_lst->type == 2)
	{
		tmp = strstr(file_name, glob_lst->to_find);
		if (tmp && tmp[__strlen(glob_lst->to_find)] == 0)
			return(1);	
		return (0);
	}
	else
	{
		tmp = strstr(file_name, glob_lst->to_find);
    	if (!tmp)
			return (0);
   	 	tmp += __strlen(glob_lst->to_find);
		return (__file_find(tmp, glob_lst->next));
	}
	return (0);
}

int __insert_token(t_lexing *lexing, char *new_glob_match, int reset)
{
	static int first = 1;
	char *tmp;
	t_lexing	*new_token;

	if (reset)
	{
		first = 1;
		return (1);
	}
	if (first)
	{
		tmp = __strdup(new_glob_match);
		printf("tmp = [%s]\n", tmp);
		if (!tmp)
			return (0);
		first = 0;
		free(lexing->token);
		lexing->token = tmp;
		return (1);
	}
	new_token = __lexnew(new_glob_match);
	if (!new_token)
		return (0);
	new_token->next = lexing->next;
	lexing->next = new_token;
	return (1);
}

int    __handle_wildcards(t_msh *msh, t_lexing *lexing)
{
	t_list		*dir_content;
	t_list		*save;
	t_glob		*glob_lst;
	t_lexing	*save_next;

    (void)msh;
    dir_content = __init_dir_content();
    if (!dir_content)
		return(0);
	//__print_dir(dir_content);
	save = dir_content;
	while(lexing)
	{
		save_next = lexing->next;
		if(__strchr(lexing->token, '*'))
		{
			glob_lst = __create_glob_lst(&lexing->token);
			if (!glob_lst)
				return (0);
			while(dir_content)
			{
				if(__file_find((char *)dir_content->content, glob_lst))
				{
					printf("find = >%s<\n", (char *)dir_content->content);
					if (!__insert_token(lexing, (char *)dir_content->content, 0))
						return ( __glob_list_clear(glob_lst), __lstclear(&save, free), 0);
				}
				dir_content = dir_content->next;
			}
			__glob_list_clear(glob_lst);
		}
		dir_content = save;
		lexing = save_next;
	}
	__lstclear(&dir_content, free);
	return (__insert_token(NULL, NULL, 1));
}
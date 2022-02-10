/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:13:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/10 09:28:36 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

static char	*create_pwd(char *pwd, char *new_path)
{
	char	*tmp;

	tmp = NULL;
	tmp = __strdup(pwd); 
	if (!tmp)
		return (NULL);
	tmp = __strjoin(tmp, "/");
	if (!tmp)
		return (NULL);
	tmp = __strjoin(tmp, new_path);
	if (!tmp)
		return (NULL);
	return (tmp);
}

int	__cd(char *new_path)
{
	char 	*new_pwd;
	char	path[PATH_MAX];

	if (!new_path)
		chdir(getenv("HOME"));
	else if (new_path[0] == '-' && __strlen(new_path) == 1)
		chdir(getenv("OLDPWD"));
	else if (new_path[0] == '/')
		chdir(new_path);
	else
	{
		if (!getcwd(path, PATH_MAX))
			return (__FAIL);
		new_pwd = create_pwd(path, new_path);
		if (new_pwd)
		{
			chdir(new_pwd);
			free(new_pwd);
		}
		else
			return (__FAIL);
	}
	return (__SUCCESS);
}

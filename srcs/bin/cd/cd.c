/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:13:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/08 15:36:37 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

static char	*create_pwd(char *pwd, char *new_path)
{
	char	*tmp;

	tmp = NULL;
	tmp = __strdup(__strchr(pwd, '=') + 1); 
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

int	__cd(char *new_path, char *envp[])
{
	int		i;
	char 	*new_pwd;

	i = 0;
	if (new_path[0] == '/')
		chdir(new_path);
	else
	{
		while(envp[i] && __strncmp("PWD=", envp[i], 4))
			i++;
		if (!envp[i] || !__strchr(envp[i], '='))
			return (__FAIL);
		new_pwd = create_pwd(envp[i], new_path);
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

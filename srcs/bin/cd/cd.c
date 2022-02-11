/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:13:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/11 14:05:50 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

static char	*create_absolut_pwd(char *pwd, char *new_path)
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

static int update_oldpwd(t_msh *msh)
{
	char	*old_pwd;
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (__FAIL);
	old_pwd = __strdup("OLDPWD=");
	if (!old_pwd)
		return (__FAIL);
	old_pwd = __strjoin(old_pwd, cwd);
	if (!old_pwd)
		return (__FAIL);
	update_key_val(msh, old_pwd);
	free(old_pwd);
	return (__SUCCESS);
}

static int	update_pwd(t_msh *msh)
{
	char	*pwd;
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (__FAIL);
	pwd = __strdup("PWD=");
	if (!pwd)
		return (__FAIL);
	pwd = __strjoin(pwd, cwd);
	if (!pwd)
		return (__FAIL);
	update_key_val(msh, pwd);
	free(pwd);
	return (__SUCCESS);
}

static int	chdir_absolute_path(char *new_path)
{
	char	*new_pwd;
	char	path[PATH_MAX];

	if (!getcwd(path, PATH_MAX))
		return (__FAIL);
	new_pwd = create_absolut_pwd(path, new_path);
	if (!new_pwd)
		return (__FAIL);
	else
	{
		chdir(new_pwd);
		free(new_pwd);
	}
	return (__SUCCESS);
}

static int	chdir_previous(t_msh *msh)
{
	char	*save;

	save = NULL;
	if (!get_key(msh, "OLDPWD"))
		return (__FAIL);
	save = __strdup(get_key(msh, "OLDPWD"));
	if (!save)
		return(__FAIL);
	update_oldpwd(msh);
	chdir(save);
	free(save);
	return (__SUCCESS);
}

int	__cd(char *new_path, t_msh *msh)
{
	if (!new_path)
	{
		update_oldpwd(msh);
		chdir(get_key(msh, "HOME"));
	}
	else if (new_path[0] == '-' && __strlen(new_path) == 1)
		chdir_previous(msh);
	else
	{
		update_oldpwd(msh);
		if (new_path[0] == '/')
			chdir(new_path);
		else
			chdir_absolute_path(new_path);
	}
	update_pwd(msh);
	return (__SUCCESS);
}

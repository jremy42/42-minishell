/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 09:36:28 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/21 09:56:01 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_absolut_pwd(char *pwd, char *new_path)
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

int	update_oldpwd(t_msh *msh)
{
	char	*values[2];
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
	values[0] = old_pwd;
	values[1] = NULL;
	__export(values, msh);
	free(old_pwd);
	return (__SUCCESS);
}

int	update_pwd(t_msh *msh)
{
	char	*values[2];
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
	values[0] = pwd;
	values[1] = NULL;
	__export(values, msh);
	free(pwd);
	return (__SUCCESS);
}

int	chdir_absolute_path(char *new_path)
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

int	chdir_previous(t_msh *msh)
{
	char	*save;

	save = NULL;
	if (!get_key(msh, "OLDPWD"))
		return (__FAIL);
	save = __strdup(get_key(msh, "OLDPWD"));
	if (!save)
		return (__FAIL);
	update_oldpwd(msh);
	chdir(save);
	free(save);
	return (__SUCCESS);
}

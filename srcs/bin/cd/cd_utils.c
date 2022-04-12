/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 09:36:28 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/12 10:08:43 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*increment_shlvl(t_msh *msh)
{
	char	*tmp;

	if (key_exist(msh, "SHLVL=") < 0)
		return (__strdup("1"));
	else
	{
		tmp = __itoa(__atoi(get_value(msh, "SHLVL")) + 1);
		if (!tmp)
			return (NULL);
		return (tmp);
	}
}

int	update_shlvl(t_msh *msh)
{
	char	*values[2];
	char	*shlvl;
	char	*up_shlvl;

	up_shlvl = increment_shlvl(msh);
	if (!up_shlvl)
		return (__MALLOC);
	shlvl = __strdup("SHLVL=");
	if (!shlvl)
		return (free(up_shlvl), __MALLOC);
	shlvl = __strjoin(shlvl, up_shlvl);
	free(up_shlvl);
	if (!shlvl)
		return (free(shlvl), __MALLOC);
	values[0] = shlvl;
	values[1] = NULL;
	if (__export(values, msh) == __MALLOC)
		return (free(shlvl), __MALLOC);
	free(shlvl);
	return (__SUCCESS);
}

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
		return (print_error("cd", NULL, strerror(errno)), __FAIL);
	old_pwd = __strdup("OLDPWD=");
	if (!old_pwd)
		return (__MALLOC);
	old_pwd = __strjoin(old_pwd, cwd);
	if (!old_pwd)
		return (__MALLOC);
	values[0] = old_pwd;
	values[1] = NULL;
	if (__export(values, msh) == __MALLOC)
		return (free(old_pwd), __MALLOC);
	free(old_pwd);
	return (__SUCCESS);
}

int	update_pwd(t_msh *msh)
{
	char	*values[2];
	char	*pwd;
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (print_error("cd", "getcwcd", strerror(errno)), __FAIL);
	pwd = __strdup("PWD=");
	if (!pwd)
		return (__MALLOC);
	pwd = __strjoin(pwd, cwd);
	if (!pwd)
		return (__MALLOC);
	values[0] = pwd;
	values[1] = NULL;
	if (__export(values, msh) == __MALLOC)
		return (free(pwd), __MALLOC);
	free(pwd);
	return (__SUCCESS);
}

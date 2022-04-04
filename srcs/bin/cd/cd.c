/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:13:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/04 12:58:30 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	chdir_absolute_path(char *new_path, t_msh *msh)
{
	char	*final_path;
	char	pwd[PATH_MAX];

	final_path = NULL;
	if (get_key(msh, "CDPATH")
		&& __try_path_cdpath(new_path, msh, &final_path) == 2)
		return (__MALLOC);
	if (final_path)
		return (__try_change_dir(final_path, new_path, msh));
	if (!getcwd(pwd, PATH_MAX))
		return (print_error("cd", "getcwcd", strerror(errno)), __FAIL);
	final_path = create_absolut_pwd(pwd, new_path);
	if (!final_path)
		return (__MALLOC);
	return (__try_change_dir(final_path, new_path, msh));
}

int	chdir_previous(t_msh *msh)
{
	char	*save;

	if (!get_key(msh, "OLDPWD") || !__access_dir(get_key(msh, "OLDPWD"),
			get_key(msh, "OLDPWD")))
		return (__FAIL);
	save = __strdup(get_key(msh, "OLDPWD"));
	if (!save)
		return (__MALLOC);
	if (update_oldpwd(msh) == __MALLOC)
		return (__MALLOC);
	chdir(save);
	free(save);
	return (update_pwd(msh));
}

int	__access_dir(char *dir, char *true_dir)
{
	struct stat	buff;

	if (access(dir, F_OK) < 0 || access(dir, X_OK) < 0)
		return (print_error("cd", true_dir, strerror(errno)), 0);
	stat(dir, &buff);
	if (!S_ISDIR(buff.st_mode))
	{
		__putstr_fd("minishell : cd : ", 2);
		__putstr_fd(dir, 2);
		__putendl_fd("Not a directory", 2);
		return (0);
	}
	return (1);
}

int	__check_home(t_msh *msh)
{
	char	*home_dir;

	home_dir = get_key(msh, "HOME");
	if (!home_dir)
		return (print_error("cd", "HOME not set", NULL), __FAIL);
	if (!home_dir[0])
		return (__SUCCESS);
	if (!__access_dir(home_dir, home_dir))
		return (__FAIL);
	if (update_oldpwd(msh) == __MALLOC)
		return (__MALLOC);
	chdir(home_dir);
	if (update_pwd(msh) == __MALLOC)
		return (__MALLOC);
	return (__SUCCESS);
}

int	__cd(char **new_path, t_msh *msh)
{
	if (new_path[0] && new_path[1])
		return (print_error("cd", "too many arguments", NULL), __FAIL);
	if (!new_path[0])
		return (__check_home(msh));
	else if (!__strcmp(new_path[0], "-"))
		return (chdir_previous(msh));
	else
	{
		if (new_path[0][0] == '/')
		{
			if (!__access_dir(new_path[0], new_path[0]))
				return (__FAIL);
			if (update_oldpwd(msh) == __MALLOC)
				return (__MALLOC);
			chdir(new_path[0]);
			return (update_pwd(msh));
		}
		else
			return (chdir_absolute_path(new_path[0], msh));
	}
	return (__FAIL);
}
